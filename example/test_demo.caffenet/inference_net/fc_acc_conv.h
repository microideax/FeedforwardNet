#ifndef _FC_ACC_CONV_H_
#define _FC_ACC_CONV_H_

#include <iostream>
#include <fstream>
#include "activation_functions.h"

#if _C_DEBUG_MODE_
#include <algorithm>
#endif

using namespace std;

template<typename Itf, typename T, typename W, typename G, int Tm, int Tn, int Tr, int Tc, int S_max, int K_max, int IBUF_t, int WBUF_t, int OBUF_t>
class fc_acc_conv {

private:
    int conv_layer_number;

public:
    fc_acc_conv() : conv_layer_number(0) { conv_layer_number = 0; };

    ////------------------------------C++ debugging functions---------------------------------------////
    // Reset output buffer
    void out_buf_reset(G buf[]) {
        for (int i = 0; i < Tm; i++) {
            for (int j = 0; j < 1; j++) {
                for (int k = 0; k < 1; k++) {
                    buf[i] = G(0);
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
        }
    }

    void in_buf_load(T buf[][K_max][K_max], T *in_data_1, int in_offset,
                     int n, int r, int c, int S, int K, int P, int R_IN, int C_IN, int N) {
        for (int j = r * S - P; j < r * S + K - P && j < R_IN; j++) {
            for (int k = c * S - P; k < c * S + K - P && k < C_IN; k++) {
#pragma HLS PIPELINE
                for (int i = 0; i < Tn; i += 1) {
#pragma HLS UNROLL
                    if ((n + Tn > N && i + 0 >= N - n) || j < 0 || j >= R_IN || k < 0 || k >= C_IN) {
                        buf[i + 0][j - r * S + P][k - c * S + P] = T(0);
                    } else {
                        buf[i + 0][j - r * S + P][k - c * S + P] = *(in_data_1 + in_offset
                                                                     + (i + n) / 1 * R_IN * C_IN
                                                                     + j * C_IN + k);
                    }
                }
            }
        }
    }

    void in_buf_load_8ch(T buf[][K_max][K_max],
                     T *i_data_0,
                     T *i_data_1,
                     T *i_data_2,
                     T *i_data_3,
                     T *i_data_4,
                     T *i_data_5,
                     T *i_data_6,
                     T *i_data_7,
                     int in_offset,
                     int n, int r, int c, int S, int K, int P, int R_IN, int C_IN, int N) {
        for (int j = r * S - P; j < r * S + K - P && j < R_IN; j++) {
            for (int k = c * S - P; k < c * S + K - P && k < C_IN; k++) {
#pragma HLS PIPELINE
                for (int i = 0; i < Tn; i += 8) {
#pragma HLS UNROLL
                    if ((n + Tn > N && (i + 0) % 8 >= N - n) || j < 0 || j >= R_IN || k < 0 || k >= C_IN) {
                        buf[i + 0][j - r * S + P][k - c * S + P] = T(0);
                    } else {
                        buf[i + 0][j - r * S + P][k - c * S + P] = *(i_data_0 + in_offset + (i + n) / 8 * R_IN * C_IN +
                                                                     j * C_IN + k);
                    }
                    if ((n + Tn > N && (i + 1) % 8 >= N - n) || j < 0 || j >= R_IN || k < 0 || k >= C_IN) {
                        buf[i + 1][j - r * S + P][k - c * S + P] = T(0);
                    } else {
                        buf[i + 1][j - r * S + P][k - c * S + P] = *(i_data_1 + in_offset + (i + n) / 8 * R_IN * C_IN +
                                                                     j * C_IN + k);
                    }
                    if ((n + Tn > N && (i + 2) % 8 >= N - n) || j < 0 || j >= R_IN || k < 0 || k >= C_IN) {
                        buf[i + 2][j - r * S + P][k - c * S + P] = T(0);
                    } else {
                        buf[i + 2][j - r * S + P][k - c * S + P] = *(i_data_2 + in_offset + (i + n) / 8 * R_IN * C_IN +
                                                                     j * C_IN + k);
                    }
                    if ((n + Tn > N && (i + 3) % 8 >= N - n) || j < 0 || j >= R_IN || k < 0 || k >= C_IN) {
                        buf[i + 3][j - r * S + P][k - c * S + P] = T(0);
                    } else {
                        buf[i + 3][j - r * S + P][k - c * S + P] = *(i_data_3 + in_offset + (i + n) / 8 * R_IN * C_IN +
                                                                     j * C_IN + k);
                    }
                    if ((n + Tn > N && (i + 4) % 8 >= N - n) || j < 0 || j >= R_IN || k < 0 || k >= C_IN) {
                        buf[i + 4][j - r * S + P][k - c * S + P] = T(0);
                    } else {
                        buf[i + 4][j - r * S + P][k - c * S + P] = *(i_data_4 + in_offset + (i + n) / 8 * R_IN * C_IN +
                                                                     j * C_IN + k);
                    }
                    if ((n + Tn > N && (i + 5) % 8 >= N - n) || j < 0 || j >= R_IN || k < 0 || k >= C_IN) {
                        buf[i + 5][j - r * S + P][k - c * S + P] = T(0);
                    } else {
                        buf[i + 5][j - r * S + P][k - c * S + P] = *(i_data_5 + in_offset + (i + n) / 8 * R_IN * C_IN +
                                                                     j * C_IN + k);
                    }
                    if ((n + Tn > N && (i + 6) % 8 >= N - n) || j < 0 || j >= R_IN || k < 0 || k >= C_IN) {
                        buf[i + 6][j - r * S + P][k - c * S + P] = T(0);
                    } else {
                        buf[i + 6][j - r * S + P][k - c * S + P] = *(i_data_6 + in_offset + (i + n) / 8 * R_IN * C_IN +
                                                                     j * C_IN + k);
                    }
                    if ((n + Tn > N && (i + 7) % 8 >= N - n) || j < 0 || j >= R_IN || k < 0 || k >= C_IN) {
                        buf[i + 7][j - r * S + P][k - c * S + P] = T(0);
                    } else {
                        buf[i + 7][j - r * S + P][k - c * S + P] = *(i_data_7 + in_offset + (i + n) / 8 * R_IN * C_IN +
                                                                     j * C_IN + k);
                    }
                }
            }
        }
    }

// Load weights to weight buffer
    void w_buf_load(W buf[][Tm][K_max][K_max], W *layer_weights, int weight_offset, int n, int m, int K, int N, int M) {
        for (int k1 = 0; k1 < K; k1++) {
            for (int k2 = 0; k2 < K; k2++) {
                for (int j = 0; j < Tn && j < N - n; j++) {
                    for (int i = 0; i < Tm && i < M - m; i++) {
#pragma HLS PIPELINE
                        buf[j][i][k1][k2] = *(layer_weights + weight_offset
                                              + (i + m) * N * K * K + (j + n) * K * K + k1 * K + k2);
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
                        w_tmp_0 = *(layer_weights_0 + weight_offset + (i + m) * N * K * K/8 +
                                    (j + 0 + n) * K * K + k1 * K + k2);
                        buf[j + 0][i][k1][k2] = w_tmp_0.range(15, 0);
                        w_tmp_1 = *(layer_weights_1 + weight_offset + (i + m) * N * K * K/8 +
                                    (j + 1 + n) * K * K + k1 * K + k2);
                        buf[j + 1][i][k1][k2] = w_tmp_1.range(15, 0);
                        w_tmp_2 = *(layer_weights_2 + weight_offset + (i + m) * N * K * K/8 +
                                    (j + 2 + n) * K * K + k1 * K + k2);
                        buf[j + 2][i][k1][k2] = w_tmp_2.range(15, 0);
                        w_tmp_3 = *(layer_weights_3 + weight_offset + (i + m) * N * K * K/8 +
                                    (j + 3 + n) * K * K + k1 * K + k2);
                        buf[j + 3][i][k1][k2] = w_tmp_3.range(15, 0);
                        w_tmp_4 = *(layer_weights_4 + weight_offset + (i + m) * N * K * K/8 +
                                    (j + 4 + n) * K * K + k1 * K + k2);
                        buf[j + 4][i][k1][k2] = w_tmp_4.range(15, 0);
                        w_tmp_5 = *(layer_weights_5 + weight_offset + (i + m) * N * K * K/8 +
                                    (j + 5 + n) * K * K + k1 * K + k2);
                        buf[j + 5][i][k1][k2] = w_tmp_5.range(15, 0);
                        w_tmp_6 = *(layer_weights_6 + weight_offset + (i + m) * N * K * K/8 +
                                    (j + 6 + n) * K * K + k1 * K + k2);
                        buf[j + 6][i][k1][k2] = w_tmp_6.range(15, 0);
                        w_tmp_7 = *(layer_weights_7 + weight_offset + (i + m) * N * K * K/8 +
                                    (j + 7 + n) * K * K + k1 * K + k2);
                        buf[j + 7][i][k1][k2] = w_tmp_7.range(15, 0);
                    }
//                    cout << "access location: " << (i + m) * N * K * K + (j + 7 + n) * K * K << endl;
//                    cout << "i j m n: " << i <<" " << j << " " << m << " " << n << " " << endl;
                }
            }
        }

    }

// Convolution computation kernel
    void fc_engine(T in_buf[][K_max][K_max], W w_buf[][Tm][K_max][K_max], W b_buf[], G out_buf[][Tr][Tc],
                   int S, int n, int r, int c, int N, int M, int K, int R_OUT, int C_OUT, int w_offset, int i_offset) {

        G out_tmp[Tm][K_max];
        G tmp_var;

        if (n >= 0 && n - Tn < N) {
            // instantiate the out_buf with bias data value
            if (n == 0) {
                for (int i = 0; i < Tm; i++) {
#pragma HLS UNROLL
                    out_buf[i][0][0] = b_buf[i];
                }
            }

            for (int i = 0; i < K; i++) {
                for (int j = 0; j < K; j++) {
#pragma HLS PIPELINE
                    for (int tm = 0; tm < Tm; tm++) {
#pragma HLS UNROLL
                        tmp_var = 0;
                        for (int tn = 0; tn < Tn; tn++) {
#pragma HLS UNROLL
                            tmp_var = tmp_var + w_buf[tn][tm][i][j] * in_buf[tn][i][j];
                        }
                        out_buf[tm][0][0] += tmp_var;
                    }
                }
            }
        }
    }

    // Ouput out_buf data to output interface
    void output_res(G out_buf[], G *out_data_1, int out_offset,
                    int n, int m, int r, int c, int N, int M, int R_OUT, int C_OUT, bool act) {
        if (n >= N - Tn) {
            for (int j = r; j < r + Tr && j < R_OUT; j++) {
                for (int k = c; k < c + Tc && k < C_OUT; k++) {
                    for (int i = 0; i < Tm && i < M - m; i += 1) {
#pragma HLS PIPELINE
                        if (act) {
                            if (i + 0 < M - m)
                                *(out_data_1 + out_offset + ((i + m) / 1) * R_OUT * C_OUT + j * C_OUT + k) = relu(
                                        out_buf[i + 0]);
                        } else {
                            if (i + 0 < M - m)
                                *(out_data_1 + out_offset + ((i + m) / 1) * R_OUT * C_OUT + j * C_OUT + k) = out_buf[i +
                                                                                                                     0];
                        }
                    }
                }
            }
        }
    }

// Ouput out_buf data to output interface
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

    // combined data load
    void load_data(W b_buf_0[Tm], W w_buf_0[Tn][Tm][K_max][K_max], W *layer_bias, W *layer_weights, int bias_offset,
                   int weight_offset,
                   int m, int K, int N, int M) {
        b_buf_load(b_buf_0, layer_bias, bias_offset, m);
        w_buf_load(w_buf_0, layer_weights, weight_offset, 0, m, K, N, M);
    }
///////////////////////------------------conv accelerator----------------//////////////////////////
#if _LAYER_MODE_ // layer function with manual double buffer -- worked
    void fc_layer_acc_mbuf(
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
        Itf *layer_weights_4,
        Itf *layer_weights_5,
        Itf *layer_weights_6,
        Itf *layer_weights_7,
        Itf *layer_bias,    // b[M]
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
        T in_buf_0[Tn][K_max][K_max];
        W w_buf_0[Tn][Tm][K_max][K_max];
        W b_buf_0[Tm];
        G out_buf_0[Tm][Tr][Tc];

        T in_buf_1[Tn][K_max][K_max];
        W w_buf_1[Tn][Tm][K_max][K_max];
        W b_buf_1[Tm];
        G out_buf_1[Tm][Tr][Tc];

        bool buf_ptr = 0;

#if _HLS_MODE_

//#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS

#pragma HLS ARRAY_PARTITION variable = in_buf_0 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = w_buf_0 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = w_buf_0 complete dim = 2
#pragma HLS ARRAY_PARTITION variable = b_buf_0 complete  dim = 0
#pragma HLS ARRAY_PARTITION variable = out_buf_0 complete dim = 0

#pragma HLS ARRAY_PARTITION variable = in_buf_1 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = w_buf_1 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = w_buf_1 complete dim = 2
#pragma HLS ARRAY_PARTITION variable = b_buf_1 complete dim = 0
#pragma HLS ARRAY_PARTITION variable = out_buf_1 complete dim = 0

#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        cout << "Starting fc acc single buffer test ...." << endl;
        //buffer local data initiallization: must do it in C++ debug!
        //out_buf_reset(out_buf_0);
        //b_buf_reset(b_buf_0);
        //w_buf_reset(K, w_buf_0);
#endif
#endif

        for (int r = 0; r < R_OUT; r += Tr)
        {
            for (int c = 0; c < C_OUT; c += Tc)
            {
                for (int m = 0; m < M; m += Tm)
                {
                    for (int n = 0; n < N + Tn; n += Tn) {
                        if (buf_ptr == 0) {
//                        cout << "Processing buffer 0" << endl;
                            //--------------------------Load input B W D in ping-pong manner-------------------------//
                            in_buf_load_8ch(in_buf_0, i_data_0, i_data_1, i_data_2, i_data_3, i_data_4, i_data_5, i_data_6, i_data_7,
                                        in_offset, n, r, c, S, K, P, R_IN, C_IN, N);
                            w_buf_load_8ch(w_buf_0,
                                            layer_weights_0, layer_weights_1, layer_weights_2, layer_weights_3,
                                            layer_weights_4, layer_weights_5, layer_weights_6, layer_weights_7,
                                            weight_offset, n, m, K, N, M);
                            b_buf_load(b_buf_0, layer_bias, bias_offset, m);
                            //------------------------------compute buffered data -----------------------------------//
                            fc_engine(in_buf_1, w_buf_1, b_buf_1, out_buf_0, S, n - Tn, r, c, N, M, K, R_OUT, C_OUT, 0, 0);
                        } else {
//                        cout << "Processing buffer 1" << endl;
                            //--------------------------Load input B W D in ping-pong manner-------------------------//
                            in_buf_load_8ch(in_buf_1, i_data_0, i_data_1, i_data_2, i_data_3, i_data_4, i_data_5, i_data_6, i_data_7,
                                        in_offset, n, r, c, S, K, P, R_IN, C_IN, N);
                            w_buf_load_8ch(w_buf_1,
                                           layer_weights_0, layer_weights_1, layer_weights_2, layer_weights_3,
                                           layer_weights_4, layer_weights_5, layer_weights_6, layer_weights_7,
                                           weight_offset, n, m, K, N, M);
                            b_buf_load(b_buf_1, layer_bias, bias_offset, m);
                            //------------------------------compute buffered data -----------------------------------//
                            fc_engine(in_buf_0, w_buf_0, b_buf_0, out_buf_0, S, n - Tn, r, c, N, M, K, R_OUT, C_OUT, 0, 0);
                        }
                        buf_ptr = !buf_ptr;
//                        cout << "finished " << n << "iteration" << endl;
                    }
                    //---------------------------transfer output data----------------------------------------//

//                    cout << "fc acc buffer output location (N, m, r, c, N, M, R_OUT, C_OUT): " << N <<
//                    " "<< m << " " << r << " " << c << " " << M << " " << R_OUT << endl;
                    output_res_8ch(out_buf_0,
                                    out_data_0, out_data_1, out_data_2, out_data_3,
                                    out_data_4, out_data_5, out_data_6, out_data_7,
                                    out_offset, N, m, r, c, N, M, R_OUT, C_OUT, act);
//                    cout << "output_res iteration " << m << " done!" << endl;
                }
            }
        }
    };

#endif
};

#endif
