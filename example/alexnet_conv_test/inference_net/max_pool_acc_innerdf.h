//
// Created by Yao Chen on 27/05/2017
//

#ifndef _MAX_POOL_ACC_H_
#define _MAX_POOL_ACC_H_

#include <iostream>
#include <fstream>
//#include "activation_functions.h"

#if _C_DEBUG_MODE_
#include <algorithm>
#endif

using namespace std;

template <typename T, typename W, typename G, int Tn, int Tr, int Tc, int S_max, int K_max>
class max_pool_acc {

private:
    int pool_layer_number;

public:
    max_pool_acc() : pool_layer_number(0) {pool_layer_number = 0;};

    ////------------------------------C++ debugging functions---------------------------------------////

    // Reset output buffer
    void out_buf_reset(G buf[][Tr][Tc]){
        for(int i = 0; i < Tn; i++){
            for(int j = 0; j < Tr; j++){
                for(int k = 0; k < Tc; k++){
                    buf[i][j][k] = G(0);
                }
            }
        }
    }
    ////-----------------------------Accelerator Functions---------------------------------------////
    // Load input data
    void in_buf_load(T buf[][(Tr-1)*S_max + K_max][(Tc-1)*S_max + K_max], T *in_data, int n, int r, int c, int S, int K, int P, int R, int C, int N, int R_IN, int C_IN, int TR, int TC) {
        for (int i = n; i < n+Tn; i++) {
            if(N < n+Tn && i == N){
                break;
            }
            for (int j = r * S - P; j < r * S + TR - P; j++) {
                for (int k = c * S - P; k < c * S + TC - P; k++) {
                    if (j < 0 || j >= (R_IN - 2 * P) || k < 0 || k >= (C_IN - 2 * P)) {
                        buf[i - n][j - r * S + P][k - c * S + P] = 0;
                    } else {
                        buf[i - n][j - r * S + P][k - c * S + P] = *(in_data +
                                                                        i * (R_IN - 2 * P) *
                                                                        (C_IN - 2 * P) +
                                                                        j * (C_IN - 2 * P) + k);
                    }
                }
            }
        }
    }
    // Max pooling computation kernel
    void pool_engine(T in_buf[][(Tr-1)*S_max + K_max][(Tc-1)*S_max + K_max], G out_buf[][Tr][Tc], int S, int n, int r, int c, int K, int R, int C, int TR, int TC){
        for (int i = 0; i < K; i++) {
            for (int j = 0; j < K; j++) {
                for(int tr=0; tr<Tr; tr++){
                    if(R < r+Tr && tr+r == R){
                        break;
                    }
                    for(int tc=0; tc<Tc; tc++){
#pragma HLS PIPELINE
                        if(C < c+Tc && tc+c == C){
                            break;
                        }
                        for(int tn=0; tn<Tn; tn++){ // unroll loop kernel
#pragma HLS UNROLL
                            if((S * (tr) + i)>=TR||(S * (tc) + j)>=TC){
                                break;
                            }
                            if(i==0&&j==0){
                                out_buf[tn][tr][tc] = in_buf[tn][S * (tr)][S * (tc)];
                            }else{
                                out_buf[tn][tr][tc] = (out_buf[tn][tr][tc] > in_buf[tn][S * (tr) + i][S * (tc) + j]) ? out_buf[tn][tr][tc] : in_buf[tn][S * (tr) + i][S * (tc) + j];
                            }
                        }
                    }
                }
            }
        }
    }
    // Ouput out_buf data to output interface
    void output_res(G out_buf[][Tr][Tc], G *out_data, int n, int r, int c, int N, int R, int C, bool act){
            for(int i = n; i < n+Tn; i++){
                if(N < n+Tn && i == N){ break; }
                for (int j = r; j < r + Tr; j++) {
                    if (R < r + Tr && j == R) { break; }
                    for (int k = c; k < c + Tc; k++) {
                        if (C < c + Tc && k == C) { break; }
                        if (act) {
                            if (out_buf[i - n][j - r][k - c] > G(0)) {
                                *(out_data + i * R * C + j * C + k) = (out_buf[i - n][j - r][k - c]);
                            } else {
                                *(out_data + i * R * C + j * C + k) = G(0);
                            }
                        }
                        else {
                            *(out_data + i * R * C + j * C + k) = out_buf[i - n][j - r][k - c];
                        }
                    }
                }
            }
    }

///////////////////////------------------pooling accelerator----------------//////////////////////////
    void max_pool_layer_acc(
            int R_IN,// input Row
            int C_IN,// input column
            int N, //input feature number
            int K, //input kernel size
            int R, // output Row
            int C, // output column
            int S, // stride size, always equal to K
            int P, // padding size
            bool act, // activation function bit (1-- with act, 0--without act)
            T *in_data, // in_data[N][(R-1)*S + K][(C-1)*S + K] --> [N][(R-1)*S + K - 2*P][(C-1)*S + K - 2*P]
            G *out_data) { // out[M][R][C]

        /***************local data buffer******************************/
        int TR=0;
        int TC=0;
        T in_buf[Tn][(Tr - 1) * S_max + K_max][(Tc - 1) * S_max + K_max];
        G out_buf[Tn][Tr][Tc];

#if _HLS_MODE_
#pragma HLS ARRAY_PARTITION variable=in_buf complete dim=1
#pragma HLS ARRAY_PARTITION variable=out_buf complete dim=1
#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
            cout << "Starting max_pool_acc layer ...." << endl;
            //buffer local data initiallization: must do it in C++ debug!
            out_buf_reset(out_buf);
#endif
#endif

        for (int r = 0; r < R; r += Tr) {
            for (int c = 0; c < C; c += Tc) {
                TR=((r * S + (Tr - 1) * S + K)>R_IN?(R_IN - r * S):((Tr - 1) * S + K));
                TC=((c * S + (Tc - 1) * S + K)>C_IN?(C_IN - c * S):((Tc - 1) * S + K));
                for (int n = 0; n < N; n += Tn) {
#if _HLS_MODE_
#pragma HLS DATAFLOW
#endif
  //--------------------------Load input B W D in ping-pong manner-------------------------//
                    // load input data
                    in_buf_load(in_buf, in_data, n, r, c, S, K, P, R, C, N, R_IN, C_IN, TR, TC);
  //------------------------------compute buffered data -----------------------------------//
                    pool_engine(in_buf, out_buf, S, n, r, c, K, R, C, TR, TC);
  //---------------------------transfer output data----------------------------------------//
                    // transfer output data
                    output_res(out_buf, out_data, n, r, c, N, R, C, act);
#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
                    // transfer output data
                    ofstream pool_out_buf;
                    pool_out_buf.open("max_pool_out_buf.txt", ios::app);
                    pool_out_buf <<"pool out buf: "<< endl;
                    for(int i = n; i < min(N, n+Tn); i++){
                        for(int j=r; j < min(R, r+Tr); j++){
                            for(int k=c; k < min(C, c+Tc); k++){
                                pool_out_buf << out_buf[i-n][j-r][k-c] << " ";
                            }
                            pool_out_buf << endl;
                        }
                        pool_out_buf << endl;
                    }
                    pool_out_buf.close();
#endif
#endif
                }
            }
        }
    }
};

#endif
