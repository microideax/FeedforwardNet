//
// Created by Yao Chen on 27/05/2017
// 

#ifndef _CONV_ACC_H_
#define _CONV_ACC_H_

#include <iostream>
#include <fstream>
//#include "activation_functions.h"

#if _C_DEBUG_MODE_
#include <algorithm>
#endif

using namespace std;

template <typename T, typename W, typename G, int Tm, int Tn, int Tr, int Tc>
class conv_acc {

private:
    int conv_layer_number;

public:
    conv_acc() : conv_layer_number(0) {conv_layer_number = 0;};

    ////------------------------------C++ debugging functions---------------------------------------////

    // Reset output buffer
    void out_buf_reset(G buf[][Tr][Tc]){
        for(int i = 0; i < Tm; i++){
            for(int j = 0; j < Tr; j++){
                for(int k = 0; k < Tc; k++){
                    buf[i][j][k] = G(0);
                }
            }
        }
    }
    // Reset weight buffer
    void w_buf_reset(int K, W buf[][Tm][11][11]){
        for(int i = 0; i < Tn; i++){
            for(int j = 0; j < Tm; j++){
                for(int k = 0; k < K; k++){
                    for(int l = 0; l < K; l++){
                        buf[i][j][k][l] = W(0);
                    }
                }
            }
        }
    }
    // Reset bias buffer
    void b_buf_reset(W buf[]){
        for(int i = 0; i < Tm; i++){
            buf[i]= W(0);
        }
    }
    ////-----------------------------Accelerator Functions---------------------------------------////
    // Load bias data
    void b_buf_load(W buf[], W *layer_bias, int bias_offset, int m){
        for(int i = 0; i < Tm; i++){
#pragma HLS UNROLL
//        #pragma HLS PIPELINE
            buf[i] = *(layer_bias + bias_offset + i + m);
        }
    }
    // Load input data
    void in_buf_load(T buf[][(Tr-1)*6 + 11][(Tc-1)*6 + 11], T *in_data, int in_offset, int n, int r, int c, int S, int K, int P, int R, int C, int N) {
        for (int i = n; i < n + Tn; i++) {
            for (int j = r * S - P; j < (r + Tr - 1) * S + K - P; j++) {
                for (int k = c * S - P; k < (c + Tc - 1) * S + K - P; k++) {
                    if (j < 0 || j >= ((R - 1) * S + K - 2 * P) || k < 0 || k >= ((C - 1) * S + K - 2 * P)) {
                        buf[i - n][j - r * S + P][k - c * S + P] = T(0);
                    } else {
                        if (N < n + Tn && i >= N) {
                            buf[i - n][j - r * S + P][k - c * S + P] = T(0);
                        } else {
                            buf[i - n][j - r * S + P][k - c * S + P] = *(in_data + in_offset +
                                                                            i * ((R - 1) * S + K - 2 * P) *
                                                                            ((C - 1) * S + K - 2 * P) +
                                                                            j * ((C - 1) * S + K - 2 * P) + k);
                        }
                    }
                }
            }
        }
    }
    // Load weights to weight buffer
    void w_buf_load(W buf[][Tm][11][11], W *layer_weights, int weight_offset, int n, int m, int K, int N, int M){
        for(int j = n; j < n+Tn; j++){
            if(N < n+Tn && j == N){
                break;
            }
            for(int i = m; i < m+Tm; i++){
                if(M < m+Tm && i == M){
                    break;
                }
                for(int k1 = 0; k1 < K; k1++){
                    for(int k2 = 0; k2 < K; k2++){
                        buf[j-n][i-m][k1][k2] = *(layer_weights + weight_offset + i*N*K*K + j*K*K + k1*K + k2);
                    }
                }
            }
        }
    }
    // Convolution computation kernel
    void conv_engine(T in_buf[][(Tr-1)*6 + 11][(Tc-1)*6 + 11], W w_buf[][Tm][11][11], W b_buf[], G out_buf[][Tr][Tc], int S, int n, int r, int c, int K, int R, int C){
        for(int i=0; i<K; i++){
            for(int j=0; j<K; j++){
                for(int tr=0; tr<Tr; tr++){
                    if(R < r+Tr && tr+r == R){
                        break;
                    }
                    for(int tc=0; tc<Tc; tc++){
#pragma HLS PIPELINE
                        if(C < c+Tc && tc+c == C){
                            break;
                        }
                        for(int tm = 0; tm < Tm; tm++){
#pragma HLS UNROLL
                            for(int tn=0; tn<Tn; tn++){
#pragma HLS UNROLL
                                if(i==0&&j==0&&tn==0&&n==0)
                                    out_buf[tm][tr][tc] = b_buf[tm] + w_buf[tn][tm][i][j]*in_buf[tn][S*(tr)+i][S*(tc)+j];
                                else
                                    out_buf[tm][tr][tc] = out_buf[tm][tr][tc] + w_buf[tn][tm][i][j]*in_buf[tn][S*(tr)+i][S*(tc)+j];
                            }
                        }
                    }
                }
            }
        }
    }
    // Ouput out_buf data to output interface
    void output_res(G out_buf[][Tr][Tc], G *out_data, int out_offset, int n, int m, int r, int c, int N, int M, int R, int C){
        if (n >= N - Tn) {
            for (int i = m; i < m + Tm; i++) {
                if (M < m + Tm && i == M) { break; }
                for (int j = r; j < r + Tr; j++) {
                    if (R < r + Tr && j == R) { break; }
                    for (int k = c; k < c + Tc; k++) {
                        if (C < c + Tc && k == C) { break; }
                        if (out_buf[i - m][j - r][k - c] > G(0)) {
                            *(out_data + out_offset + i * R * C + j * C + k) = (out_buf[i - m][j - r][k - c]);
//                            out_buf[i - m][j - r][k - c] = G(0);
                        } else {
                            *(out_data + out_offset + i * R * C + j * C + k) = G(0);
//                            out_buf[i - m][j - r][k - c] = G(0);
                        }
//                                 *(out_data + i*R*C + j*C +k) = out_buf[i-m][j-r][k-c];
//                                 out_buf[i-m][j-r][k-c] = 0;
                    }
                }
            }
        }
    }
///////////////////////------------------conv accelerator----------------//////////////////////////
    void conv_layer_acc(
            int N, //input feature number
            int K, //input kernel size
            int M, // output feature number
            int R, // output Row
            int C, // output column
            int S, // stride size
            int P, // padding size
            T *in_data, // in_data[N][(R-1)*S + K][(C-1)*S + K] --> [N][(R-1)*S + K - 2*P][(C-1)*S + K - 2*P]
            W *layer_weights, //w[M][N][K][K]
            W *layer_bias, // b[M]
            G *out_data,
            int weight_offset,
            int bias_offset,
            int in_offset,
            int out_offset){ // out[M][R][C]

        /***************local data buffer******************************/
        T in_buf_1[Tn][(Tr-1)*6 + 11][(Tc-1)*6 + 11];
//        T in_buf_2[Tn][(Tr-1)*6 + 11][(Tc-1)*6 + 11];
        W w_buf_1[Tn][Tm][11][11];
//        W w_buf_2[Tn][Tm][11][11];
        W b_buf_1[Tm];
//        W b_buf_2[Tm];

        G out_buf[Tm][Tr][Tc];
//        G out_buf_2[Tm][Tr][Tc];

        bool buf_1_full = 0;
        bool buf_2_full = 0;
        bool buf_1_act = 0;
        bool buf_2_act = 0;
        int loadbufPtr = 0;
        int combufPtr = 0;

//        T (*act_in_buf)[(Tr-1)*6 + 11][(Tc-1)*6 + 11] = in_buf_1;
//        W (*act_w_buf)[Tm][11][11] = w_buf_1;
//        W *act_b_buf = b_buf_1;
//        T (*com_in_buf)[(Tr-1)*6 + 11][(Tc-1)*6 + 11] = in_buf_1;
//        W (*com_w_buf)[Tm][11][11] = w_buf_1;
//        W *com_b_buf = b_buf_1;
//        G (*act_out_buf)[Tr][Tc] = out_buf_1;

#if _HLS_MODE_
#pragma HLS ARRAY_PARTITION variable=in_buf_1 complete dim=1
//#pragma HLS ARRAY_PARTITION variable=in_buf_2 complete dim=1
#pragma HLS ARRAY_PARTITION variable=w_buf_1 complete dim=1
#pragma HLS ARRAY_PARTITION variable=w_buf_1 complete dim=2
//#pragma HLS ARRAY_PARTITION variable=w_buf_2 complete dim=1
//#pragma HLS ARRAY_PARTITION variable=w_buf_2 complete dim=1
#pragma HLS ARRAY_PARTITION variable=b_buf_1 complete dim=1
//#pragma HLS ARRAY_PARTITION variable=b_buf_2 complete dim=1
#pragma HLS ARRAY_PARTITION variable=out_buf complete dim=1
#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
            cout << "Starting conv_acc layer ...." << endl;
            //buffer local data initiallization: must do it in C++ debug!
            out_buf_reset(out_buf);
//            out_buf_reset(out_buf_2);
            b_buf_reset(b_buf_1);
//            b_buf_reset(b_buf_2);
            w_buf_reset(K, w_buf_1);
//            w_buf_reset(K, w_buf_2);
#endif
#endif
        int counter =0;

        for(int r = 0; r < R; r += Tr){
            for(int c = 0; c < C; c += Tc){
                for(int m = 0; m < M; m += Tm){
                    for(int n = 0; n < N; n += Tn){
#if _HLS_MODE_
#pragma HLS DATAFLOW
#endif
   //--------------------------Load input B W D in ping-pong manner-------------------------//
                        //load input bias
                        b_buf_load(b_buf_1, layer_bias, bias_offset, m);
                        // load input data
                        in_buf_load(in_buf_1, in_data, in_offset, n, r, c, S, K, P, R, C, N);
                        // load input weights
                        w_buf_load(w_buf_1, layer_weights, weight_offset, n, m, K, N, M);
  //------------------------------compute buffered data -----------------------------------//
                        conv_engine(in_buf_1, w_buf_1, b_buf_1, out_buf, S, n, r, c, K, R, C);

                        // transfer output data
                        output_res(out_buf, out_data, out_offset, n, m, r, c, N, M, R, C);
                    }

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
                        ofstream conv_out_buf;
                        conv_out_buf.open("conv_out_buf.txt", ios::app);
                        conv_out_buf <<"conv out buf: "<< endl;
                        for(int i = m; i < min(M, m+Tm); i++){
                            for(int j=r; j < min(R, r+Tr); j++){
                                for(int k=c; k < min(C, c+Tc); k++){
                                    conv_out_buf << *(out_data + out_offset + i * R * C + j * C + k) << " ";
                                }
                                conv_out_buf << endl;
                            }
                            conv_out_buf << endl;
                        }
                        conv_out_buf.close();
#endif
#endif
                }
            }
        }
    }
};

#endif

/*


#if _C_DEBUG_MODE_
                        #if _KERNEL_DEBUG_
//                            ofstream conv_w;
//                            conv_w.open("conv_in_weights.txt", ios::app);
//                            for (int j = n; j < min(N, n+Tn); j++) {
//                                for (int i = m; i < min(M, m+Tm); i++) {
//                                    for(int k = 0; k < K; k++){
//                                        for(int l = 0; l < K; l++){
//                                            conv_w << w_buf[j-n][i-m][k][l] << " ";
//                                        }
//                                        conv_w << endl;
//                                    }
//                                    conv_w << endl;
//                                }
//                                conv_w << endl;
//                            }
//                            conv_w.close();
#endif
#endif



#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
            cout << "Finished conv_acc layer ...." << endl;
            cout << endl;
            ofstream conv_out;
            conv_out.open("conv_out_data.txt", ios::app);
            conv_out <<"conv output: "<< endl;
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < R; j++) {
                    for(int k = 0; k < C; k++){
                        conv_out << *(out_data + out_offset + i*R*C + j*C + k) << " ";
                    }
                    conv_out << endl;
                }
                conv_out << endl;
            }
            conv_out.close();
#endif
#endif
*/
