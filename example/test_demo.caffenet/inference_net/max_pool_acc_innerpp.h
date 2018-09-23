#ifndef _MAX_POOL_ACC_1_H_
#define _MAX_POOL_ACC_1_H_

#include <iostream>
#include <fstream>
#include "activation_functions.h"

#if _C_DEBUG_MODE_
#include <algorithm>
#endif

using namespace std;

template <typename pT, typename pW, typename pG, int pTn, int pTr, int pTc, int pS_max, int pK_max>
class max_pool_acc_1 {

private:
	int pool_layer_number;

public:
	max_pool_acc_1() : pool_layer_number(0) {pool_layer_number = 0;};

	////------------------------------C++ debugging functions---------------------------------------////
	// Reset output buffer
	void out_buf_reset(pG buf[][pTr][pTc]){
        for(int i = 0; i < pTn; i++){
            for(int j = 0; j < pTr; j++){
                for(int k = 0; k < pTc; k++){
                    buf[i][j][k] = pG(0);
				}
			}
		}
	}
    ////-----------------------------Accelerator Functions---------------------------------------////
    // Load input data
    void in_buf_load(pT buf[][(pTr-1)*pS_max + pK_max][(pTc-1)*pS_max + pK_max],
                     pT *i_data_0,
                     pT *i_data_1,
                     pT *i_data_2,
                     pT *i_data_3,
                     pT *i_data_4,
                     pT *i_data_5,
                     pT *i_data_6,
                     pT *i_data_7,
                     int n, int r, int c, int S, int K, int P,
                     int R, int C, int N, int R_IN, int C_IN, int TR, int TC) {
        for (int j = r * S - P; j < r * S + TR - P; j++) {
            for (int k = c * S - P; k < c * S + TC - P; k++) {
        		 for (int i = 0; i < pTn; i+=8){
#pragma HLS PIPELINE
                    if ((n + pTn > N && (i+0)%8 + n + 0 >= N ) || j < 0 || j >= (R_IN - 2 * P) || k < 0 || k >= (C_IN - 2 * P)) {
                        buf[i][j - r * S + P][k - c * S + P] = pT(0);
                    }else {
                        buf[i][j - r * S + P][k - c * S + P] = *(i_data_0 + (i+n)/8 * (R_IN - 2 * P) * (C_IN - 2 * P) + j * (C_IN - 2 * P) + k);
                    }
                     if ((n + pTn > N && (i+1)%8 + n + 0 >= N ) || j < 0 || j >= (R_IN - 2 * P) || k < 0 || k >= (C_IN - 2 * P)) {
                         buf[(i + 1)][j - r * S + P][k - c * S + P] = pT(0);
                     }else {
                         buf[(i + 1)][j - r * S + P][k - c * S + P] = *(i_data_1 + (i+n)/8 * (R_IN - 2 * P) * (C_IN - 2 * P) + j * (C_IN - 2 * P) + k);
                     }
                     if ((n + pTn > N && (i+2)%8 + n + 0 >= N ) || j < 0 || j >= (R_IN - 2 * P) || k < 0 || k >= (C_IN - 2 * P)) {
                         buf[(i + 2)][j - r * S + P][k - c * S + P] = pT(0);
                     }else {
                         buf[(i + 2)][j - r * S + P][k - c * S + P] = *(i_data_2 + (i+n)/8 * (R_IN - 2 * P) * (C_IN - 2 * P) + j * (C_IN - 2 * P) + k);
                     }
                     if ((n + pTn > N && (i+3)%8 + n + 0 >= N ) || j < 0 || j >= (R_IN - 2 * P) || k < 0 || k >= (C_IN - 2 * P)) {
                         buf[(i + 3)][j - r * S + P][k - c * S + P] = pT(0);
                     }else {
                         buf[(i + 3)][j - r * S + P][k - c * S + P] = *(i_data_3 + (i+n)/8 * (R_IN - 2 * P) * (C_IN - 2 * P) + j * (C_IN - 2 * P) + k);
                     }
                     if ((n + pTn > N && (i+4)%8 + n + 0 >= N ) || j < 0 || j >= (R_IN - 2 * P) || k < 0 || k >= (C_IN - 2 * P)) {
                         buf[(i + 4)][j - r * S + P][k - c * S + P] = pT(0);
                     }else {
                         buf[(i + 4)][j - r * S + P][k - c * S + P] = *(i_data_4 + (i+n)/8 * (R_IN - 2 * P) * (C_IN - 2 * P) + j * (C_IN - 2 * P) + k);
                     }
                     if ((n + pTn > N && (i+5)%8 + n + 0 >= N ) || j < 0 || j >= (R_IN - 2 * P) || k < 0 || k >= (C_IN - 2 * P)) {
                         buf[(i + 5)][j - r * S + P][k - c * S + P] = pT(0);
                     }else {
                         buf[(i + 5)][j - r * S + P][k - c * S + P] = *(i_data_5 + (i+n)/8 * (R_IN - 2 * P) * (C_IN - 2 * P) + j * (C_IN - 2 * P) + k);
                     }
                     if ((n + pTn > N && (i+6)%8 + n + 0 >= N ) || j < 0 || j >= (R_IN - 2 * P) || k < 0 || k >= (C_IN - 2 * P)) {
                         buf[(i + 6)][j - r * S + P][k - c * S + P] = pT(0);
                     }else {
                         buf[(i + 6)][j - r * S + P][k - c * S + P] = *(i_data_6 + (i+n)/8 * (R_IN - 2 * P) * (C_IN - 2 * P) + j * (C_IN - 2 * P) + k);
                     }
                     if ((n + pTn > N && (i+7)%8 + n + 0 >= N ) || j < 0 || j >= (R_IN - 2 * P) || k < 0 || k >= (C_IN - 2 * P)) {
                         buf[(i + 7)][j - r * S + P][k - c * S + P] = pT(0);
                     }else {
                         buf[(i + 7)][j - r * S + P][k - c * S + P] = *(i_data_7 + (i+n)/8 * (R_IN - 2 * P) * (C_IN - 2 * P) + j * (C_IN - 2 * P) + k);
                     }
				}
			}
		}
	}
    // Max pooling computation kernel
    void pool_engine(pT in_buf[][(pTr-1)*pS_max + pK_max][(pTc-1)*pS_max + pK_max],
                     pG out_buf[][pTr][pTc],
                     int S, int n, int r, int c, int K, int R, int C, int TR, int TC) {
        if (n >= 0) {
            for (int i = 0; i < K; i++) {
                for (int j = 0; j < K; j++) {
                    for (int tr = 0; tr < pTr && tr + r < R && (S * tr + i) < TR; tr++) {
                        for (int tc = 0; tc < pTc && tc + c < C && (S * tc + j) < TC; tc++) {
#pragma HLS PIPELINE
                            for (int tn = 0; tn < pTn; tn++) {
#pragma HLS UNROLL
                                out_buf[tn][tr][tc] = (i == 0 && j == 0) ? in_buf[tn][S * tr][S * tc]
                                                                         : ((out_buf[tn][tr][tc]
                                                                         > in_buf[tn][S * tr + i][S * tc + j])
                                                                         ? out_buf[tn][tr][tc]
                                                                         : in_buf[tn][S * tr + i][S * tc + j]);
                            }
                        }
                    }
                }
            }
        }
    }

    // Ouput out_buf data to output interface
    void output_res(pG out_buf[][pTr][pTc],
                    pG *o_data_0,
                    pG *o_data_1,
                    pG *o_data_2,
                    pG *o_data_3,
                    pG *o_data_4,
                    pG *o_data_5,
                    pG *o_data_6,
                    pG *o_data_7,
                    int n, int r, int c, int N, int R, int C, bool act) {
        if (n >= 0) {
            for (int j = r; j < r + pTr && j < R; j++) {
                for (int k = c; k < c + pTc && k < C; k++) {
                    for (int i = 0; i < pTn && i < N - n; i += 8) {
#pragma HLS PIPELINE
//                        if (act) {
                            if ((i + 0) < N - n)
                                *(o_data_0 + ((i + n) / 8) * R * C + j * C + k) = relu(out_buf[(i+0)][j - r][k - c]);
                            if (i + 1 < N - n)
                                *(o_data_1 + ((i + n) / 8) * R * C + j * C + k) = relu(out_buf[(i+1)][j - r][k - c]);
                            if (i + 2 < N - n)
                                *(o_data_2 + ((i + n) / 8) * R * C + j * C + k) = relu(out_buf[(i+2)][j - r][k - c]);
                            if (i + 3 < N - n)
                                *(o_data_3 + ((i + n) / 8) * R * C + j * C + k) = relu(out_buf[(i+3)][j - r][k - c]);
                            if (i + 4 < N - n)
                                *(o_data_4 + ((i + n) / 8) * R * C + j * C + k) = relu(out_buf[(i+4)][j - r][k - c]);
                            if (i + 5 < N - n)
                                *(o_data_5 + ((i + n) / 8) * R * C + j * C + k) = relu(out_buf[(i+5)][j - r][k - c]);
                            if (i + 6 < N - n)
                                *(o_data_6 + ((i + n) / 8) * R * C + j * C + k) = relu(out_buf[(i+6)][j - r][k - c]);
                            if (i + 7 < N - n)
                                *(o_data_7 + ((i + n) / 8) * R * C + j * C + k) = relu(out_buf[(i+7)][j - r][k - c]);
//                        } else {
//                            if (i + 0 < N)
//                                *(out_data_1 + (i / 1) * R * C + j * C + k) = out_buf[i + 0 - n][j - r][k - c];
//                        }
                    }
                }
            }
        }
    }

    void output(pG* out_data_1,
                pG out_buf_0[][pTr][pTc],
                int r, int c, int N, int R, int C, bool act) {
        for (int m = 0; m < N + pTn; m += pTn) {
            output_res(out_buf_0, out_data_1, m-pTn, r, c, N, R, C, act);
        }
    }
///////////////////////------------------conv accelerator----------------//////////////////////////
    void max_pool_layer_dbuf(
            int R_IN,// input Row
            int C_IN,// input column
            int N, //input feature number
            int K, //input kernel size
            int R, // output Row
            int C, // output column
            int S, // stride size
            int P, // padding size
            bool act, // activation function bit (1-- with act, 0--without act)
            pT *in_data_1, // in_data[N][(R-1)*S + K][(C-1)*S + K] --> [N][(R-1)*S + K - 2*P][(C-1)*S + K - 2*P]
            pG *out_data_1){ // out[M][R][C]

        /***************local data buffer******************************/
        int TR=0;
        int TC=0;
        pT in_buf_1[pTn][(pTr-1)*pS_max + pK_max][(pTc-1)*pS_max + pK_max];
        pT in_buf_0[pTn][(pTr-1)*pS_max + pK_max][(pTc-1)*pS_max + pK_max];
        pG out_buf_1[pTn][pTr][pTc];
        pG out_buf_0[pTn][pTr][pTc];

        /***************Ptr and buffer initialization******************************/
        bool in_buf_0_empty = 1;
        bool in_buf_1_empty = 1;
        bool out_buf_0_empty = 1;
        bool out_buf_1_empty = 1;
        int loadbufPtr = 0;
        int combufPtr = 0;
        int resbufPtr = 0;

#if _HLS_MODE_
#pragma HLS ARRAY_PARTITION variable=in_buf_1 complete dim=1
#pragma HLS ARRAY_PARTITION variable=in_buf_0 complete dim=1
#pragma HLS ARRAY_PARTITION variable=out_buf_1 complete dim=1
#pragma HLS ARRAY_PARTITION variable=out_buf_0 complete dim=1
#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
            cout << "Starting max_pool_acc_innerpp layer ...." << endl;
            //buffer local data initiallization: must do it in C++ debug!
            out_buf_reset(out_buf_1);
            out_buf_reset(out_buf_0);
#endif
#endif
        for(int r = 0; r < R; r += pTr){
            for(int c = 0; c < C; c += pTc){
                TR=((r * S + (pTr - 1) * S + K)>R_IN?(R_IN - r * S):((pTr - 1) * S + K));
                TC=((c * S + (pTc - 1) * S + K)>C_IN?(C_IN - c * S):((pTc - 1) * S + K));
                    for(int n = 0; n < N; n += 2*pTn){
   //--------------------------Load input B W D in ping-pong manner-------------------------//
						while (in_buf_0_empty | in_buf_1_empty) {
							if (loadbufPtr == 1) {
                    			cout << "loading input buffer 1...." << endl;
                        		// load input data
                        		in_buf_load(in_buf_1, in_data_1, n+pTn, r, c, S, K, P, R, C, N, R_IN, C_IN, TR, TC);
                        		in_buf_1_empty = 0;
                        		cout << "buffer 1 full" << endl;
                        		loadbufPtr = 0;
                        	} else {
                    			cout << "loading input buffer 0...." << endl;
                        		// load input data
                        		in_buf_load(in_buf_0, in_data_1, n, r, c, S, K, P, R, C, N, R_IN, C_IN, TR, TC);
                        		in_buf_0_empty = 0;
                        		cout << "buffer 0 full" << endl;
                        		loadbufPtr = 1;
							}
                       }
                       loadbufPtr = 0;
  //------------------------------compute buffered data -----------------------------------//
                    	while (!in_buf_0_empty | !in_buf_1_empty) {
                    		if (combufPtr == 1) {
                    			cout << "computing input buffer 1...." << endl;
                        		pool_engine(in_buf_1, out_buf_1, S, n+pTn, r, c, K, R, C, TR, TC);
                    			out_buf_1_empty = 0;
                    			in_buf_1_empty = 1;
                    			combufPtr = 0;
                    			cout << "buffer 1 computed" << endl;
                    		} else {
                    			cout << "computing input buffer 0...." << endl;
                        		pool_engine(in_buf_0, out_buf_0, S, n, r, c, K, R, C, TR, TC);
                    			out_buf_0_empty = 0;
                    			in_buf_0_empty = 1;
                    			combufPtr = 1;
                    			cout << "buffer 0 computed" << endl;
							}
                       }
                       combufPtr = 0;
  //---------------------------transfer output data----------------------------------------//
                    	while (!out_buf_0_empty | !out_buf_1_empty) {
                    		if (resbufPtr == 1) {
                    			cout << "output buffer 1...." << endl;
                    			// transfer output data
                    			output_res(out_buf_1, out_data_1, n+pTn, r, c, N, R, C, act);
                    			out_buf_1_empty = 1;
                    			resbufPtr = 0;
                    			cout << "buffer 1 res" << endl;
                    		} else {
                    			cout << "output buffer 0...." << endl;
                    			// transfer output data
                    			output_res(out_buf_0, out_data_1, n, r, c, N, R, C, act);
                    			out_buf_0_empty = 1;
                    			resbufPtr = 1;
								cout << "buffer 0 res" << endl;
							}
						}
						resbufPtr = 0;
					}
				}
			}
#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
            cout << "Finished max_pool_acc_innerpp layer ...." << endl;
            ofstream max_pool_out;
            max_pool_out.open("max_pool_out_data.txt",ios::app);
            max_pool_out <<"max_pool output: "<< endl;
            for (int i = 0; i < N/1; i++) {
                for (int j = 0; j < R; j++) {
                    for(int k = 0; k < C; k++){
                        max_pool_out << *(out_data_1 + i*R*C + j*C + k) << " ";
                    }max_pool_out << endl;
                }max_pool_out << endl;
            }max_pool_out.close();
#endif
#endif
    }

	void max_pool_layer_mbuf(
            int R_IN,// input Row
            int C_IN,// input column
            int N, //input feature number
            int K, //input kernel size
            int R, // output Row
            int C, // output column
            int S, // stride size
            int P, // padding size
            bool act, // activation function bit (1-- with act, 0--without act)
            pT *i_data_0, // in_data[N][(R-1)*S + K][(C-1)*S + K] --> [N][(R-1)*S + K - 2*P][(C-1)*S + K - 2*P]
            pT *i_data_1,
            pT *i_data_2,
            pT *i_data_3,
            pT *i_data_4,
            pT *i_data_5,
            pT *i_data_6,
            pT *i_data_7,
            pG *o_data_0,
            pG *o_data_1,
            pG *o_data_2,
            pG *o_data_3,
            pG *o_data_4,
            pG *o_data_5,
            pG *o_data_6,
            pG *o_data_7){ // out[M][R][C]

        /***************local data buffer******************************/
        int TR=0;
        int TC=0;
        pT in_buf_1[pTn][(pTr-1)*pS_max + pK_max][(pTc-1)*pS_max + pK_max];
        pT in_buf_0[pTn][(pTr-1)*pS_max + pK_max][(pTc-1)*pS_max + pK_max];
        pG out_buf_1[pTn][pTr][pTc];
        pG out_buf_0[pTn][pTr][pTc];

        bool i_buf_ptr = 0;
        bool o_buf_ptr = 0;

#if _HLS_MODE_

#pragma HLS ARRAY_PARTITION variable=in_buf_1 complete dim=1
#pragma HLS ARRAY_PARTITION variable=in_buf_0 complete dim=1
#pragma HLS ARRAY_PARTITION variable=out_buf_1 complete dim=1
#pragma HLS ARRAY_PARTITION variable=out_buf_0 complete dim=1

#endif

#if _C_DEBUG_MODE_
        #if _KERNEL_DEBUG_
            cout << "Starting max_pool_acc_mbuf ...." << endl;
            //buffer local data initiallization: must do it in C++ debug!
            out_buf_reset(out_buf_1);
            out_buf_reset(out_buf_0);
#endif
#endif
        for(int r = 0; r < R; r += pTr) {
            for (int c = 0; c < C; c += pTc) {
                TR = ((r * S + (pTr - 1) * S + K) > R_IN ? (R_IN - r * S) : ((pTr - 1) * S + K));
                TC = ((c * S + (pTc - 1) * S + K) > C_IN ? (C_IN - c * S) : ((pTc - 1) * S + K));
                for (int n = 0; n < N ; n += pTn) {
//                    if (i_buf_ptr == 0) {
//--------------------------Load input B W D in ping-pong manner-------------------------//
                        in_buf_load(in_buf_0,
                                    i_data_0,
                                    i_data_1,
                                    i_data_2,
                                    i_data_3,
                                    i_data_4,
                                    i_data_5,
                                    i_data_6,
                                    i_data_7,
                                    n, r, c, S, K, P, R, C, N, R_IN, C_IN, TR, TC);
                        pool_engine(in_buf_0, out_buf_0, S, n, r, c, K, R, C, TR, TC);
//                    } else {
//                        in_buf_load(in_buf_1,
//                                    i_data_0,
//                                    i_data_1,
//                                    i_data_2,
//                                    i_data_3,
//                                    i_data_4,
//                                    i_data_5,
//                                    i_data_6,
//                                    i_data_7,
//                                    n, r, c, S, K, P, R, C, N, R_IN, C_IN, TR, TC);
//                        pool_engine(in_buf_0, out_buf_0, S, n - pTn, r, c, K, R, C, TR, TC);
//                    }
//                    i_buf_ptr = !i_buf_ptr;
                    output_res(out_buf_0,
                               o_data_0,
                               o_data_1,
                               o_data_2,
                               o_data_3,
                               o_data_4,
                               o_data_5,
                               o_data_6,
                               o_data_7,
                               n, r, c, N, R, C, act);
                }
            }
        }

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
            cout << "Finished max_pool_acc_mbuf ...." << endl;
            ofstream max_pool_out;
            max_pool_out.open("max_pool_out_data.txt",ios::app);
            max_pool_out <<"max_pool output: "<< endl;
/*
            for (int i = 0; i < N; i++){
                for (int j = 0; j < R; j++){
                    for (int k = 0; k < C; k++){
                        max_pool_out << out_buf_0[i][j][k] << " ";
                    }
                    max_pool_out << endl;
                }
                max_pool_out << endl;
            }
            max_pool_out << endl;
*/
            for (int i = 0; i < N; i += 8) {
                for (int j = 0; j < R; j++) {
                    for(int k = 0; k < C; k++){
//                        max_pool_out << out_buf_0[i][j][k] << " ";
                        max_pool_out << *(o_data_0 + (i/8)*R*C + j*C + k) << " ";
                    }
                    max_pool_out << endl;
                }
                max_pool_out << endl;
                for (int j = 0; j < R; j++) {
                    for(int k = 0; k < C; k++){
//                        max_pool_out << out_buf_0[i][j][k] << " ";
                        max_pool_out << *(o_data_1 + (i/8)*R*C + j*C + k) << " ";
                    }
                    max_pool_out << endl;
                }
                max_pool_out << endl;
                for (int j = 0; j < R; j++) {
                    for(int k = 0; k < C; k++){
//                        max_pool_out << out_buf_0[i][j][k] << " ";
                        max_pool_out << *(o_data_2 + (i/8)*R*C + j*C + k) << " ";
                    }
                    max_pool_out << endl;
                }
                max_pool_out << endl;
                for (int j = 0; j < R; j++) {
                    for(int k = 0; k < C; k++){
//                        max_pool_out << out_buf_0[i][j][k] << " ";
                        max_pool_out << *(o_data_3 + (i/8)*R*C + j*C + k) << " ";
                    }
                    max_pool_out << endl;
                }
                max_pool_out << endl;
                for (int j = 0; j < R; j++) {
                    for(int k = 0; k < C; k++){
//                        max_pool_out << out_buf_0[i][j][k] << " ";
                        max_pool_out << *(o_data_4 + (i/8)*R*C + j*C + k) << " ";
                    }
                    max_pool_out << endl;
                }
                max_pool_out << endl;
                for (int j = 0; j < R; j++) {
                    for(int k = 0; k < C; k++){
//                        max_pool_out << out_buf_0[i][j][k] << " ";
                        max_pool_out << *(o_data_5 + (i/8)*R*C + j*C + k) << " ";
                    }
                    max_pool_out << endl;
                }
                max_pool_out << endl;
                for (int j = 0; j < R; j++) {
                    for(int k = 0; k < C; k++){
//                        max_pool_out << out_buf_0[i][j][k] << " ";
                        max_pool_out << *(o_data_6 + (i/8)*R*C + j*C + k) << " ";
                    }
                    max_pool_out << endl;
                }
                max_pool_out << endl;
                for (int j = 0; j < R; j++) {
                    for(int k = 0; k < C; k++){
//                        max_pool_out << out_buf_0[i][j][k] << " ";
                        max_pool_out << *(o_data_7 + (i/8)*R*C + j*C + k) << " ";
                    }
                    max_pool_out << endl;
                }
                max_pool_out << endl;
            }
            max_pool_out.close();
#endif
#endif
    }

};
#endif
