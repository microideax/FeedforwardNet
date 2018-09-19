#ifndef _ACC_INSTANCE_H_
#define _ACC_INSTANCE_H_

// including pre-defined data type and parameters
#include "config.h"

// including accelerator templates
#include "conv_acc_innerpp.h"
#include "max_pool_acc_innerpp.h"
#include "fc_acc_conv.h"


using namespace std;

conv_acc<data_type, data_type_w, data_type_o, 8, 8, 28, 28, 5, 5, 32, 32, 32> convAcc1;

void conv_layer_acc_1(
        const int N,
        const int K,
        const int M,
        const int R_IN,
        const int C_IN,
        const int C_OUT,
        const int R_OUT,
        const int S,
        const int P,
        bool act,
        data_type_w layer_weights_0[2550],
        data_type_w layer_weights_1[2550],
        data_type_w layer_weights_2[2550],
        data_type_w layer_weights_3[2550],
        data_type_w layer_weights_4[2550],
        data_type_w layer_weights_5[2550],
        data_type_w layer_weights_6[2550],
        data_type_w layer_weights_7[2550],
        data_type_w layer_bias[22],
        const int weight_offset,
        const int bias_offset,
        const int in_offset,
        const int out_offset,
        data_type i_data_0[1024],
        data_type i_data_1[1024],
        data_type i_data_2[1024],
        data_type i_data_3[1024],
        data_type i_data_4[1024],
        data_type i_data_5[1024],
        data_type i_data_6[1024],
        data_type i_data_7[1024],
        data_type_o out_data_0[1024],
        data_type_o out_data_1[1024],
        data_type_o out_data_2[1024],
        data_type_o out_data_3[1024],
        data_type_o out_data_4[1024],
        data_type_o out_data_5[1024],
        data_type_o out_data_6[1024],
        data_type_o out_data_7[1024]) {

    convAcc1.conv_layer_acc_mbuf(N, K, M, R_IN, C_IN, C_OUT, R_OUT, S, P, act,
                                 layer_weights_0, layer_weights_1, layer_weights_2, layer_weights_3,
                                 layer_weights_4, layer_weights_5, layer_weights_6, layer_weights_7,
                                 layer_bias, weight_offset,
                                 bias_offset, in_offset, out_offset,
                                 i_data_0, i_data_1, i_data_2, i_data_3, i_data_4, i_data_5, i_data_6, i_data_7,
                                 out_data_0, out_data_1, out_data_2, out_data_3, out_data_4, out_data_5, out_data_6, out_data_7);

};
/*
max_pool_acc<data_type, data_type_w, data_type_o, 16, 16, 16, 2, 2> maxPoolAcc1;

void max_pool_layer_new(
        int R_in,
        int C_in,
        int N,
        int K,
        int R,
        int C,
        int S,
        int P,
        bool act,
        data_type i_data_0[1024],
        data_type i_data_1[1024],
        data_type i_data_2[1024],
        data_type i_data_3[1024],
        data_type i_data_4[1024],
        data_type i_data_5[1024],
        data_type i_data_6[1024],
        data_type i_data_7[1024],
        data_type_o o_data_0[256],
        data_type_o o_data_1[256],
        data_type_o o_data_2[256],
        data_type_o o_data_3[256],
        data_type_o o_data_4[256],
        data_type_o o_data_5[256],
        data_type_o o_data_6[256],
        data_type_o o_data_7[256]) {

    maxPoolAcc1.max_pool_layer_mbuf(R_in, C_in, N, K, R, C, S, P, act,
                                    i_data_0, i_data_1, i_data_2, i_data_3, i_data_4, i_data_5, i_data_6, i_data_7,
                                    o_data_0, o_data_1, o_data_2, o_data_3, o_data_4, o_data_5, o_data_6, o_data_7);

};
*/


conv_acc<data_type, data_type_w, data_type_o, 16, 8, 16, 16, 5, 5, 32, 32, 32> convAcc2;
void conv_layer_acc_2(
        const int N,
        const int K,
        const int M,
        const int R_IN,
        const int C_IN,
        const int C_OUT,
        const int R_OUT,
        const int S,
        const int P,
        bool act,
        data_type_w layer_weights_0[2550],
        data_type_w layer_weights_1[2550],
        data_type_w layer_weights_2[2550],
        data_type_w layer_weights_3[2550],
        data_type_w layer_weights_4[2550],
        data_type_w layer_weights_5[2550],
        data_type_w layer_weights_6[2550],
        data_type_w layer_weights_7[2550],
        data_type_w layer_bias[22],
        const int weight_offset,
        const int bias_offset,
        const int in_offset,
        const int out_offset,
        data_type i_data_0[256],
        data_type i_data_1[256],
        data_type i_data_2[256],
        data_type i_data_3[256],
        data_type i_data_4[256],
        data_type i_data_5[256],
        data_type i_data_6[256],
        data_type i_data_7[256],
        data_type_o out_data_0[256],
        data_type_o out_data_1[256],
        data_type_o out_data_2[256],
        data_type_o out_data_3[256],
        data_type_o out_data_4[256],
        data_type_o out_data_5[256],
        data_type_o out_data_6[256],
        data_type_o out_data_7[256]) {

    convAcc2.conv_layer_acc_mbuf(N, K, M, R_IN, C_IN, C_OUT, R_OUT, S, P, act,
                                 layer_weights_0, layer_weights_1, layer_weights_2, layer_weights_3,
                                 layer_weights_4, layer_weights_5, layer_weights_6, layer_weights_7,
                                 layer_bias, weight_offset,
                                 bias_offset, in_offset, out_offset,
                                 i_data_0, i_data_1, i_data_2, i_data_3, i_data_4, i_data_5, i_data_6, i_data_7,
                                 out_data_0, out_data_1, out_data_2, out_data_3,
                                 out_data_4, out_data_5, out_data_6, out_data_7);

};


max_pool_acc<data_type, data_type_w, data_type_o, 16, 16, 16, 2, 2> maxPoolAcc1;

void max_pool_layer_new(
        int R_in,
        int C_in,
        int N,
        int K,
        int R,
        int C,
        int S,
        int P,
        bool act,
        data_type *i_data_0,
        data_type *i_data_1,
        data_type *i_data_2,
        data_type *i_data_3,
        data_type *i_data_4,
        data_type *i_data_5,
        data_type *i_data_6,
        data_type *i_data_7,
        data_type_o *o_data_0,
        data_type_o *o_data_1,
        data_type_o *o_data_2,
        data_type_o *o_data_3,
        data_type_o *o_data_4,
        data_type_o *o_data_5,
        data_type_o *o_data_6,
        data_type_o *o_data_7) {

    maxPoolAcc1.max_pool_layer_mbuf(R_in, C_in, N, K, R, C, S, P, act,
                                    i_data_0, i_data_1, i_data_2, i_data_3, i_data_4, i_data_5, i_data_6, i_data_7,
                                    o_data_0, o_data_1, o_data_2, o_data_3, o_data_4, o_data_5, o_data_6, o_data_7);

};


fc_acc_conv<data_type, data_type_w, data_type_o, 16, 8, 1, 1, 5, 5, 32, 32, 32> fcAccConv1;

void fc_layer_new(
        int N,               //input feature number
        int K,               //input kernel size
        int M,               // output feature number
        int R_IN,            // input Row
        int C_IN,            // input column
        int R_OUT,           // output Row
        int C_OUT,           // output column
        int S,               // stride size
        int P,               // padding size
        bool act,            // activation function bit (1-- with act, 0--without act)
        data_type_w fc_layer_weights[4000],
        data_type_w fc_layer_bias[10],    // b[M]
        int fc_w_offset,
        int fc_b_offset,
        int fc_i_offset,
        int fc_o_offset,
        data_type i_data_0[4074], // in_data[N][(R-1)*S + K][(C-1)*S + K] --> [N][(R-1)*S + K - 2*P][(C-1)*S + K - 2*P]
        data_type i_data_1[4074],
        data_type i_data_2[4074],
        data_type i_data_3[4074],
        data_type i_data_4[4074],
        data_type i_data_5[4074],
        data_type i_data_6[4074],
        data_type i_data_7[4074],
        data_type_o fc_o_data[4074]) {

    fcAccConv1.fc_layer_acc_mbuf(N, K, M, R_IN, C_IN, R_OUT, C_OUT, S, P, act,
                                 fc_layer_weights, fc_layer_bias,
                                 fc_w_offset, fc_b_offset, fc_i_offset, fc_o_offset,
                                 i_data_0, i_data_1, i_data_2, i_data_3, i_data_4, i_data_5, i_data_6, i_data_7,
                                 fc_o_data);
}

#endif
