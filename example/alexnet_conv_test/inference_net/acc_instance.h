//
// Created by yaochen on 23/6/17.
//

#ifndef _ACC_INSTANCE_H_
#define _ACC_INSTANCE_H_

#include "conv_acc_break.h"
#include "config.h"

conv_acc<data_type, data_type_w, data_type_o, 16, 4, 16, 16> convAcc1;//{0<Tm<=M;0<Tn<=N;0<Tr<=R;0<Tc<=C;}

void conv_layer_new(
        int N, //input feature number
        int K, //input kernel size
        int M, // output feature number
        int R, // output Row
        int C, // output column
        int S, // stride size
        int P, // padding size
        data_type *in_data, // in_data[N][(R-1)*S + K][(C-1)*S + K] --> [N][(R-1)*S + K - 2*P][(C-1)*S + K - 2*P]
        data_type_w *layer_weights, //w[M][N][K][K]
        data_type_w *layer_bias, // b[M]
        data_type_o *out_data,
        int weight_offset,
        int bias_offset) {

    convAcc1.conv_layer_acc(N, K, M, R, C, S, P, in_data, layer_weights, layer_bias, out_data, weight_offset, bias_offset);

}

#endif //FFNET_ACC_INSTANCE_H_H
