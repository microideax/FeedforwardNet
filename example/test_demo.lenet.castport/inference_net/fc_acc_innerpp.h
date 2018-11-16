#ifndef _FC_ACC_H_
#define _FC_ACC_H_

#include <iostream>
#include <fstream>
#include "config.h"
#include "activation_functions.h"

#if _C_DEBUG_MODE_
#include <algorithm>
#endif

using namespace std;

template <typename Itf, typename Tparam, typename T, typename W, typename G, int iTm, int iTn, int iS_max, int iK_max>
class fc_acc
{

  private:
    int fc_layer_number;

  public:
    fc_acc() : fc_layer_number(0) { fc_layer_number = 0; };

    ////------------------------------C++ debugging functions---------------------------------------////
    // Reset output buffer
    void out_buf_reset(G buf[])
    {
        for (int i = 0; i < iTm; i++)
        {
            buf[i] = G(0);
        }
    }
    // Reset weight buffer
    void w_buf_reset(W buf[iTn][iK_max*iK_max])
    {
        for(int k = 0; k < iK_max*iK_max; k++) {
            for (int i = 0; i < iTn; i++) {
                buf[i][k] = W(0);
            }
        }
    }
    // Reset bias buffer
    void b_buf_reset(W buf[])
    {
        for (int i = 0; i < iTm; i++)
        {
            buf[i] = W(0);
        }
    }
    ////-----------------------------Accelerator Functions---------------------------------------////
    // Load bias data
    void b_buf_load(W buf[], W *fc_layer_bias, int fc_b_offset, int m, int M)
    {
        for (int i = m; i < m + iTm && i < M; i++)
        {
            buf[i - m] = *(fc_layer_bias + fc_b_offset + i);
        }
#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        ofstream fc_b;
        fc_b.open("fc_in_bias_mbuf.txt", ios::app);
        fc_b << "fc_b: " << endl;
        for (int i = 0; i < iTm; i++)
        {
            fc_b << buf[i] << " ";
        }
        fc_b << endl;
        fc_b.close();
#endif
#endif
    }

    // Load input data
    void in_buf_load(T buf[iTn][iK_max*iK_max],
                     Itf *fc_in_data,
                     int fc_i_offset, int n, int N, int K, int R_IN, int C_IN)
    {
        for (int j = 0; j < K; j++)
        {
            for (int k = 0; k < K; k++)
            {
                for (int i = n; i < n + iTn && i < N; i += 1)
                {
#pragma HLS PIPELINE
                    if (i > N)
                    {
                        buf[(i - n)][j*K + k] = T(0);
                    } else {
                        buf[(i - n)][j * K + k] = *(fc_in_data + fc_i_offset + i * R_IN * C_IN +
                                                             j * C_IN + k);
                    }
                }
            }
        }
    }
    void in_buf_load_512(T buf[iTn][iK_max*iK_max],
                     Itf *fc_in_data,
                     int fc_i_offset, int n, int N, int K, int R_IN, int C_IN) {
        Itf data_tmp = 0;
        for (int j = 0; j < K; j++) {
            for (int k = 0; k < K; k++) {
                for (int i = 0; i < iTn; i += 32) {
                    data_tmp = *(fc_in_data + fc_i_offset + (i+n)/32*R_IN*C_IN + j*C_IN + k);
                    if (i > N) {
                        for (int wr = 0; wr < iTn && wr < 32; wr++) {
                            buf[wr][j * K + k] = T(0);
                        }
                    } else {
                            for (int wr = 0; wr < iTn && wr < 32; wr++) {
                            buf[wr][j * K + k].range(15,0) = data_tmp.range((n+wr+1)*16-1, (n+wr)*16);
                        }
                    }
                }
            }
        }
    }

    // Load weights to weight buffer
    void w_buf_load_512(W buf[][iK_max*iK_max],
                    Itf *fc_layer_weights,
                    int fc_w_offset,
                    int n, int N, int m, int M, int K) {
        Itf w_tmp = 0;
        for (int k1 = 0; k1 < K; k1++) {
            for (int k2 = 0; k2 < K; k2++) {
#pragma HLS PIPELINE
                for (int j = 0; j < iTn && j < N - n; j++) {
                    w_tmp = *(fc_layer_weights + fc_w_offset + (j + n)* N * K * K +  m * K * K + k1 * K + k2);
                    for (int wr = 0; wr < iTn && wr < 32; wr++){
                        buf[wr][k1*K + k2].range(15, 0) = w_tmp.range((wr + 1) * 16 - 1, wr * 16);
                    }
                }
            }
        }
    }

    // Convolution computation kernel
    void fc_engine(T in_buf[][iK_max*iK_max],
                   W w_buf[][iK_max*iK_max],
                   W b_buf[],
                   G out_buf[],
                   int n,
                   int K) {
        G tmp[iTn];
        for (int k1 = 0; k1 < K; k1++) {
            for (int k2 = 0; k2 < K; k2++) {
#pragma HLS PIPELINE
                for (int tm = 0; tm < iTm; tm++) {
#pragma HLS UNROLL
                    for (int tn = 0; tn < iTn; tn++) {
#pragma HLS UNROLL
                        if (tn == 0 && n == 0)
                            out_buf[tm] = b_buf[tm] + w_buf[tn][k1 * iK_max + k2] * in_buf[tn][k1 * iK_max + k2];
                        else
                            out_buf[tm] = out_buf[tm] + w_buf[tn][k1 * iK_max + k2] * in_buf[tn][k1 * iK_max + k2];
                    }
                }
            }
        }
    }

    // Ouput out_buf data to output interface
    void output_res(G out_buf[],
                    Itf *fc_o_data,
                    int fc_o_offset, int n, int m, int N, int M, bool act) {
        Itf out_tmp = 0;
        G tmp_outbuf = 0;
        if (n >= N - iTn) {
            for (int i = 0; i < iTm; i += 32) {
#pragma HLS PIPELINE
                for (int wr = 0; wr < (Tm<32?Tm:32); wr++) {
                    if (i + wr < M) {
                        tmp_outbuf = RELU(out_tmp[wr]);
//                        *(fc_o_data + fc_o_offset + i) = relu(out_buf[i - m]);
                    } else {
//                        *(fc_o_data + fc_o_offset + i) = out_buf[i - m];
                        tmp_outbuf.range(15, 0) = 0;
                    }
                    out_tmp.range(16*(m+wr+1)-1, 16*(m+wr)) = tmp_outbuf.range(15,0);
                }
                *(fc_o_data + fc_o_offset + ((i+m)/32)) = out_tmp;
            }
        }
    }


#if _LAYER_MODE_ // layer function with manual double buffer -- worked
    void fc_layer_acc_mbuf(
        int N,               //input feature number
        int M,               // output feature number
        int R_IN,
        int C_IN,
        int K,
        bool act,            // activation function bit (1-- with act, 0--without act)
        Itf *fc_layer_weights, //w[M][N][K][K]
        W *fc_layer_bias,    // b[M]
        int fc_w_offset,
        int fc_b_offset,
        int fc_i_offset,
        int fc_o_offset,
        Itf *fc_i_data, // in_data[N][(R-1)*S + K][(C-1)*S + K] --> [N][(R-1)*S + K - 2*P][(C-1)*S + K - 2*P]
        Itf *fc_o_data)
    { // out[M][R][C]

        /***************local data buffer groups******************************/
        T in_buf_0[iTn][iK_max * iK_max];
        W w_buf_0[iTn][iK_max * iK_max];
        W b_buf_0[iTm];
        G out_buf_0[iTm];

        T in_buf_1[iTn * iK_max * iK_max];
        W w_buf_1[iTn * iK_max * iK_max];
        W b_buf_1[iTm];
        G out_buf_1[iTm];

#if _HLS_MODE_
#pragma HLS ARRAY_PARTITION variable = in_buf_0 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = w_buf_0 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = b_buf_0 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = out_buf_0 complete dim = 1
#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        cout << "Starting fc acc manual double buffer test ...." << endl;
        out_buf_reset(out_buf_0);
        b_buf_reset(b_buf_0);
        w_buf_reset(w_buf_0);
        cout << "Local buffer reset finised ..." << endl;
#endif
#endif

        for (int m = 0; m < M; m += iTm)
        {

            for (int n = 0; n < N; n += iTn)
            {
//--------------------------Load input B W D in ping-pong manner-------------------------//
                b_buf_load(b_buf_0, fc_layer_bias, fc_b_offset, m, M);
                w_buf_load(w_buf_0, fc_layer_weights, fc_w_offset, n, N, m, M, K);
                in_buf_load(in_buf_0, fc_i_data, fc_i_offset, n, N, K, R_IN, C_IN);
//------------------------------compute buffered data -----------------------------------//
                fc_engine(in_buf_0, w_buf_0, b_buf_0, out_buf_0, n, K);
//---------------------------transfer output data----------------------------------------//
                output_res(out_buf_0, fc_o_data, fc_o_offset, n, m, N, M, act);
            }
        }
    };

};

#endif