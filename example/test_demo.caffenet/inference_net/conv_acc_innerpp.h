#ifndef _CONV_ACC_0_H_
#define _CONV_ACC_0_H_

#include <iostream>
#include <fstream>
#include "hls_stream.h"
#include "activation_functions.h"

#if _C_DEBUG_MODE_
#include <algorithm>
#endif

using namespace std;

template<typename Itf, typename T, typename W, typename G, int Tm, int Tn, int Tr, int Tc, int S_max, int K_max, int IBUF_t, int WBUF_t, int OBUF_t>
class conv_acc_0 {

private:
    int conv_layer_number;

public:
    conv_acc_0() : conv_layer_number(0) { conv_layer_number = 0; };

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
    void b_buf_load(W buf[], Itf *layer_bias, int bias_offset, int m) {
        Itf b_tmp = 0;
        for (int i = 0; i < Tm; i++) {
            b_tmp = *(layer_bias + bias_offset + i + m);
            buf[i] = b_tmp.range(15, 0);
//            buf[i+1] = b_tmp.range(31,16);
        }
    }

    void in_buf_load_8ch(
            T buf[][(Tr - 1) * S_max + K_max][(Tc - 1) * S_max + K_max],
            Itf *i_data_0,
            Itf *i_data_1,
            Itf *i_data_2,
            Itf *i_data_3,
            Itf *i_data_4,
            Itf *i_data_5,
            Itf *i_data_6,
            Itf *i_data_7,
            int in_offset, int n, int r, int c, int S, int K, int P, int R_IN, int C_IN, int N) {

        Itf i_tmp_0 = 0;
        Itf i_tmp_1 = 0;
        Itf i_tmp_2 = 0;
        Itf i_tmp_3 = 0;
        Itf i_tmp_4 = 0;
        Itf i_tmp_5 = 0;
        Itf i_tmp_6 = 0;
        Itf i_tmp_7 = 0;
        for (int j = r * S - P; j < (r + Tr - 1) * S + K - P && j < R_IN; j++) {
            for (int k = c * S - P; k < (c + Tc - 1) * S + K - P && j < C_IN; k++) {
#pragma HLS PIPELINE
                for (int i = 0; i < Tn && i < N; i += 8) {

                    if ((n + Tn > N && (i + 0) % 8 >= N - n) || j < 0 || j >= R_IN || k < 0 || k >= C_IN) {
                        buf[i + 0][j - r * S + P][k - c * S + P] = T(0);
//                        buf[i + 0][j - r * S + P][k - c * S + P + 1] = T(0);
                        buf[i + 1][j - r * S + P][k - c * S + P] = T(0);
//                        buf[i + 0][j - r * S + P][k - c * S + P + 1] = T(0);
                        buf[i + 2][j - r * S + P][k - c * S + P] = T(0);
//                        buf[i + 0][j - r * S + P][k - c * S + P + 1] = T(0);
//                            buf[i + 3][j - r * S + P][k - c * S + P] = T(0);
//                            buf[i + 4][j - r * S + P][k - c * S + P] = T(0);
//                            buf[i + 5][j - r * S + P][k - c * S + P] = T(0);
//                            buf[i + 6][j - r * S + P][k - c * S + P] = T(0);
//                            buf[i + 7][j - r * S + P][k - c * S + P] = T(0);
                    } else {
                        i_tmp_0 = *(i_data_0 + in_offset + (i + 0 + n) / 8 * R_IN * C_IN + j * C_IN + k);
                        buf[i + 0][j - r * S + P][k - c * S + P] = i_tmp_0.range(15, 0);
                        i_tmp_1 = *(i_data_1 + in_offset + (i + 1 + n) / 8 * R_IN * C_IN + j * C_IN + k);
                        buf[i + 1][j - r * S + P][k - c * S + P] = i_tmp_1.range(15, 0);
                        i_tmp_2 = *(i_data_2 + in_offset + (i + 2 + n) / 8 * R_IN * C_IN + j * C_IN + k);
                        buf[i + 2][j - r * S + P][k - c * S + P] = i_tmp_2.range(15, 0);
                        /*
                        buf[i + 3][j - r * S + P][k - c * S + P] = *(i_data_3 + in_offset +
                                                                     (i + 3 + n) / 8 * R_IN * C_IN +
                                                                     j * C_IN + k);
                        buf[i + 4][j - r * S + P][k - c * S + P] = *(i_data_4 + in_offset +
                                                                     (i + 4 + n) / 8 * R_IN * C_IN +
                                                                     j * C_IN + k);
                        buf[i + 5][j - r * S + P][k - c * S + P] = *(i_data_5 + in_offset +
                                                                     (i + 5 + n) / 8 * R_IN * C_IN +
                                                                     j * C_IN + k);
                        buf[i + 6][j - r * S + P][k - c * S + P] = *(i_data_6 + in_offset +
                                                                     (i + 6 + n) / 8 * R_IN * C_IN +
                                                                     j * C_IN + k);
                        buf[i + 7][j - r * S + P][k - c * S + P] = *(i_data_7 + in_offset +
                                                                     (i + 7 + n) / 8 * R_IN * C_IN +
                                                                     j * C_IN + k);
                                                                     */
                    }
                }
            }
        }
    }

// Load weights to weight buffer
    void w_buf_load_4ch(W buf[][Tm][K_max][K_max],
                        W *layer_weights_0,
                        W *layer_weights_1,
                        W *layer_weights_2,
                        W *layer_weights_3,
                        int weight_offset, int n, int m, int K, int N, int M) {
        for (int j = 0; j < Tn && j < N - n; j += 4) {
            for (int i = 0; i < Tm && i < M - m; i++) {
                for (int k1 = 0; k1 < K; k1++) {
                    for (int k2 = 0; k2 < K; k2++) {
#pragma HLS PIPELINE
                        buf[j + 0][i][k1][k2] = *(layer_weights_0 + weight_offset + (i + m) * N * K * K +
                                                  (j + 0 + n) * K * K + k1 * K + k2);
                        buf[j + 1][i][k1][k2] = *(layer_weights_1 + weight_offset + (i + m) * N * K * K +
                                                  (j + 1 + n) * K * K + k1 * K + k2);
                        buf[j + 2][i][k1][k2] = *(layer_weights_2 + weight_offset + (i + m) * N * K * K +
                                                  (j + 2 + n) * K * K + k1 * K + k2);
                        buf[j + 3][i][k1][k2] = *(layer_weights_3 + weight_offset + (i + m) * N * K * K +
                                                  (j + 3 + n) * K * K + k1 * K + k2);
                    }
                }
            }
        }

    }

    void w_buf_load_8ch(W buf[][Tm][K_max][K_max],
                        Itf *layer_weights_0,
                        Itf *layer_weights_1,
                        Itf *layer_weights_2,
                        Itf *layer_weights_3,
                        Itf *layer_weights_4,
                        Itf *layer_weights_5,
                        Itf *layer_weights_6,
                        Itf *layer_weights_7,
                        int weight_offset, int n, int m, int K, int N, int M) {
        Itf w_tmp_0 = 0;
        Itf w_tmp_1 = 0;
        Itf w_tmp_2 = 0;
        Itf w_tmp_3 = 0;
        Itf w_tmp_4 = 0;
        Itf w_tmp_5 = 0;
        Itf w_tmp_6 = 0;
        Itf w_tmp_7 = 0;
        for (int j = 0; j < Tn && j < N - n; j += 8) {
            for (int i = 0; i < Tm && i < M - m; i++) {
                for (int k1 = 0; k1 < K; k1++) {
                    for (int k2 = 0; k2 < K; k2++) {
#pragma HLS PIPELINE
                        w_tmp_0 = *(layer_weights_0 + weight_offset + (i + m) * N * K * K +
                                    (j + 0 + n) * K * K + k1 * K + k2);
                        buf[j + 0][i][k1][k2] = w_tmp_0.range(15, 0);
                        w_tmp_1 = *(layer_weights_1 + weight_offset + (i + m) * N * K * K +
                                    (j + 1 + n) * K * K + k1 * K + k2);
                        buf[j + 1][i][k1][k2] = w_tmp_1.range(15, 0);
                        w_tmp_2 = *(layer_weights_2 + weight_offset + (i + m) * N * K * K +
                                    (j + 2 + n) * K * K + k1 * K + k2);
                        buf[j + 2][i][k1][k2] = w_tmp_2.range(15, 0);
                        /*
                        buf[j+3][i][k1][k2] = *(layer_weights_3 + weight_offset + (i + m) * N * K * K +
                                                (j+3+n) * K * K + k1 * K + k2);
                        buf[j+4][i][k1][k2] = *(layer_weights_0 + weight_offset + (i + m) * N * K * K +
                                                (j+4+n) * K * K + k1 * K + k2);
                        buf[j+5][i][k1][k2] = *(layer_weights_1 + weight_offset + (i + m) * N * K * K +
                                                (j+5+n) * K * K + k1 * K + k2);
                        buf[j+6][i][k1][k2] = *(layer_weights_2 + weight_offset + (i + m) * N * K * K +
                                                (j+6+n) * K * K + k1 * K + k2);
                        buf[j+7][i][k1][k2] = *(layer_weights_3 + weight_offset + (i + m) * N * K * K +
                                                (j+7+n) * K * K + k1 * K + k2);
                        */
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

// Convolution computation kernel
    void conv_engine_flex(T in_buf[][(Tr - 1) * S_max + K_max][(Tc - 1) * S_max + K_max], W w_buf[][Tm][K_max][K_max],
                          W b_buf[], G out_buf[][Tr][Tc], int S, int n, int N, int r, int c, int K, int R_OUT,
                          int C_OUT,
                          int w_offset, int i_offset) {

        if (n >= 0 && n - Tn < N) {
            for (int i = 0; i < K; i++) {
                for (int j = 0; j < K; j += 2) {
                    for (int tr = 0; tr < Tr && tr < R_OUT - r; tr++) {
                        for (int tc = 0; tc < Tc; tc++) {
#pragma HLS PIPELINE
                            for (int tm = 0; tm < Tm; tm++) {
#pragma HLS UNROLL
                                for (int tn = 0; tn < Tn; tn++) {
#pragma HLS UNROLL
                                    for (int tj = 0; tj < 2; tj++) {
#pragma HLS UNROLL
                                        if (i == 0 && j == 0 && tn == 0 && n == 0)
                                            out_buf[tm][tr][tc] = b_buf[tm] + w_buf[tn][tm][i + w_offset][j + tj] *
                                                                              in_buf[tn][S * (tr) + i + i_offset][
                                                                                      S * (tc) + j + tj];
                                        else
                                            out_buf[tm][tr][tc] =
                                                    out_buf[tm][tr][tc] + w_buf[tn][tm][i + w_offset][j + tj] *
                                                                          in_buf[tn][S * (tr) + i +
                                                                                     i_offset][S * (tc) +
                                                                                               j + tj];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void conv_iq(T in_buf[][(Tr - 1) * S_max + K_max][(Tc - 1) * S_max + K_max], W w_buf[][Tm][K_max][K_max],
                 W b_buf[], G out_buf[][Tr][Tc],
                 int S, int n, int N, int r, int c, int K, int R_OUT, int C_OUT,
                 int w_offset, int i_offset) {

        if (n >= 0 && n - Tn < N) {
            for (int tr = 0; tr < Tr; tr++) {
                for (int tc = 0; tc < Tc; tc++) {
//#pragma HLS PIPELINE
                    for (int tn = 0; tn < Tn; tn++) {
#pragma HLS UNROLL
                        for (int tm = 0; tm < Tm; tm++) {
#pragma HLS UNROLL
                            for (int i = 0; i < K_max; i++) {
                                for (int j = 0; j < K_max; j++) {
#pragma HLS PIPELINE
                                    if (i == 0 && j == 0 && tn == 0 && n == 0)
                                        out_buf[tm][tr][tc] = b_buf[tm] + w_buf[tn][tm][i][j] *
                                                                          in_buf[tn][S_max * (tr) + i][S_max * (tc) +
                                                                                                       j];
                                    else
                                        out_buf[tm][tr][tc] =
                                                out_buf[tm][tr][tc] + w_buf[tn][tm][i][j] *
                                                                      in_buf[tn][S_max * (tr) + i][S_max * (tc) + j];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Ouput out_buf data to output interface
    void output_res_8ch_itf(G out_buf[][Tr][Tc],
                        Itf *out_data_0, Itf *out_data_1, Itf *out_data_2, Itf *out_data_3,
                        Itf *out_data_4, Itf *out_data_5, Itf *out_data_6, Itf *out_data_7,
                        int out_offset, int n, int m, int r, int c, int N, int M,
                        int R_OUT, int C_OUT, bool act) {
/*
        G* out_port[8] = {out_data_0, out_data_1, out_data_2, out_data_3,
                          out_data_4, out_data_5, out_data_6, out_data_7};
*/
        Itf o_tmp_0 = 0;
        Itf o_tmp_1 = 0;
        Itf o_tmp_2 = 0;
        Itf o_tmp_3 = 0;
        Itf o_tmp_4 = 0;
        Itf o_tmp_5 = 0;
        Itf o_tmp_6 = 0;
        Itf o_tmp_7 = 0;
        if (n >= N - Tn) {
            for (int i = 0; i < Tm && i < M - m; i += 8) {
#pragma HLS UNROLL
                for (int j = r; j < r + Tr && j < R_OUT; j++) {
                    for (int k = c; k < c + Tc && k < C_OUT; k++) {
#pragma HLS PIPELINE
                        if (i + 0 < M - m)
                            *(out_data_0 + ((i + 0 + m) / 8) * R_OUT * C_OUT + j * C_OUT + k) = relu(
                                    out_buf[(i + 0)][j - r][k - c]);
                        if (i + 1 < M - m)
                            *(out_data_1 + ((i + 1 + m) / 8) * R_OUT * C_OUT + j * C_OUT + k) = relu(
                                    out_buf[(i + 1)][j - r][k - c]);
                        if (i + 2 < M - m)
                            *(out_data_2 + ((i + 2 + m) / 8) * R_OUT * C_OUT + j * R_OUT + k) = relu(
                                    out_buf[(i + 2)][j - r][k - c]);
                        if (i + 3 < M - m)
                            *(out_data_3 + ((i + 3 + m) / 8) * R_OUT * C_OUT + j * R_OUT + k) = relu(
                                    out_buf[(i + 3)][j - r][k - c]);
                        if (i + 4 < M - m)
                            *(out_data_4 + ((i + 4 + m) / 8) * R_OUT * C_OUT + j * R_OUT + k) = relu(
                                    out_buf[(i + 4)][j - r][k - c]);
                        if (i + 5 < M - m)
                            *(out_data_5 + ((i + 5 + m) / 8) * R_OUT * C_OUT + j * R_OUT + k) = relu(
                                    out_buf[(i + 5)][j - r][k - c]);
                        if (i + 6 < M - m)
                            *(out_data_6 + ((i + 6 + m) / 8) * R_OUT * C_OUT + j * R_OUT + k) = relu(
                                    out_buf[(i + 6)][j - r][k - c]);
                        if (i + 7 < M - m)
                            *(out_data_7 + ((i + 7 + m) / 8) * R_OUT * C_OUT + j * R_OUT + k) = relu(
                                    out_buf[(i + 7)][j - r][k - c]);

                    }
                }
            }
        }
    }

    void output_res_8ch(G out_buf[][Tr][Tc],
                            G *out_data_0, G *out_data_1, G *out_data_2, G *out_data_3,
                            G *out_data_4, G *out_data_5, G *out_data_6, G *out_data_7,
                            int out_offset, int n, int m, int r, int c, int N, int M,
                            int R_OUT, int C_OUT, bool act) {

        if (n >= N - Tn) {
            for (int i = 0; i < Tm && i < M - m; i += 8) {
#pragma HLS UNROLL
                for (int j = r; j < r + Tr && j < R_OUT; j++) {
                    for (int k = c; k < c + Tc && k < C_OUT; k++) {
#pragma HLS PIPELINE
                        if (i + 0 < M - m)
                            *(out_data_0 + ((i + 0 + m) / 8) * R_OUT * C_OUT + j * C_OUT + k) = relu(
                                    out_buf[(i + 0)][j - r][k - c]);
                        if (i + 1 < M - m)
                            *(out_data_1 + ((i + 1 + m) / 8) * R_OUT * C_OUT + j * C_OUT + k) = relu(
                                    out_buf[(i + 1)][j - r][k - c]);
                        if (i + 2 < M - m)
                            *(out_data_2 + ((i + 2 + m) / 8) * R_OUT * C_OUT + j * R_OUT + k) = relu(
                                    out_buf[(i + 2)][j - r][k - c]);
                        if (i + 3 < M - m)
                            *(out_data_3 + ((i + 3 + m) / 8) * R_OUT * C_OUT + j * R_OUT + k) = relu(
                                    out_buf[(i + 3)][j - r][k - c]);
                        if (i + 4 < M - m)
                            *(out_data_4 + ((i + 4 + m) / 8) * R_OUT * C_OUT + j * R_OUT + k) = relu(
                                    out_buf[(i + 4)][j - r][k - c]);
                        if (i + 5 < M - m)
                            *(out_data_5 + ((i + 5 + m) / 8) * R_OUT * C_OUT + j * R_OUT + k) = relu(
                                    out_buf[(i + 5)][j - r][k - c]);
                        if (i + 6 < M - m)
                            *(out_data_6 + ((i + 6 + m) / 8) * R_OUT * C_OUT + j * R_OUT + k) = relu(
                                    out_buf[(i + 6)][j - r][k - c]);
                        if (i + 7 < M - m)
                            *(out_data_7 + ((i + 7 + m) / 8) * R_OUT * C_OUT + j * R_OUT + k) = relu(
                                    out_buf[(i + 7)][j - r][k - c]);

                    }
                }
            }
        }
    }
///////////////////////------------------conv accelerator----------------//////////////////////////
#if _LAYER_MODE_ // layer function with manual double buffer -- worked
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
        Itf *layer_weights_0, //w[M][N][K][K]
        Itf *layer_weights_1,
        Itf *layer_weights_2,
        Itf *layer_weights_3,
        Itf *layer_weights_4, //w[M][N][K][K]
        Itf *layer_weights_5,
        Itf *layer_weights_6,
        Itf *layer_weights_7,
        Itf *layer_bias,    // b[M]
        int weight_offset,
        int bias_offset,
        int in_offset,
        int out_offset,
        Itf *i_data_0, // in_data[N][(R-1)*S + K][(C-1)*S + K] --> [N][(R-1)*S + K - 2*P][(C-1)*S + K - 2*P]
        Itf *i_data_1,
        Itf *i_data_2,
        Itf *i_data_3,
        Itf *i_data_4,
        Itf *i_data_5,
        Itf *i_data_6,
        Itf *i_data_7,
        G *out_data_0,
        G *out_data_1,
        G *out_data_2,
        G *out_data_3,
        G *out_data_4,
        G *out_data_5,
        G *out_data_6,
        G *out_data_7)
    { // out[M][R][C]

        /***************local data buffer groups******************************/

        T in_buf_0[Tn][(Tr - 1) * S_max + K_max][(Tc - 1) * S_max + K_max];
        W w_buf_0[Tn][Tm][K_max][K_max];
        W b_buf_0[Tm];
        G out_buf_0[Tm][Tr][Tc];

        T in_buf_1[Tn][(Tr - 1) * S_max + K_max][(Tc - 1) * S_max + K_max];
        W w_buf_1[Tn][Tm][K_max][K_max];
        W b_buf_1[Tm];
        G out_buf_1[Tm][Tr][Tc];

        bool in_load_0 = 0;
        bool in_load_1 = 0;
        bool com_0 = 0;
        bool com_1 = 0;
        bool out_0 = 0;
        bool out_1 = 0;

#if _HLS_MODE_

#pragma HLS resource variable=in_buf_0 core=XPM_MEMORY uram
//#pragma HLS resource variable=w_buf_0 core=XPM_MEMORY uram
#pragma HLS resource variable=out_buf_0 core=XPM_MEMORY uram
#pragma HLS resource variable=in_buf_1 core=XPM_MEMORY uram
//#pragma HLS resource variable=w_buf_1 core=XPM_MEMORY uram
#pragma HLS resource variable=out_buf_1 core=XPM_MEMORY uram

#pragma HLS ARRAY_PARTITION variable = in_buf_0 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = w_buf_0 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = w_buf_0 complete dim = 2
//#pragma HLS ARRAY_PARTITION variable = w_buf_0 complete dim = 4
#pragma HLS ARRAY_PARTITION variable = b_buf_0 complete
#pragma HLS ARRAY_PARTITION variable = out_buf_0 complete dim = 1
//II=4 depth=61 // II=2 Depth=65

#pragma HLS ARRAY_PARTITION variable = in_buf_1 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = w_buf_1 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = w_buf_1 complete dim = 2
//#pragma HLS ARRAY_PARTITION variable = w_buf_1 complete dim = 4
#pragma HLS ARRAY_PARTITION variable = b_buf_1 complete
#pragma HLS ARRAY_PARTITION variable = out_buf_1 complete dim = 1

#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        cout << "Starting conv acc double in buffer test ...." << endl;
        //buffer local data initiallization: must do it in C++ debug!
        out_buf_reset(out_buf_0);
        b_buf_reset(b_buf_0);
        w_buf_reset(K, w_buf_0);
        out_buf_reset(out_buf_1);
        b_buf_reset(b_buf_0);
        w_buf_reset(K, w_buf_1);
#endif
#endif


//--------------------------Initial data load ---------------------------------------------//
        for (int r = 0; r < R_OUT; r += Tr)
        {
            for (int c = 0; c < C_OUT; c += Tc)
            {
                for (int m = 0; m < M; m += Tm)
                {
                    for (int n = 0; n < N + Tn; n += Tn)
                    {
                        // ping-pong buffer region
                        if (com_0 == 0)
                        {
//--------------------------Load input B W D in ping-pong manner-------------------------//
                            b_buf_load(b_buf_0, layer_bias, bias_offset, m);
                            w_buf_load_8ch(w_buf_0,
                                        layer_weights_0, layer_weights_1, layer_weights_2, layer_weights_3,
                                        layer_weights_4, layer_weights_5, layer_weights_6, layer_weights_7,
                                        weight_offset, n, m, K, N, M);
                            in_buf_load_8ch(in_buf_0,
                                        i_data_0, i_data_1, i_data_2, i_data_3, i_data_4, i_data_5, i_data_6, i_data_7,
                                        in_offset, n, r, c, S, K, P, R_IN, C_IN, N);
//------------------------------compute buffered data -----------------------------------//
                            conv_engine(in_buf_1, w_buf_1, b_buf_1, out_buf_0, S, n-Tn, N, r, c, K, R_OUT, C_OUT, 0, 0);
                        } else {
//--------------------------Load input B W D in ping-pong manner-------------------------//
                            b_buf_load(b_buf_1, layer_bias, bias_offset, m);
                            w_buf_load_8ch(w_buf_1,
                                       layer_weights_0, layer_weights_1, layer_weights_2, layer_weights_3,
                                       layer_weights_4, layer_weights_5, layer_weights_6, layer_weights_7,
                                       weight_offset, n, m, K, N, M);
                            in_buf_load_8ch(in_buf_1,
                                        i_data_0, i_data_1, i_data_2, i_data_3, i_data_4, i_data_5, i_data_6, i_data_7,
                                        in_offset, n, r, c, S, K, P, R_IN, C_IN, N);
//------------------------------compute buffered data -----------------------------------//
                            conv_engine(in_buf_0, w_buf_0, b_buf_0, out_buf_0, S, n-Tn, N, r, c, K, R_OUT, C_OUT, 0, 0);
                        }
                        com_0 = !com_0;
#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        ofstream in_buf;
        in_buf.open("in_buf_data.txt", ios::app);
        in_buf << "in_buf data: " << endl;
        for (int i = 0; i < Tn && i < N; i++){
            for (int j = 0; j < ((Tr - 1) * S + K); j++){
                for (int k = 0; k < ((Tc - 1) * S + K); k++){
                    if (com_0 == 1) {
                        in_buf << in_buf_0[i][j][k] << " ";
                    } else {
                        in_buf << in_buf_1[i][j][k] << " ";
                    }
                }
                in_buf << endl;
            }
            in_buf << endl;
        }
        in_buf << endl;
        in_buf.close();

        ofstream in_weight;
        in_weight.open("in_weight_data.txt", ios::app);
        in_weight << "in_weight data: " << endl;
        for (int i = 0; i < Tn && i < N; i++){
            for (int h = 0; h < Tm && h < M; h++) {
                for (int j = 0; j < K; j++){
                    for (int k = 0; k < K; k++){
                        if (com_0 == 1) {
                            in_weight << w_buf_0[i][h][j][k] << " ";
                        } else {
                            in_weight << w_buf_1[i][h][j][k] << " ";
                        }
                    }
                    in_weight << endl;
                }
                in_weight << endl;
            }
            in_weight << endl;
        }
        in_weight << endl;
        in_weight.close();
#endif
#endif
                    }
//---------------------------transfer output data----------------------------------------//
                    output_res_8ch(out_buf_0,
                                out_data_0, out_data_1, out_data_2, out_data_3,
                                out_data_4, out_data_5, out_data_6, out_data_7,
                                out_offset, N, m, r, c, N, M, R_OUT, C_OUT, act);
                }
            }
        }


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        cout << "Finished conv_acc_single buffer test ...." << endl;
        ofstream conv_out;
        conv_out.open("conv_out_data_mbuf.txt", ios::app);
        conv_out << "conv output: " << endl;
/*
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < R_OUT; j++)
            {
                for (int k = 0; k < C_OUT; k++)
                {
                    conv_out << out_buf_0[i][j][k] << " ";
                }
                conv_out << endl;
            }
            conv_out << endl;
        }
        conv_out << endl;
*/
conv_out << "Channel M on port: " << M << endl;
for (int i = 0; i < M; i+=8) {
//            if (M%8 == 1) {
//            conv_out << "Channel M at port: " << M%8 << endl;
            for (int j = 0; j < R_OUT; j++)
            {
                for (int k = 0; k < C_OUT; k++)
                {
                    conv_out << *(out_data_0 + (i/8)*R_OUT*C_OUT + j*R_OUT + k) << " ";
                }
                conv_out << endl;
            }
//            }
        conv_out << endl;
//            if (M%8 == 2) {
//            conv_out << "Channel M at port: " << M%8 << endl;
            for (int j = 0; j < R_OUT; j++)
            {
                for (int k = 0; k < C_OUT; k++)
                {
                    conv_out << *(out_data_1 + (i/8)*R_OUT*C_OUT + j*R_OUT + k) << " ";
                }
                conv_out << endl;
            }
//            }
        conv_out << endl;

//            if (M%8 == 3) {
//            conv_out << "Channel M at port: " << M%8 << endl;
            for (int j = 0; j < R_OUT; j++)
            {
                for (int k = 0; k < C_OUT; k++)
                {
                    conv_out << *(out_data_2 + (i/8)*R_OUT*C_OUT + j*R_OUT + k) << " ";
                }
                conv_out << endl;
            }
//            }
        conv_out << endl;

//            if (M%8 == 4) {
//            conv_out << "Channel M at port: " << M%8 << endl;
            for (int j = 0; j < R_OUT; j++)
            {
                for (int k = 0; k < C_OUT; k++)
                {
                    conv_out << *(out_data_3 + (i/8)*R_OUT*C_OUT + j*R_OUT + k) << " ";
                }
                conv_out << endl;
            }
//            }
            conv_out << endl;

//            if (M%8 == 5) {
//            conv_out << "Channel M at port: " << M%8 << endl;
            for (int j = 0; j < R_OUT; j++)
            {
                for (int k = 0; k < C_OUT; k++)
                {
                    conv_out << *(out_data_4 + (i/8)*R_OUT*C_OUT + j*R_OUT + k) << " ";
                }
                conv_out << endl;
            }
//            }
            conv_out << endl;

//            if (M%8 == 6) {
//            conv_out << "Channel M at port: " << M%8 << endl;
//            conv_out << "Output channel M at mem_bank: " << M%8 << endl;
            for (int j = 0; j < R_OUT; j++)
            {
                for (int k = 0; k < C_OUT; k++)
                {
                    conv_out << *(out_data_5 + (i/8)*R_OUT*C_OUT + j*R_OUT + k) << " ";
                }
                conv_out << endl;
            }
//            }
            conv_out << endl;
//            if (M%8 == 7) {
//            conv_out << "Channel M at port: " << M%8 << endl;
//            conv_out << "Output channel M at mem_bank: " << M%8 << endl;
            for (int j = 0; j < R_OUT; j++)
            {
                for (int k = 0; k < C_OUT; k++)
                {
                    conv_out << *(out_data_6 + (i/8)*R_OUT*C_OUT + j*R_OUT + k) << " ";
                }
                conv_out << endl;
            }
//            }
            conv_out << endl;
//            if (M%8 == 8) {
//            conv_out << "Channel M at port: " << M%8 << endl;
//            conv_out << "Output channel M at mem_bank: " << M%8 << endl;
            for (int j = 0; j < R_OUT; j++)
            {
                for (int k = 0; k < C_OUT; k++)
                {
                    conv_out << *(out_data_7 + (i/8)*R_OUT*C_OUT + j*R_OUT + k) << " ";
                }
                conv_out << endl;
            }
//            }
            conv_out << endl;
}
        conv_out.close();
#endif
#endif
    };


    void conv_layer_acc(
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
        W *layer_weights_0, //w[M][N][K][K]
//            W *layer_weights_1,
        W *layer_bias,    // b[M]
        int weight_offset,
        int bias_offset,
        int in_offset,
        int out_offset,
        T *i_data_0, // in_data[N][(R-1)*S + K][(C-1)*S + K] --> [N][(R-1)*S + K - 2*P][(C-1)*S + K - 2*P]
        T *i_data_1,
        T *i_data_2,
        T *i_data_3,
        T *i_data_4,
        T *i_data_5,
        T *i_data_6,
        T *i_data_7,
        G *out_data_0,
        G *out_data_1,
        G *out_data_2,
        G *out_data_3,
        G *out_data_4,
        G *out_data_5,
        G *out_data_6,
        G *out_data_7)
    { // out[M][R][C]

        /***************local data buffer groups******************************/

        T in_buf_0[Tn][(Tr - 1) * S_max + K_max][(Tc - 1) * S_max + K_max];
        W w_buf_0[Tn][Tm][K_max][K_max];
        W b_buf_0[Tm];
        G out_buf_0[Tm][Tr][Tc];

#if _HLS_MODE_

//#pragma HLS DATAFLOW

#pragma HLS ARRAY_PARTITION variable = in_buf_0 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = w_buf_0 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = w_buf_0 complete dim = 2
#pragma HLS ARRAY_PARTITION variable = b_buf_0 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = out_buf_0 complete dim = 1

#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        cout << "Starting conv acc single buffer test ...." << endl;
        //buffer local data initiallization: must do it in C++ debug!
        out_buf_reset(out_buf_0);
        b_buf_reset(b_buf_0);
        w_buf_reset(K, w_buf_0);
#endif
#endif


//--------------------------Initial data load ---------------------------------------------//
        for (int r = 0; r < R_OUT; r += Tr)
        {
            for (int c = 0; c < C_OUT; c += Tc)
            {
                for (int m = 0; m < M; m += Tm)
                {
                    for (int n = 0; n < N; n += Tn)
                    {
//#pragma HLS DATAFLOW
//--------------------------Load input B W D in ping-pong manner-------------------------//
                        b_buf_load(b_buf_0, layer_bias, bias_offset, m);
                        w_buf_load(w_buf_0, layer_weights_0, weight_offset, n, m, K, N, M);
                        in_buf_load(in_buf_0,
                            i_data_0, i_data_1, i_data_2, i_data_3, i_data_4, i_data_5, i_data_6, i_data_7,
                            in_offset, n, r, c, S, K, P, R_IN, C_IN, N);
#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        ofstream in_buf;
        in_buf.open("in_buf_data.txt", ios::app);
        in_buf << "in_buf data: " << endl;
        for (int i = 0; i < Tn && i < N; i++){
            for (int j = 0; j < ((Tr - 1) * S + K); j++){
                for (int k = 0; k < ((Tc - 1) * S + K); k++){
                    in_buf << in_buf_0[i][j][k] << " ";
                }
                in_buf << endl;
            }
            in_buf << endl;
        }
        in_buf << endl;
        in_buf.close();
#endif
#endif
//------------------------------compute buffered data -----------------------------------//
                        conv_engine(in_buf_0, w_buf_0, b_buf_0, out_buf_0, S, n, N, r, c, K, R_OUT, C_OUT, 0, 0);
//---------------------------transfer output data----------------------------------------//
                        output_res(out_buf_0,
                                out_data_0, out_data_1, out_data_2, out_data_3,
                                out_data_4, out_data_5, out_data_6, out_data_7,
                                out_offset, N, m, r, c, N, M, R_OUT, C_OUT, act);
                    }
                }
            }
        }


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        cout << "Finished conv_acc_double buffer test ...." << endl;
        ofstream conv_out;
        conv_out.open("conv_out_data_mbuf.txt", ios::app);
        conv_out << "conv output: " << endl;
/*
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < R_OUT; j++)
            {
                for (int k = 0; k < C_OUT; k++)
                {
                    conv_out << out_buf_0[i][j][k] << " ";
                }
                conv_out << endl;
            }
            conv_out << endl;
        }
        conv_out << endl;
*/

        for (int j = 0; j < R_OUT; j++)
        {
            for (int k = 0; k < C_OUT; k++)
            {
                conv_out << *(out_data_0 + j*R_OUT + k) << " ";
            }
            conv_out << endl;
        }
        conv_out << endl;

        for (int j = 0; j < R_OUT; j++)
        {
                for (int k = 0; k < C_OUT; k++)
                {
                    conv_out << *(out_data_1 + j*R_OUT + k) << " ";
                }
                conv_out << endl;
        }
        conv_out << endl;
        for (int j = 0; j < R_OUT; j++)
        {
                for (int k = 0; k < C_OUT; k++)
                {
                    conv_out << *(out_data_2 + j*R_OUT + k) << " ";
                }
                conv_out << endl;
        }
        conv_out << endl;

            for (int j = 0; j < R_OUT; j++)
            {
                for (int k = 0; k < C_OUT; k++)
                {
                    conv_out << *(out_data_3 + j*R_OUT + k) << " ";
                }
                conv_out << endl;
            }
            conv_out << endl;

            for (int j = 0; j < R_OUT; j++)
            {
                for (int k = 0; k < C_OUT; k++)
                {
                    conv_out << *(out_data_4 + j*R_OUT + k) << " ";
                }
                conv_out << endl;
            }
            conv_out << endl;

        if (M%8 >= 6) {
        conv_out << "Output channel M at mem_bank: " << M%8 << endl;
            for (int j = 0; j < R_OUT; j++)
            {
                for (int k = 0; k < C_OUT; k++)
                {
                    conv_out << *(out_data_5 + j*R_OUT + k) << " ";
                }
                conv_out << endl;
            }
        }
            conv_out << endl;
        if (M%8 >= 7) {
        conv_out << "Output channel M at mem_bank: " << M%8 << endl;
            for (int j = 0; j < R_OUT; j++)
            {
                for (int k = 0; k < C_OUT; k++)
                {
                    conv_out << *(out_data_6 + j*R_OUT + k) << " ";
                }
                conv_out << endl;
            }
        }
            conv_out << endl;
        if (M%8 >= 8) {
        conv_out << "Output channel M at mem_bank: " << M%8 << endl;
            for (int j = 0; j < R_OUT; j++)
            {
                for (int k = 0; k < C_OUT; k++)
                {
                    conv_out << *(out_data_7 + j*R_OUT + k) << " ";
                }
                conv_out << endl;
            }
        }
            conv_out << endl;

        conv_out.close();
#endif
#endif
    };
#endif

};

#endif
