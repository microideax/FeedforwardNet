#ifndef _CONV_ACC_H_
#define _CONV_ACC_H_

#include <iostream>
#include <fstream>
#include "activation_functions.h"

#if _C_DEBUG_MODE_
#include <algorithm>
#endif

using namespace std;

template <typename T, typename W, typename G, int Tm, int Tn, int Tr, int Tc, int S_max, int K_max, int IBUF_t, int WBUF_t, int OBUF_t>
class conv_acc
{

  private:
    int conv_layer_number;

  public:
    conv_acc() : conv_layer_number(0) { conv_layer_number = 0; };

    ////------------------------------C++ debugging functions---------------------------------------////
    // Reset output buffer
    void out_buf_reset(G buf[][Tr][Tc])
    {
        for (int i = 0; i < Tm; i++)
        {
            for (int j = 0; j < Tr; j++)
            {
                for (int k = 0; k < Tc; k++)
                {
                    buf[i][j][k] = G(0);
                }
            }
        }
    }
    // Reset weight buffer
    void w_buf_reset(int K, W buf[][Tm][K_max][K_max])
    {
        for (int i = 0; i < Tn; i++)
        {
            for (int j = 0; j < Tm; j++)
            {
                for (int k = 0; k < K; k++)
                {
                    for (int l = 0; l < K; l++)
                    {
                        buf[i][j][k][l] = W(0);
                    }
                }
            }
        }
    }
    // Reset bias buffer
    void b_buf_reset(W buf[])
    {
        for (int i = 0; i < Tm; i++)
        {
            buf[i] = W(0);
        }
    }
    ////-----------------------------Accelerator Functions---------------------------------------////
    // Load bias data
    void b_buf_load(W buf[], W *layer_bias, int bias_offset, int m)
    {
        for (int i = 0; i < Tm; i++)
        {
            buf[i] = *(layer_bias + bias_offset + i + m);
        }
    }

    void
    in_buf_load(T buf[][(Tr - 1) * S_max + K_max][(Tc - 1) * S_max + K_max],
                T *in_data_1, int in_offset, int n, int r,
                int c, int S, int K, int P, int R_IN, int C_IN, int N)
        {
            for (int j = r * S - P; j < (r + Tr - 1) * S + K - P && j < R_IN; j++) {
                for (int k = c * S - P; k < (c + Tc - 1) * S + K - P && j < C_IN; k++) {
#pragma HLS PIPELINE
                    for (int i = 0; i < Tn && i < N; i += 1) {
#pragma HLS UNROLL
                        if ((n + Tn > N && i + 0 >= N - n) || j < 0 || j >= R_IN || k < 0 || k >= C_IN) {
                            buf[i + 0][j - r * S + P][k - c * S + P] = T(0);
                        } else {
                            buf[i + 0][j - r * S + P][k - c * S + P] = *(in_data_1 + in_offset +
                                                                         (i + n) / 1 * R_IN * C_IN +
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
                            buf[j][i][k1][k2] = *(layer_weights + weight_offset + (i + m) * N * K * K +
                                                  (j + n) * K * K + k1 * K + k2);
                        }
                    }
                }
            }
        }

// Convolution computation kernel
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
                                                                              in_buf[tn][S * (tr) + i + i_offset][
                                                                                      S * (tc) + j];
                                        else
                                            out_buf[tm][tr][tc] = out_buf[tm][tr][tc] + w_buf[tn][tm][i + w_offset][j] *
                                                                                        in_buf[tn][S * (tr) + i +
                                                                                                   i_offset][
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
        void output_res(G out_buf[][Tr][Tc], G *out_data_1, int out_offset, int n, int m, int r, int c, int N, int M,
                        int R_OUT, int C_OUT, bool act) {
            if (n >= N - Tn) {
                for (int j = r; j < r + Tr && j < R_OUT; j++) {
                    for (int k = c; k < c + Tc && k < C_OUT; k++) {
                        for (int i = 0; i < Tm && i < M - m; i += 1) {
#pragma HLS PIPELINE
                            if (act) {
                                if (i + 0 < M - m)
                                    *(out_data_1 + out_offset + ((i + m) / 1) * R_OUT * C_OUT + j * C_OUT + k) = relu(
                                            out_buf[i + 0][j - r][k - c]);
                            } else {
                                if (i + 0 < M - m)
                                    *(out_data_1 + out_offset + ((i + m) / 1) * R_OUT * C_OUT + j * C_OUT +
                                      k) = out_buf[i + 0][j - r][k - c];
                            }
                        }
                    }
                }
            }
        }

///////////////////////------------------conv accelerator----------------//////////////////////////
#if _LAYER_MODE_ // layer function with manual double buffer -- failed
        void conv_layer_acc_dbuf(
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
            W *layer_weights, //w[M][N][K][K]
            W *layer_bias,    // b[M]
            int weight_offset,
            int bias_offset,
            int in_offset,
            int out_offset,
            T *in_data_1, // in_data[N][(R-1)*S + K][(C-1)*S + K] --> [N][(R-1)*S + K - 2*P][(C-1)*S + K - 2*P]
            G *out_data_1)
        { // out[M][R][C]

            /***************local data buffer******************************/

            T in_buf_1[Tn][(Tr - 1) * S_max + K_max][(Tc - 1) * S_max + K_max];
            T in_buf_0[Tn][(Tr - 1) * S_max + K_max][(Tc - 1) * S_max + K_max];
            W w_buf_1[Tn][Tm][K_max][K_max];
            W w_buf_0[Tn][Tm][K_max][K_max];
            W b_buf_1[Tm];
            W b_buf_0[Tm];
            G out_buf_1[Tm][Tr][Tc];
            G out_buf_0[Tm][Tr][Tc];

            /***************Ptr and buffer initialization******************************/

            bool in_buf_0_empty = 1;
            bool in_buf_1_empty = 1;
            bool out_buf_0_empty = 1;
            bool out_buf_1_empty = 1;
            int loadbufPtr = 0;
            int combufPtr = 0;
            int resbufPtr = 0;
            bool last_com = 0;
            bool last_load = 0;
            bool last_res = 0;

#if _HLS_MODE_
#pragma HLS ARRAY_PARTITION variable = in_buf_1 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = in_buf_0 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = w_buf_1 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = w_buf_1 complete dim = 2
#pragma HLS ARRAY_PARTITION variable = w_buf_0 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = w_buf_0 complete dim = 2
#pragma HLS ARRAY_PARTITION variable = b_buf_1 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = b_buf_0 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = out_buf_1 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = out_buf_0 complete dim = 1
#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
            cout << "Starting conv_acc_innerpp layer ...." << endl;
            //buffer local data initiallization: must do it in C++ debug!
            out_buf_reset(out_buf_1);
            out_buf_reset(out_buf_0);
            b_buf_reset(b_buf_1);
            b_buf_reset(b_buf_0);
            w_buf_reset(K, w_buf_1);
            w_buf_reset(K, w_buf_0);
#endif
#endif

            for (int r = 0; r < R_OUT; r += Tr)
            {
                for (int c = 0; c < C_OUT; c += Tc)
                {
                    for (int m = 0; m < M; m += Tm)
                    {
                        for (int n = 0; n < N; n += 2 * Tn)
                        {
                            //--------------------------Load input B W D in ping-pong manner-------------------------//
                            while ((in_buf_0_empty | in_buf_1_empty) && (!last_load))
                            {
                                if (loadbufPtr == 1)
                                {
                                    cout << "loading input buffer 1...." << endl;
                                    //load input bias
                                    b_buf_load(b_buf_1, layer_bias, bias_offset, m);
                                    // load input data
                                    in_buf_load(in_buf_1, in_data_1, in_offset, n + Tn, r, c, S, K, P, R_IN, C_IN, N);
                                    // load input weights
                                    w_buf_load(w_buf_1, layer_weights, weight_offset, n + Tn, m, K, N, M);
                                    in_buf_1_empty = 0;
                                    cout << "buffer 1 full" << endl;
                                    loadbufPtr = 0;
                                    if (n + 2 * Tn >= N)
                                    {
                                        last_load = 1;
                                    }
                                }
                                else
                                {
                                    cout << "loading input buffer 0...." << endl;
                                    //load input bias
                                    b_buf_load(b_buf_0, layer_bias, bias_offset, m);
                                    // load input data
                                    in_buf_load(in_buf_0, in_data_1, in_offset, n, r, c, S, K, P, R_IN, C_IN, N);
                                    // load input weights
                                    w_buf_load(w_buf_0, layer_weights, weight_offset, n, m, K, N, M);
                                    in_buf_0_empty = 0;
                                    cout << "buffer 0 full" << endl;
                                    loadbufPtr = 1;
                                    if (n + Tn >= N)
                                    {
                                        last_load = 1;
                                    }
                                }
                            }
                            loadbufPtr = 0;
                            last_load = 0;
                            //------------------------------compute buffered data -----------------------------------//
                            while ((!in_buf_0_empty | !in_buf_1_empty) && (!last_com))
                            {
                                if (combufPtr == 1)
                                {
                                    cout << "computing input buffer 1...." << endl;
                                    if (resbufPtr == 1)
                                    {
                                        conv_engine(in_buf_1, w_buf_1, b_buf_1, out_buf_1, S, n + Tn, N, r, c, K, R_OUT, C_OUT, 0, 0);
                                        out_buf_1_empty = 0;
                                    }
                                    else
                                    {
                                        conv_engine(in_buf_1, w_buf_1, b_buf_1, out_buf_0, S, n + Tn, N, r, c, K, R_OUT, C_OUT, 0, 0);
                                        out_buf_0_empty = 0;
                                    }
                                    in_buf_1_empty = 1;
                                    combufPtr = 0;
                                    cout << "buffer 1 computed" << endl;
                                    if (n + 2 * Tn >= N)
                                    {
                                        last_com = 1;
                                    }
                                }
                                else
                                {
                                    cout << "computing input buffer 0...." << endl;
                                    if (resbufPtr == 1)
                                    {
                                        conv_engine(in_buf_0, w_buf_0, b_buf_0, out_buf_1, S, n, N, r, c, K, R_OUT, C_OUT, 0, 0);
                                        out_buf_1_empty = 0;
                                    }
                                    else
                                    {
                                        conv_engine(in_buf_0, w_buf_0, b_buf_0, out_buf_0, S, n, N, r, c, K, R_OUT, C_OUT, 0, 0);
                                        out_buf_0_empty = 0;
                                    }
                                    in_buf_0_empty = 1;
                                    combufPtr = 1;
                                    cout << "buffer 0 computed" << endl;
                                    if (n + Tn >= N)
                                    {
                                        last_com = 1;
                                    }
                                }
                            }
                            combufPtr = 0;
                            last_com = 0;
                            //---------------------------transfer output data----------------------------------------//
                            while ((!out_buf_0_empty | !out_buf_1_empty) && (!last_res))
                            {
                                if (resbufPtr == 1)
                                {
                                    cout << "output buffer 1...." << endl;
                                    // transfer output data
                                    if (n + Tn >= N)
                                    {
                                        last_res = 1;
                                        resbufPtr = 0;
                                        output_res(out_buf_1, out_data_1, out_offset, n, m, r, c, N, M, R_OUT, C_OUT, act);
                                    }
                                    else if (n + 2 * Tn >= N)
                                    {
                                        last_res = 1;
                                        resbufPtr = 0;
                                        output_res(out_buf_1, out_data_1, out_offset, n + Tn, m, r, c, N, M, R_OUT, C_OUT, act);
                                    }
                                    out_buf_1_empty = 1;
                                    cout << "buffer 1 res" << endl;
                                }
                                else
                                {
                                    cout << "output buffer 0...." << endl;
                                    // transfer output data
                                    if (n + Tn >= N)
                                    {
                                        last_res = 1;
                                        resbufPtr = 1;
                                        output_res(out_buf_0, out_data_1, out_offset, n, m, r, c, N, M, R_OUT, C_OUT, act);
                                    }
                                    else if (n + 2 * Tn >= N)
                                    {
                                        last_res = 1;
                                        resbufPtr = 1;
                                        output_res(out_buf_0, out_data_1, out_offset, n + Tn, m, r, c, N, M, R_OUT, C_OUT, act);
                                    }
                                    out_buf_0_empty = 1;
                                    cout << "buffer 0 res" << endl;
                                }
                            }
                            last_res = 0;
                        }
                    }
                }
            }
#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
            cout << "Finished conv_acc_innerpp layer ...." << endl;
            ofstream conv_out;
            conv_out.open("conv_out_data.txt", ios::app);
            conv_out << "conv output: " << endl;
            for (int i = 0; i < M / 1; i++)
            {
                for (int j = 0; j < R_OUT; j++)
                {
                    for (int k = 0; k < C_OUT; k++)
                    {
                        conv_out << *(out_data_1 + out_offset + i * R_OUT * C_OUT + j * C_OUT + k) << " ";
                    }
                    conv_out << endl;
                }
                conv_out << endl;
            }
            conv_out.close();
#endif
#endif
        };
#endif


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
            W *layer_weights, //w[M][N][K][K]
            W *layer_bias,    // b[M]
            int weight_offset,
            int bias_offset,
            int in_offset,
            int out_offset,
            T *in_data_1, // in_data[N][(R-1)*S + K][(C-1)*S + K] --> [N][(R-1)*S + K - 2*P][(C-1)*S + K - 2*P]
            G *out_data_1)
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

    //#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS

#pragma HLS ARRAY_PARTITION variable = in_buf_0 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = w_buf_0 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = w_buf_0 complete dim = 2
#pragma HLS ARRAY_PARTITION variable = b_buf_0 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = out_buf_0 complete dim = 1

#pragma HLS ARRAY_PARTITION variable = in_buf_1 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = w_buf_1 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = w_buf_1 complete dim = 2
#pragma HLS ARRAY_PARTITION variable = b_buf_1 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = out_buf_1 complete dim = 1

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
                        for (int n = 0; n < N + Tn; n += Tn)
                        {
                            // ping-pong buffer region
                            if (com_0 == 0)
                            {
    //--------------------------Load input B W D in ping-pong manner-------------------------//
                                b_buf_load(b_buf_0, layer_bias, bias_offset, m);
                                w_buf_load(w_buf_0, layer_weights, weight_offset, n, m, K, N, M);
                                in_buf_load(in_buf_0, in_data_1, in_offset, n, r, c, S, K, P, R_IN, C_IN, N);
    //------------------------------compute buffered data -----------------------------------//
                                conv_engine(in_buf_1, w_buf_1, b_buf_1, out_buf_0, S, n-Tn, N, r, c, K, R_OUT, C_OUT, 0, 0);
                            } else {
    //--------------------------Load input B W D in ping-pong manner-------------------------//
                                b_buf_load(b_buf_1, layer_bias, bias_offset, m);
                                w_buf_load(w_buf_1, layer_weights, weight_offset, n, m, K, N, M);
                                in_buf_load(in_buf_1, in_data_1, in_offset, n, r, c, S, K, P, R_IN, C_IN, N);
    //------------------------------compute buffered data -----------------------------------//
                                conv_engine(in_buf_0, w_buf_0, b_buf_0, out_buf_0, S, n-Tn, N, r, c, K, R_OUT, C_OUT, 0, 0);
                            }
                            com_0 = !com_0;
                        }
    //---------------------------transfer output data----------------------------------------//
                        output_res(out_buf_0, out_data_1, out_offset, N, m, r, c, N, M, R_OUT, C_OUT, act);
                    }
                }
            }


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
            cout << "Finished conv_acc_single buffer test ...." << endl;
            ofstream conv_out;
            conv_out.open("conv_out_data_sbuf.txt", ios::app);
            conv_out << "conv output: " << endl;
            for (int i = 0; i < M / 1; i++)
            {
                for (int j = 0; j < R_OUT; j++)
                {
                    for (int k = 0; k < C_OUT; k++)
                    {
                        conv_out << *(out_data_1 + out_offset + i * R_OUT * C_OUT + j * C_OUT + k) << " ";
                    }
                    conv_out << endl;
                }
                conv_out << endl;
            }
            conv_out.close();
#endif
#endif
        };

#endif

////////-----------------CONV_ACC----------------////////
#if _ACC_MODE_
        void conv_core_acc(
            data_type_w in_buf_0[Tn][IBUF_t][IBUF_t],
            data_type_w w_buf_0[Tn][Tm][WBUF_t][WBUF_t],
            data_type_w b_buf_0[Tm],
            data_type_w out_buf_0[Tm][OBUF_t][OBUF_t],
            int param[16])
        {

            data_type_w out_buf_tmp[Tm][Tr][Tc];
#pragma HLS ARRAY_PARTITION variable = out_buf_tmp complete dim = 1

            conv_engine(in_buf_0, w_buf_0, b_buf_0, out_buf_tmp, param[0], param[1], param[2], param[3], param[4], param[5], param[6], param[7], param[8]);

            for (int j = 0; j < Tr; j++)
            {
                for (int k = 0; k < Tc; k++)
                {
#pragma HLS PIPELINE
                    for (int i = 0; i < Tm; i++)
                    {
                        out_buf_0[i][j][k] = out_buf_tmp[i][j][k];
                    }
                }
            }
        };
#endif

    };
#endif
