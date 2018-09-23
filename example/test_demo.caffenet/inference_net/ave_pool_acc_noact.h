//
// Created by Yao Chen on 27/05/2017
//

#ifndef _AVE_POOL_ACC_NOACT_H_
#define _AVE_POOL_ACC_NOACT_H_

#include <iostream>
#include <fstream>
//#include "activation_functions.h"

using namespace std;

template <typename T, typename W, typename G, int Tn, int Tr, int Tc>
class ave_pool_acc_noact {

private:
    int pool_layer_number;

public:
    ave_pool_acc_noact() : pool_layer_number(0) {pool_layer_number = 0;};

///////////////////////------------------pooling accelerator----------------//////////////////////////
    void ave_pool_layer_acc_noact(
            int R_IN,// input Row
            int C_IN,// input column
            int N, //input feature number
            int K, //input kernel size
            int R, // output Row
            int C, // output column
            int S, // stride size
            int P, // padding size
            T *in_data, // in_data[N][(R-1)*S + K][(C-1)*S + K] --> [N][(R-1)*S + K - 2*P][(C-1)*S + K - 2*P]
            G *out_data) { // out[M][R][C]

//stride size S=K
        
#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
            cout << "Starting ave_pool_acc_noact layer ...." << endl;
#endif
#endif
        //buffer local data before computation
        //int TR=(((Tr - 1) * S + K)>R_IN?R_IN:((Tr - 1) * S + K));
        //int TC=(((Tc - 1) * S + K)>C_IN?C_IN:((Tc - 1) * S + K));
        int TR=0;
        int TC=0;

        T in_buf[Tn][(Tr - 1) * S + K][(Tc - 1) * S + K];
        G out_buf[Tn][Tr][Tc];

#if _HLS_MODE_
#pragma HLS ARRAY_PARTITION variable=in_buf complete dim=1
#pragma HLS ARRAY_PARTITION variable=out_buf complete dim=1
#endif

        //buffer local data initiallization:must do it!
        for(int i = 0; i < Tn; i++){
            for(int j = 0; j < Tr; j++){
                for(int k = 0; k < Tc; k++){
                    out_buf[i][j][k] = G(0);
                }
            }
        }



        for (int r = 0; r < R; r += Tr) {
            for (int c = 0; c < C; c += Tc) {
//                TR=((r * S + (Tr - 1) * S + K)>R_IN?(R_IN - r * S):((Tr - 1) * S + K));
//                TC=((c * S + (Tc - 1) * S + K)>C_IN?(C_IN - c * S):((Tc - 1) * S + K));
                for (int n = 0; n < N; n += Tn) {
                    // load input data
                    for (int i = n; i < n+Tn; i++) {
                        if(N < n+Tn && i == N){
                            break;
                        }
                        for (int j = r * S - P; j < (r + Tr - 1)*S + K; j++) {
                            for (int k = c * S - P; k < (c + Tc - 1)*S + K; k++) {
                                if (j < 0 || j >= ((R-1)*S + K - 2 * P) || k < 0 || k >= ((C-1)*S + K - 2 * P)) {
                                    in_buf[i - n][j - r * S + P][k - c * S + P] = 0;
                                } else {
                                    in_buf[i - n][j - r * S + P][k - c * S + P] = *(in_data +
                                                                                    i * ((R-1)*S + K - 2 * P) *
                                                                                    ((C-1)*S + K - 2 * P) +
                                                                                    j * ((C-1)*S + K - 2 * P) + k);
                                }
                            }
                        }
                    }
#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
                    ofstream pool_in_data;
                    pool_in_data.open("pool_in_data.txt", ios::app);
                    pool_in_data <<"pool input: "<< endl;
                    for (int i = n; i < min(N, n+Tn); i++) {
                        for (int j = 0; j < TR; j++) {
                            for(int k = 0; k < TC; k++){
                                pool_in_data << in_buf[i-n][j][k] << " ";
                            }
                            pool_in_data << endl;
                        }
                        pool_in_data << endl;
                    }
#endif
#endif

                    // ave pooling computation core
                    for (int i = 0; i < K; i++) {
                        for (int j = 0; j < K; j++) {
                            for(int tr=0; tr<Tr; tr++){
//#pragma HLS UNROLL
                                if(R < r+Tr && tr+r == R){
                                    break;
                                }
                                for(int tc=0; tc<Tc; tc++){
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE variable=out_buf inter false
                                    if(C < c+Tc && tc+c == C){
                                        break;
                                    }
                                    for(int tn=0; tn<Tn; tn++){ // unroll loop kernel
#pragma HLS UNROLL
//                                        if(N < n+Tn && tn+n == N){
//                                            break;
//                                        }
                                
//                                        if((S * (tr) + i)>=TR||(S * (tc) + j)>=TC){
//                                            break;
//                                        }
                                        out_buf[tn][tr][tc] += in_buf[tn][S * (tr) + i][S * (tc) + j]/(K*K);
//                                        if(i+1==((S * (tr) + K)>TR?(TR-S * (tr)):K)&&j+1==((S * (tc) + K)>TC?(TC-S * (tc)):K)){
//                                            out_buf[tn][tr][tc] = (T)(out_buf[tn][tr][tc] / (((S * (tr) + K)>TR?(TR-S * (tr)):K) * ((S * (tc) + K)>TC?(TC-S * (tc)):K)));
//                                        }
                                    }
                                }
                            }
                        }
                    }

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
                    // transfer output data
                    ofstream pool_out_buf;
                    pool_out_buf.open("ave_pool_out_buf.txt", ios::app);
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

                    // transfer output data
                    for(int i = n; i < n+Tn; i++){
                        if(N < n+Tn && i == N){
                            break;
                        }
                        for(int j=r; j < r+Tr; j++){
                            if(R < r+Tr && j == R){
                                break;
                            }
                            for(int k=c; k < c+Tc; k++){
                                if(C < c+Tc && k == C){
                                    break;
                                }
                                *(out_data + i * R * C + j * C + k) = out_buf[i-n][j-r][k-c];
                                out_buf[i-n][j-r][k-c] = G(0);
                            }
                        }
                    }
                }
            }
        }

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        cout << "Finished ave_pool_acc_noact layer ...." << endl;
        cout << endl;
        ofstream pool_out;
        pool_out.open("ave_pool_out_data.txt", ios::app);
        pool_out <<"pool output: "<< endl;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < R; j++) {
                for(int k = 0; k < C; k++){
                    pool_out << *(out_data + i*R*C + j*C + k) << " ";
                }
                pool_out << endl;
            }
            pool_out << endl;
        }
        pool_out.close();
#endif
#endif
    }

};

#endif
