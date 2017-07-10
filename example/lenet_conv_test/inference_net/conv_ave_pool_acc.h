//
// Created by Yao Chen on 27/05/2017
// 

#ifndef _CONV_AVE_POOL_ACC_H_
#define _CONV_AVE_POOL_ACC_H_

#include <iostream>
#include <fstream>
#include "activation_functions.h"

#if _C_DEBUG_MODE_
#include <algorithm>
#endif

using namespace std;

template <typename T, typename W, typename G, int Tm, int Tn, int Tr, int Tc>
class conv_ave_pool_acc {

private:
    int conv_layer_number;

public:
    conv_ave_pool_acc() : conv_layer_number(0) {conv_layer_number = 0;};

///////////////////////------------------conv accelerator----------------//////////////////////////
    void conv_ave_pool_layer_acc(
        int N, //input feature number
        int K, //input kernel size
        int K_P, //pooling input kernel size
        int M, // output feature number
        int R, // output Row
        int C, // output column
        int R_P, // pooling output Row
        int C_P, // pooling output column
        int S, // stride size
        int S_P, //pooling stride size
        int P, // padding size
        T *in_data, // in_data[N][(R-1)*S + K][(C-1)*S + K] --> [N][(R-1)*S + K - 2*P][(C-1)*S + K - 2*P]
        W *layer_weights, //w[M][N][K][K]
        W *layer_bias, // b[M]
        G *out_data,
        int weight_offset,
        int bias_offset,
    int in_offset,
    int out_offset){ // out[M][R][C]

#if _HLS_MODE_
#pragma HLS DATAFLOW
#endif

            /***************local data buffer******************************/
            T in_buf[Tn][(Tr-1)*1 + 5][(Tc-1)*1 + 5];
            G out_buf[Tm][Tr][Tc];
            G out_buf_temp[Tm][Tr/2][Tc/2];
            W w_buf[Tn][Tm][5][5];
            W b_buf[Tm];

#if _HLS_MODE_
#pragma HLS ARRAY_PARTITION variable=in_buf complete dim=1
#pragma HLS ARRAY_PARTITION variable=w_buf complete dim=1
#pragma HLS ARRAY_PARTITION variable=w_buf complete dim=2
#pragma HLS ARRAY_PARTITION variable=b_buf complete dim=1
#pragma HLS ARRAY_PARTITION variable=out_buf complete dim=1
#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
            cout << "Starting conv_ave_pool_acc layer ...." << endl;
            //buffer local data initiallization:must do it!
            for(int i = 0; i < Tm; i++){
                for(int j = 0; j < Tr; j++){
                    for(int k = 0; k < Tc; k++){
                        out_buf[i][j][k] = G(0);
                    }
                }
            }
            for(int i = 0; i < Tm; i++){
                for(int j = 0; j < Tr/2; j++){
                    for(int k = 0; k < Tc/2; k++){
                        out_buf_temp[i][j][k] = G(0);
                    }
                }
            }
            for(int i = 0; i < Tn; i++){
                for(int j = 0; j < Tm; j++){
                    for(int k = 0; k < K; k++){
                        for(int l = 0; l < K; l++){
                            w_buf[i][j][k][l] = W(0);
                        }
                    }
                }
            }
            for(int i = 0; i < Tm; i++){
                b_buf[i]= W(0);
            }
#endif
#endif
            for(int r = 0; r < R; r += Tr){
                for(int c = 0; c < C; c += Tc){
                    for(int m = 0; m < M; m += Tm){
                        for(int n = 0; n < N; n += Tn){
                            
                            // load input data
                            for(int i = n; i < n+Tn; i++){
                                //if(N < n+Tn && i == N){
                                //    break;
                                //}
                                for(int j = r*S - P; j < (r+Tr-1)*S + K - P; j++){
                                    for(int k = c*S - P; k < (c+Tc-1)*S + K - P; k++){
                                        if(j < 0 || j >= ((R-1)*S + K - 2*P) || k < 0 || k >= ((C-1)*S + K - 2*P)){
                                            in_buf[i-n][j-r*S+P][k-c*S+P] = T(0);}
                                        else{
                                            if(N < n+Tn && i >= N){
                                                in_buf[i-n][j-r*S+P][k-c*S+P] = T(0);
                                            }else{
                                                in_buf[i-n][j-r*S+P][k-c*S+P] = *(in_data + in_offset + i*((R-1)*S+K - 2*P)*((C-1)*S+K - 2*P) + j*((C-1)*S+K - 2*P) +k);
                                            }
                                        }
                                    }
                                }
                            }

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
                            ofstream conv_acc;
                            conv_acc.open("conv_in_data.txt", ios::app);
                            conv_acc <<"conv input: "<< endl;
                            for (int i = n; i < min(N, n+Tn); i++) {
                                for (int j = 0; j < (Tr-1)*S + K; j++) {
                                    for(int k = 0; k < (Tc-1)*S + K; k++){
                                        conv_acc << in_buf[i-n][j][k] << " ";
                                    }
                                    conv_acc << endl;
                                }
                                conv_acc << endl;
                            }
                            conv_acc.close();
#endif
#endif
                            // load input weights
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
                                            w_buf[j-n][i-m][k1][k2] = *(layer_weights + weight_offset + i*N*K*K + j*K*K + k1*K + k2);
                                        }
                                    }
                                }
                            }
                            //load input bias
                            for(int i = m; i < m+Tm; i++){
                                b_buf[i-m] = *(layer_bias + bias_offset + i);
                            }
#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
                            ofstream conv_w;
                            conv_w.open("conv_in_weights.txt", ios::app);
                            for (int j = n; j < min(N, n+Tn); j++) {
                                for (int i = m; i < min(M, m+Tm); i++) {
                                    for(int k = 0; k < K; k++){
                                        for(int l = 0; l < K; l++){
                                            conv_w << w_buf[j-n][i-m][k][l] << " ";
                                        }
                                        conv_w << endl;
                                    }
                                    conv_w << endl;
                                }
                                conv_w << endl;
                            }
                            conv_w.close();
#endif
#endif
                            // convolutional accelerator
                            for(int i=0; i<K; i++){
                                for(int j=0; j<K; j++){
                                    for(int tr=0; tr<Tr; tr++){
                                        if(R < r+Tr && tr+r == R){
                                            break;
                                        }
                                        for(int tc=0; tc<Tc; tc++){
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE variable=out_buf inter false
                                            if(C < c+Tc && tc+c == C){
                                                break;
                                            }
                                            for(int tm = 0; tm < Tm; tm++){
#pragma HLS UNROLL
                                                //if(M < m+Tm && tm+m == M){
                                                //    break;
                                                //}
                                                for(int tn=0; tn<Tn; tn++){
#pragma HLS UNROLL
                                                    //if(N < n+Tn && tn+n == N){
                                                    //    break;
                                                    //}
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

                        // ave pooling & transfer output data
                        int TR=0;//true pooling input row
                        int TC=0;//true pooling input col
                        for (int i = 0; i < K_P; i++) {
                        for (int j = 0; j < K_P; j++) {
                            for(int tr=0; tr < Tr; tr+=S_P){
                                TR=((r+Tr>=R)?(R-r):Tr);
                                if(R < r+Tr && tr+r == R){
                                    break;
                                }
                                for(int tc=0; tc < Tc; tc+=S_P){
                                    TC=((c+Tc>=C)?(C-c):Tc);
                                    if(C < c+Tc && tc+c == C){
                                        break;
                                    }
                                    for(int tm = 0; tm < Tm; tm++){
                                    if(M < m+Tm && tm + m == M){
                                       break;
                                    }
                                    if((tr + i)>=TR){
                                            break;
                                        }
                                    if((tc + j)>=TC){
                                            break;
                                        }
                                    out_buf_temp[tm][tr/S_P][tc/S_P] += relu(out_buf[tm][tr + i][tc + j]);
                                    if(i+1==((tr + K_P)>TR?(TR-tr):K_P)&&j+1==((tc + K_P)>TC?(TC-tc):K_P)){
                                        out_buf_temp[tm][tr/S_P][tc/S_P] = (T)(out_buf_temp[tm][tr/S_P][tc/S_P] / (((tr + K_P)>TR?(TR-tr):K_P) * ((tc + K_P)>TC?(TC-tc):K_P)));
                                        *(out_data + out_offset + (tm + m) * R_P * C_P + (tr/S_P+r/S_P) * C_P + tc/S_P+c/S_P) = relu(out_buf_temp[tm][tr/S_P][tc/S_P]);
                                        out_buf_temp[tm][tr/S_P][tc/S_P] = G(0);
                                    }
                                }
                            }
                        }
                    }
                }
#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
                        ofstream conv_out_buf;
                        conv_out_buf.open("conv_out_buf.txt", ios::app);
                        conv_out_buf <<"conv out buf: "<< endl;
                        for(int i = m; i < min(M, m+Tm); i++){
                            for(int j=r; j < min(R, r+Tr); j++){
                                for(int k=c; k < min(C, c+Tc); k++){
                                    conv_out_buf << out_buf[i-m][j-r][k-c] << " ";
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

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
            cout << "Finished conv_ave_pool_acc layer ...." << endl;
            cout << endl;
            ofstream conv_out;
            conv_out.open("conv_ave_pool_out_data.txt", ios::app);
            conv_out <<"conv_ave_pool output: "<< endl;
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < R_P; j++) {
                    for(int k = 0; k < C_P; k++){
                        conv_out << *(out_data + out_offset + i*R_P*C_P + j*C_P + k) << " ";
                    }
                    conv_out << endl;
                }
                conv_out << endl;
            }
            conv_out.close();
#endif
#endif
        }
};

#endif
