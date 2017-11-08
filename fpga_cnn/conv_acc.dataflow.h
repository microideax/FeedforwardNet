#ifndef _CONV_ACC_H_
#define _CONV_ACC_H_

#include <iostream>
#include <fstream>
#include "activation_functions.h"

#if _C_DEBUG_MODE_
#include <algorithm>
#endif

using namespace std;

template <typename T, typename W, typename G, int Tm, int Tn, int Tr, int Tc, int S_max, int K_max>
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
    void w_buf_reset(int K, W buf[][Tm][K_max][K_max]){
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
            buf[i] = *(layer_bias + bias_offset + i + m);
		}
	}
    // Load input data
    void in_buf_load(T buf[][(Tr-1)*S_max + K_max][(Tc-1)*S_max + K_max],T *in_data_1, int in_offset, int n, int r, int c, int S, int K, int P, int R_IN, int C_IN, int N) {
       for (int j = r * S - P; j < (r + Tr - 1) * S + K - P; j++) {
           for (int k = c * S - P; k < (c + Tc - 1) * S + K - P; k++) {
#pragma HLS PIPELINE
        		for (int i = 0; i < Tn; i+=1){
                   	if ((n + Tn > N && i + 0 >= N - n ) || j < 0 || j >= R_IN || k < 0 || k >= C_IN) {
                       	buf[i + 0][j - r * S + P][k - c * S + P] = T(0);
                   	} else {
                       	buf[i + 0][j - r * S + P][k - c * S + P] = *(in_data_1 + in_offset + (i + n)/1 * R_IN * C_IN + j * C_IN + k);
               		}
				}
			}
		}
	}


    // Load weights to weight buffer
   void w_buf_load(W buf[][Tm][K_max][K_max], W *layer_weights, int weight_offset, int n, int m, int K, int N, int M){
       for(int k1 = 0; k1 < K; k1++){
           for(int k2 = 0; k2 < K; k2++){
#pragma HLS PIPELINE
        		for(int j = 0; j < Tn; j++){
            		if(N < n+Tn && j == N-n){
                		break;
            		}
            		for(int i = 0; i < Tm && i < M-m; i++){
                		if(M < m+Tm && i == M-m){
                    		break;
                		}
                        buf[j][i][k1][k2] = *(layer_weights + weight_offset + (i+m)*N*K*K + (j+n)*K*K + k1*K + k2);
                   }
				}
			}
		}
	}
    // Convolution computation kernel
    void conv_engine(T in_buf[][(Tr-1)*S_max + K_max][(Tc-1)*S_max + K_max], W w_buf[][Tm][K_max][K_max], W b_buf[], G out_buf[][Tr][Tc], int S, int n, int r, int c, int K, int R_OUT, int C_OUT){
        for(int i=0; i<K; i++){
            for(int j=0; j<K; j++){
                for(int tr=0; tr<Tr; tr++){
                    for(int tc=0; tc<Tc; tc++){
#pragma HLS PIPELINE
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
    void output_res(G out_buf[][Tr][Tc],G *out_data_1, int out_offset, int n, int m, int r, int c, int N, int M, int R_OUT, int C_OUT, bool act){
        if (n >= N - Tn) {
            for (int j = r; j < r + Tr && j < R_OUT; j++) {
                for (int k = c; k < c + Tc && k < C_OUT; k++) {
#pragma HLS PIPELINE
                    for (int i = 0; i < Tm && i < M-m; i += 1) {
                        if (act) {
                        	if (i + 0 < M-m)
                            	*(out_data_1 + out_offset + ((i+m)/1) * R_OUT * C_OUT + j * C_OUT + k) = relu(out_buf[i + 0][j - r][k - c]);
                        }
                        else {
                        	if (i + 0 < M-m)
                            	*(out_data_1 + out_offset + ((i+m)/1) * R_OUT * C_OUT + j * C_OUT + k) = out_buf[i + 0][j - r][k - c];
                        }
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
            int R_IN, // input Row
            int C_IN, // input column
            int R_OUT, // output Row
            int C_OUT,// output column
            int S, // stride size
            int P, // padding size
            bool act, // activation function bit (1-- with act, 0--without act)
            W *layer_weights, //w[M][N][K][K]
            W *layer_bias, // b[M]
            int weight_offset,
            int bias_offset,
            int in_offset,
            int out_offset,
            T *in_data_1, // in_data[N][(R-1)*S + K][(C-1)*S + K] --> [N][(R-1)*S + K - 2*P][(C-1)*S + K - 2*P]
            G *out_data_1){ // out[M][R][C]

        /***************local data buffer******************************/
        T in_buf_1[Tn][(Tr-1)*S_max + K_max][(Tc-1)*S_max + K_max];
        W w_buf_1[Tn][Tm][K_max][K_max];
        W b_buf_1[Tm];
        G out_buf_1[Tm][Tr][Tc];

#if _HLS_MODE_
#pragma HLS ARRAY_PARTITION variable=in_buf_1 complete dim=1
#pragma HLS ARRAY_PARTITION variable=w_buf_1 complete dim=1
#pragma HLS ARRAY_PARTITION variable=w_buf_1 complete dim=2
#pragma HLS ARRAY_PARTITION variable=b_buf_1 complete dim=1
#pragma HLS ARRAY_PARTITION variable=out_buf_1 complete dim=1
#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
            cout << "Starting conv_acc_innerdf layer ...." << endl;
            //buffer local data initiallization: must do it in C++ debug!
            out_buf_reset(out_buf_1);
            b_buf_reset(b_buf_1);
            w_buf_reset(K, w_buf_1);
#endif
#endif
        for(int r = 0; r < R_OUT; r += Tr){
            for(int c = 0; c < C_OUT; c += Tc){
                for(int m = 0; m < M; m += Tm){
                    for(int n = 0; n < N; n += Tn){
#if _HLS_MODE_
#pragma HLS DATAFLOW
#endif
   //--------------------------Load input B W D in ping-pong manner-------------------------//
                        //load input bias
                        b_buf_load(b_buf_1, layer_bias, bias_offset, m);
                        // load input data
                        in_buf_load(in_buf_1, in_data_1, in_offset, n, r, c, S, K, P, R_IN, C_IN, N);
                        // load input weights
                        w_buf_load(w_buf_1, layer_weights, weight_offset, n, m, K, N, M);
  //------------------------------compute buffered data -----------------------------------//
                        conv_engine(in_buf_1, w_buf_1, b_buf_1, out_buf_1, S, n, r, c, K, R_OUT, C_OUT);
  //---------------------------transfer output data----------------------------------------//
                        // transfer output data
                        output_res(out_buf_1, out_data_1, out_offset, n, m, r, c, N, M, R_OUT, C_OUT, act);
                    }
                }
            }
        }
#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
            cout << "Finished conv_acc_innerdf layer ...." << endl;
            ofstream conv_out;
            conv_out.open("conv_out_data.txt",ios::app);
            conv_out <<"conv output: "<< endl;
            for (int i = 0; i < M/1; i++) {
                for (int j = 0; j < R_OUT; j++) {
                    for(int k = 0; k < C_OUT; k++){
                        conv_out << *(out_data_1 + out_offset + i*R_OUT*C_OUT + j*C_OUT + k) << " ";
                    }conv_out << endl;
                }conv_out << endl;
            }conv_out.close();
#endif
#endif
    }
};
#endif
