#ifndef _ACC_INSTANCE_H_
#define _ACC_INSTANCE_H_

// including pre-defined data type and parameters
#include "config.h"

// including accelerator templates
#include "conv_acc_innerpp.h"
#include "max_pool_acc_innerpp.h"
#include "fc_acc_conv.h"


using namespace std;

conv_acc<data_type, data_type_w, data_type_o, 8, 2, 4, 4, 5, 5, 32, 32, 32> convAcc1;

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
        data_type_w layer_weights[6550],
        data_type_w layer_bias[36],
        const int weight_offset,
        const int bias_offset,
        const int in_offset,
        const int out_offset,
        data_type in_data_1[4704],
        data_type_o out_data_1[4704]) {

    convAcc1.conv_layer_acc_mbuf(N, K, M, R_IN, C_IN, C_OUT, R_OUT, S, P, act, layer_weights, layer_bias, weight_offset,
                                 bias_offset, in_offset, out_offset, in_data_1, out_data_1);

};

conv_acc<data_type, data_type_w, data_type_o, 16, 2, 4, 4, 5, 5, 32, 32, 32> convAcc2;

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
        data_type_w layer_weights[6550],
        data_type_w layer_bias[36],
        const int weight_offset,
        const int bias_offset,
        const int in_offset,
        const int out_offset,
        data_type in_data_1[4704],
        data_type_o out_data_1[4704]) {

    convAcc2.conv_layer_acc_mbuf(N, K, M, R_IN, C_IN, C_OUT, R_OUT, S, P, act, layer_weights, layer_bias, weight_offset,
                                 bias_offset, in_offset, out_offset, in_data_1, out_data_1);

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
        data_type in_data_1[4074],
        data_type_o out_data_1[4074]) {

    maxPoolAcc1.max_pool_layer_mbuf(R_in, C_in, N, K, R, C, S, P, act, in_data_1, out_data_1);

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
        data_type fc_i_data[4074], // in_data[N][(R-1)*S + K][(C-1)*S + K] --> [N][(R-1)*S + K - 2*P][(C-1)*S + K - 2*P]
        data_type_o fc_o_data[4074]) {

    fcAccConv1.fc_layer_acc_mbuf(N, K, M, R_IN, C_IN, R_OUT, C_OUT, S, P, act, fc_layer_weights, fc_layer_bias,
                                 fc_w_offset, fc_b_offset, fc_i_offset, fc_o_offset, fc_i_data, fc_o_data);
}


#endif