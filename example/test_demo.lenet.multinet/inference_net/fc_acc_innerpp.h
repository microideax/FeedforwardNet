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

template <typename T, typename W, typename G, int iTm, int iTn, int iS_max, int iK_max>
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
    // TODO: change the data load into linebuffer mode, to avoid redundant data load
    void in_buf_load(T buf[iTn][iK_max*iK_max],
                     T *fc_in_data,
                     int fc_i_offset, int n, int N, int K, int R_IN, int C_IN)
    {
        /*
        for (int i = n; i < n + iTn && i < N; i += 1)
        {
#pragma HLS PIPELINE
            buf[i - n] = *(fc_in_data + fc_i_offset + i);
        } */

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

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        ofstream fc_in;
        fc_in.open("fc_in_data_mbuf.txt", ios::app);
        fc_in << "fc input: " << endl;
        for (int i = 0; i < iTn*iK_max*iK_max; i++)
        {
            fc_in << buf[i] << " ";
        }
        fc_in << endl;
        fc_in.close();
#endif
#endif
    }

    // Load weights to weight buffer
    void w_buf_load(W buf[][iK_max*iK_max], W *fc_layer_weights, int fc_w_offset, int n, int N, int m, int M, int K)
    {
/*
        for (int i = n; i < n + iTn && i < N; i += 1)
        {
#pragma HLS PIPELINE
            buf[i - n] = *(fc_layer_weights + fc_w_offset + i );
        }
*/
        for (int k1 = 0; k1 < K; k1++)
        {
            for (int k2 = 0; k2 < K; k2++)
            {
#pragma HLS PIPELINE
                for (int j = 0; j < iTn && j < N - n; j++)
                {
                    buf[j][k2*K + k1] = *(fc_layer_weights + fc_w_offset + (j + n)* N * K * K +  m * K * K + k1 * K + k2);
                }
            }
        }

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        ofstream fc_w;
        fc_w.open("fc_in_weight_mbuf.txt", ios::app);
        fc_w << "fc weith: " << endl;
        for (int i = 0; i < iTn * iK_max * iK_max; i++)
        {
            fc_w << buf[i] << " ";
        }
        fc_w << endl;
        fc_w.close();
#endif
#endif
    }

    // Convolution computation kernel
    void fc_engine(T in_buf[][iK_max*iK_max],
                   W w_buf[][iK_max*iK_max],
                   W b_buf[],
                   G out_buf[],
                   int n,
                   int K)
    {
        G tmp[iTn];
//#pragma HLS INLINE OFF
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
/*
        if (n == 0)
        {
            for (int tm = 0; tm < iTm; tm++) {
#pragma HLS PIPELINE
                out_buf[tm] = out_buf[tm] + b_buf[tm];
            }
        }
*/
    }

    // Ouput out_buf data to output interface
    void output_res(G out_buf[], G *fc_o_data, int fc_o_offset, int n, int m, int N, int M, bool act)
    {
        if (n >= N - iTn)
        {
            for (int i = m; i < m + iTm && i < M; i += 1)
            {
#pragma HLS PIPELINE
                if (act)
                {
                    *(fc_o_data + fc_o_offset + i) = relu(out_buf[i - m]);
                }
                else
                {
                    *(fc_o_data + fc_o_offset + i) = out_buf[i - m];
                }
            }
        }

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        ofstream out_tmp;
        out_tmp.open("out_buf.txt", ios::app);
        out_tmp << "fc out_buf: " << endl;
        for (int i = 0; i < iTm; i++)
        {
            out_tmp << out_buf[i] << " ";
        }
        out_tmp << endl;
        out_tmp.close();
#endif
#endif

    }


#if _LAYER_MODE_ // layer function with manual double buffer -- worked
    void fc_layer_acc_mbuf(
        int N,               //input feature number
        int M,               // output feature number
        int R_IN,
        int C_IN,
        int K,
        bool act,            // activation function bit (1-- with act, 0--without act)
        W *fc_layer_weights, //w[M][N][K][K]
        W *fc_layer_bias,    // b[M]
        int fc_w_offset,
        int fc_b_offset,
        int fc_i_offset,
        int fc_o_offset,
        T *fc_i_data, // in_data[N][(R-1)*S + K][(C-1)*S + K] --> [N][(R-1)*S + K - 2*P][(C-1)*S + K - 2*P]
        G *fc_o_data)
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
/*
#pragma HLS ARRAY_PARTITION variable = in_buf_1 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = w_buf_1 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = b_buf_1 complete dim = 1
#pragma HLS ARRAY_PARTITION variable = out_buf_1 complete dim = 1
*/
#pragma HLS INLINE off
#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        cout << "Starting fc acc manual double buffer test ...." << endl;
        //buffer local data initiallization: must do it in C++ debug!
        out_buf_reset(out_buf_0);
        b_buf_reset(b_buf_0);
        w_buf_reset(w_buf_0);
        out_buf_reset(out_buf_1);
        b_buf_reset(b_buf_1);
        w_buf_reset(w_buf_1);
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

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        cout << "Finished fc_acc_manual_double buffer test ...." << endl;
        ofstream fc_out;
        fc_out.open("fc_out_data_mbuf.txt", ios::app);
        fc_out << "fc output: " << endl;
        for (int i = 0; i < M; i++)
        {
            fc_out << *(fc_o_data + i) << " ";
        }
        fc_out << endl;
        fc_out.close();
#endif
#endif

    };

#endif

};

#endif