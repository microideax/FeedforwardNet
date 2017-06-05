//
// Created by Yao Chen on 27/05/2017
//

#ifndef _MAX_POOL_ACC_H_
#define _MAX_POOL_ACC_H_

#include <iostream>
#include <fstream>
//#include "activation_functions.h"

using namespace std;

template <typename T, typename W, typename G, int Tm, int Tn, int Tr, int Tc>
class max_pool_acc {

private:
    int pool_layer_number;

public:
    max_pool_acc() : pool_layer_number(0) {pool_layer_number = 0;};

///////////////////////------------------pooling accelerator----------------//////////////////////////
    void max_pool_layer_acc(
            int N, //input feature number
            int K, //input kernel size
            int M, // output feature number
            int R, // output Row
            int C, // output column
            int S, // stride size, always equal to K
            int P, // padding size
            T *in_data, // in_data[N][(R-1)*S + K][(C-1)*S + K] --> [N][(R-1)*S + K - 2*P][(C-1)*S + K - 2*P]
            G *out_data) { // out[M][R][C]

//stride size S=K

        //buffer local data before computation
        T in_buf[Tn][(Tr - 1) * S + K][(Tc - 1) * S + K];
        G out_buf[Tn][Tr][Tc];

        for (int r = 0; r < R; r += Tr) {
            for (int c = 0; c < C; c += Tc) {
//                for(int m = 0; m < M; m += Tm){
                for (int n = 0; n < N; n += Tn) {

                    // load input data
                    for (int i = n; i < n + Tn; i++) {
                        for (int j = r * S - P; j < (r + Tr - 1) * S + K - P; j++) {
                            for (int k = c * S - P; k < (c + Tc - 1) * S + K - P; k++) {
                                if (j < 0 || j >= ((R - 1) * S + K - 2 * P) || k < 0 ||
                                    k >= ((C - 1) * S + K - 2 * P)) {
                                    in_buf[i - n][j - r * S + P][k - c * S + P] = 0;
                                } else {
                                    in_buf[i - n][j - r * S + P][k - c * S + P] = *(in_data +
                                                                                    i * ((R - 1) * S + K - 2 * P) *
                                                                                    ((C - 1) * S + K - 2 * P) +
                                                                                    j * ((C - 1) * S + K - 2 * P) + k);
                                }
//            							    in_buf[i-n][j-r*S][k-c*S] = *(in_data + i*((R-1)*S+K)*((C-1)*S+K) + j*((C-1)*S+K) +k);
                            }
                        }
                    }
#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
                    ofstream pool_in_data;
                    pool_in_data.open("pool_in_data.txt", ios::app);
                    pool_in_data <<"pool input: "<< endl;
                    for (int i = 0; i < Tn; i++) {
                        for (int j = 0; j < (Tr-1)*S + K; j++) {
                            for(int k = 0; k < (Tc-1)*S + K; k++){
                                pool_in_data << in_buf[i][j][k] << " ";
                            }
                            pool_in_data << endl;
                        }
                        pool_in_data << endl;
                    }
//                    pool_in_data.close();
#endif
#endif

                    // convolutional accelerator
//                        for(int i=0; i<K; i++){
//                            for(int j=0; j<K; j++){
                    for (int tr = 0; tr + r < min(R, r + Tr); tr++) {
                        for (int tc = 0; tc + c < min(C, c + Tc); tc++) {
//                                        for(int tm=0; tm<Tm; tm++){ // unroll loop kernel
                            for (int tn = 0; tn < Tn; tn++) { // unroll loop kernel
                                T max = 0;
                                for (int i = 0; i < K; i++) {
                                    for (int j = 0; j < K; j++) {
                                        max = (max > in_buf[tn][S * tr + i][S * tc + j]) ? max : in_buf[tn][S * tr + i][
                                                S * tc + j];
                                    }
                                }
                                out_buf[tn][tr][tc] = max;
                            }
                        }
                    }
//                                }
//                        }
//                    }

                    // transfer output data
                    ofstream pool_out_buf;
                    pool_out_buf.open("pool_out_buf.txt", ios::app);
                    int flag1 = 0;
                    int flag2 = 0;
                    if (R < r + Tr) {
                        flag1 = 1;
                    }
                    if (C < c + Tc) {
                        flag2 = 1;
                    }
                    for (int i = 0; i < Tn; i++) {
                        for (int j = 0; j < (flag1 > 0 ? (R % Tr) : Tr); j++) {
                            for (int k = 0; k < (flag2 > 0 ? (C % Tc) : Tc); k++) {
                                *(out_data + (i + n) * R * C + (j + r) * C + k + c) = out_buf[i][j][k];
                                pool_out_buf << out_buf[i][j][k] << " ";
                            }
                            pool_out_buf << endl;
                        }
                        pool_out_buf << endl;
                    }
                    pool_out_buf.close();
//                }
                }
            }
        }

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        ofstream pool_out;
        pool_out.open("pool_out_data.txt", ios::app);
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