#ifndef _CONV_ACC_H_
#define _CONV_ACC_H_

#include <iostream>
#include <fstream>
#include "hls_stream.h"
#include "activation_functions.h"
using namespace std;

template<typename Itf, typename Tparam, typename T, typename W, typename G, int Tm, int Tn, int Tr, int Tc, int S_max, int K_max, int IBUF_t, int WBUF_t, int OBUF_t>
class conv_acc {

private:
    int conv_layer_number;

public:
    conv_acc() : conv_layer_number(0) { conv_layer_number = 0; };

    ////------------------------------C++ debugging functions---------------------------------------////
    // Reset output buffer
    void out_buf_reset(G buf[][Tr][Tc]) {
        for (int i = 0; i < Tm; i++) {
            for (int j = 0; j < Tr; j++) {
                for (int k = 0; k < Tc; k++) {
                    buf[i][j][k] = G(0);
                }
            }
        }
    }
    // Reset weight buffer
    void w_buf_reset(int K, W buf[][Tm][K_max][K_max]) {
        for (int i = 0; i < Tn; i++) {
            for (int j = 0; j < Tm; j++) {
                for (int k = 0; k < K; k++) {
                    for (int l = 0; l < K; l++) {
                        buf[i][j][k][l] = W(0);
                    }
                }
            }
        }
    }
    // Reset bias buffer
    void b_buf_reset(W buf[]) {
        for (int i = 0; i < Tm; i++) {
            buf[i] = W(0);
        }
    }

    ////-----------------------------Accelerator Functions---------------------------------------////
    // Load bias data
    void b_buf_load(W buf[], ap_fixed<32,26> *layer_bias, int bias_offset, int m) {
        for (int i = 0; i < Tm; i++) {
            buf[i].range(15,0) = (*(layer_bias + bias_offset + i + m)).range(15,0);
//            cout << "Read bias location: " << bias_offset + i + m << "  Read bias data: " << buf[i] << endl;
        }
    }
    // Tn << 32 && N << 32
    void in_buf_load_512(
            T buf[][(Tr - 1) * S_max + K_max][(Tc - 1) * S_max + K_max],
            Itf* i_data,
            int in_offset, int n, int r, int c, int S, int K, int P, int R_IN, int C_IN, int N ) {
        Itf data_tmp = 0;
        // valid data portion
        for (int j = r * S - P; j < (r + Tr - 1) * S + K - P ; j++) {//
            for (int k = c * S - P; k < (c + Tc -1) * S + K - P; k++) {
#pragma HLS PIPELINE
                for (int i = 0; i < Tn; i += Tn) {
#pragma HLS UNROLL
                    if ((i + n >= N) || j < 0 || j >= R_IN || k < 0 || k >= C_IN) {
                        for (int wr = 0; wr < Tn; wr++) {

#pragma HLS UNROLL
                            buf[wr][j - r * S + P][k - c * S + P] = T(0);
                        }
                    }
                    else {
                        data_tmp = *(i_data + in_offset + (i + n)/32 * R_IN * C_IN + j * R_IN + k);
                        for (int wr = 0; wr < Tn; wr++) {
#pragma HLS UNROLL

                            buf[wr][j - r * S + P][k - c * S + P].range(15,0) = data_tmp.range(((n + wr)%32 + 1) * 16 - 1, ((n + wr)%32) * 16);
                        }
                    }
                }
            }
        }
    }

    void w_buf_load_512(W buf[][Tm][K_max][K_max],
                        Itf *layer_weights,
                        int weight_offset,
                        int n, int m, int K, int N, int M)
    {
        Itf w_tmp = 0;
        for (int k1 = 0; k1 < K; k1++) {
            for (int k2 = 0; k2 < K; k2++) {
                for (int i = 0; i < Tm; i++) {
                    for (int j = 0; j < Tn; j += Tn) {
#pragma HLS PIPELINE
                    	w_tmp = *(layer_weights + weight_offset + ((j + n)/32)* M * K * K + (i + m) * K * K + k1*K + k2);
//                        cout << "Read weight location: " << weight_offset + ((j + n)/32)* M * K * K + (i + m) * K * K + k1*K + k2
//                             << "  Read weight addr: " << (n+wr)%32 << endl;
                        for (int wr = 0; wr < Tn; wr++) {
#pragma HLS UNROLL
                            buf[wr][i][k1][k2].range(15,0) = w_tmp.range(((n + wr)%32 + 1) * 16 - 1, ((n+wr)%32) * 16);
                        }
                    }
                }
            }
        }
    }

// Convolution computation kernel Tm, Tn based
    void conv_engine(T in_buf[][(Tr - 1) * S_max + K_max][(Tc - 1) * S_max + K_max], W w_buf[][Tm][K_max][K_max],
                     W b_buf[], G out_buf[][Tr][Tc], int S, int n, int N, int r, int c, int K, int R_OUT, int C_OUT,
                     int w_offset, int i_offset) {
        if (n >= 0 && n - Tn < N) {
            for (int i = 0; i < K; i++) {
                for (int j = 0; j < K; j++) {
                    for (int tr = 0; tr < Tr && tr < R_OUT - r; tr++) {
                        for (int tc = 0; tc < Tc; tc++) {
#pragma HLS PIPELINE
                            for (int tm = 0; tm < Tm; tm++) {
#pragma HLS UNROLL
                                for (int tn = 0; tn < Tn; tn++) {
#pragma HLS UNROLL
                                    if (i == 0 && j == 0 && tn == 0 && n == 0)
                                        out_buf[tm][tr][tc] = b_buf[tm] + w_buf[tn][tm][i + w_offset][j] *
                                                                          in_buf[tn][S * (tr) + i + i_offset][S * (tc) +
                                                                                                              j];
                                    else
                                        out_buf[tm][tr][tc] = out_buf[tm][tr][tc] + w_buf[tn][tm][i + w_offset][j] *
                                                                                    in_buf[tn][S * (tr) + i + i_offset][
                                                                                            S * (tc) + j];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Ouput out_buf data to output interface
    void output_res_512(ap_fixed<16,10> out_buf[][Tr][Tc],
    					ap_int<512>*	out_data,
                        int 			out_offset,
						int n, int m, int r, int c, int N, int M,
                        int R_OUT, int C_OUT, bool act)
    {
        ap_int<512>     out_tmp = 0;
        ap_fixed<16,10> tmp = 0;
        ap_fixed<16,10> tmp_outbuf = 0;
        if (n >= N - Tn)
        {
            for (int j = r; (j < r + Tr) && (j < R_OUT); j++)
            {
                for (int k = c; (k < c + Tc) && (k < C_OUT); k++)
                {
#pragma HLS PIPELINE
                  //  for (int i = 0; i < Tm; i += 32)
                    //{
#pragma HLS UNROLL
                        for (int wr = 0; wr < (Tm<32?Tm:32); wr++)
                        {
#pragma HLS UNROLL
                            if(m + wr < M)
                            {
                                tmp_outbuf = RELU(out_buf[wr][j - r][k - c]);
                                tmp.range(15, 0) = tmp_outbuf.range(15, 0);
                            }
                            else
                                tmp.range(15,0) = 0;


                            out_tmp.range(16 * (wr + 1) - 1, 16 * wr) = tmp.range(15,0);
                        }

                        *(out_data + out_offset + (m / 32) * R_OUT * C_OUT + j * C_OUT + k) = out_tmp;
                    //}
                }
            }
        }
    }

///////////////////////------------------conv accelerator----------------//////////////////////////
#if _LAYER_MODE_ // layer function with cast port
    void conv_layer_acc_mbuf(
            int N,            //input feature number
            int K,            //input kernel size
            int M,            // output feature number
            int R_IN,         // input Row
            int C_IN,         // input column
            int R_OUT,        // output Row
            int C_OUT,        // output column
            int S,            // stride size
            int P,            // padding size
            bool act,         // activation function bit (1-- with act, 0--without act)
            int weight_offset,
            int bias_offset,
            int in_offset,
            int out_offset,
			ap_fixed<32,26> *layer_bias,
            Itf *i_weight,
            Itf *i_data,
            Itf *out_data ) { // out[M][R][C]

        /***************local data buffer groups******************************/

        T in_buf_0[Tn][(Tr - 1) * S_max + K_max][(Tc - 1) * S_max + K_max];
        W w_buf_0[Tn][Tm][K_max][K_max];
        W b_buf_0[Tm];
        G out_buf_0[Tm][Tr][Tc];

#pragma HLS ARRAY_PARTITION variable = in_buf_0 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = w_buf_0 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = w_buf_0 complete dim = 2
#pragma HLS ARRAY_PARTITION variable = b_buf_0 complete
#pragma HLS ARRAY_PARTITION variable = out_buf_0 complete dim = 1




        //--------------------------Initial data load ---------------------------------------------//
        for (int r = 0; r < R_OUT; r += Tr)
        {
            for (int c = 0; c < C_OUT; c += Tc)
            {
                for (int m = 0; m < M; m += Tm)
                {
                    for (int n = 0; n < N; n += Tn)
                    {


                        //--------------------------Load input B W D in ping-pong manner-------------------------//
                        b_buf_load(b_buf_0, layer_bias, bias_offset, m);

                        w_buf_load_512(w_buf_0, i_weight, weight_offset, n, m, K, N, M);
//
                        in_buf_load_512(in_buf_0, i_data, in_offset, n, r, c, S, K, P, R_IN, C_IN, N);
//
//                        //------------------------------compute buffered data -----------------------------------//
                        conv_engine(in_buf_0, w_buf_0, b_buf_0, out_buf_0, S, n, N, r, c, K, R_OUT, C_OUT, 0, 0);
//
//                    //---------------------------transfer output data----------------------------------------//
                        output_res_512(out_buf_0,out_data,out_offset, n, m, r, c, N, M, R_OUT, C_OUT, act);

                }
            }
        }
    }


    };

#endif

};

#endif
