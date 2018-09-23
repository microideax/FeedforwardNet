#ifndef _ACC_INSTANCE_H_
#define _ACC_INSTANCE_H_

#include "conv_acc_break.h"
#include "conv_acc_innerpp.h"
#include "conv_acc_innerpp_1.h"
#include "conv_acc_innerpp_2.h"
#include "conv_acc_innerpp_3.h"
#include "conv_acc_innerpp_4.h"
#include "max_pool_acc_innerpp.h"
#include "max_pool_acc_innerpp_1.h"
//#include "max_pool_acc_innerpp_2.h"
//#include "max_pool_acc_innerpp_3.h"
//#include "conv_acc_break_noact.h"
//#include "max_pool_acc_noact.h"
#include "fc_acc_conv.h"
#include "config.h"

using namespace std;

/*
conv_acc<data_type, data_type_w, data_type_o, 132, 10, 28, 28, 7, 11> convAcc2;
void conv_layer_new(
   int N,
   int K,
   int M,
   int R,
   int C,
   int S,
   int P,
   bool act,
   data_type *in_data,
   data_type_w *layer_weights,
   data_type_w *layer_bias,
   data_type_o *out_data,
   int weight_offset,
   int bias_offset,
   int in_offset,
   int out_offset) {
   convAcc2.conv_layer_acc(N, K, M, R, C, S, P, act, in_data, layer_weights, layer_bias, out_data, weight_offset, bias_offset, in_offset, out_offset);
} */


//----------------------------------------inf_net_0 accelerator definitions ---------------------------//
conv_acc_0<data_type_itf, data_type, data_type_w, data_type_o, 96, 3, 28, 28, 4, 11, 32, 32, 32> conv_0;
// use this accelerator to process conv_layer_1_1
void conv_layer_acc_0(
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
        data_type_itf layer_weights_0[4536],
        data_type_itf layer_weights_1[4536],
        data_type_itf layer_weights_2[4536],
        data_type_itf layer_weights_3[4536],
        data_type_itf layer_weights_4[4536],
        data_type_itf layer_weights_5[4536],
        data_type_itf layer_weights_6[4536],
        data_type_itf layer_weights_7[4536],
        data_type_itf layer_bias[96],
        const int weight_offset,
        const int bias_offset,
        const int in_offset,
        const int out_offset,
        data_type_itf i_data_0[227*227],
        data_type_itf i_data_1[227*227],
        data_type_itf i_data_2[227*227],
        data_type_itf i_data_3[227*227],
        data_type_itf i_data_4[227*227],
        data_type_itf i_data_5[227*227],
        data_type_itf i_data_6[227*227],
        data_type_itf i_data_7[227*227],
        data_type_o out_data_0[55*55*12],
        data_type_o out_data_1[55*55*12],
        data_type_o out_data_2[55*55*12],
        data_type_o out_data_3[55*55*12],
        data_type_o out_data_4[55*55*12],
        data_type_o out_data_5[55*55*12],
        data_type_o out_data_6[55*55*12],
        data_type_o out_data_7[55*55*12]
) {
    conv_0.conv_layer_acc_mbuf(N, K, M, R_IN, C_IN, C_OUT, R_OUT, S, P, act,
                                 layer_weights_0, layer_weights_1, layer_weights_2, layer_weights_3,
                                 layer_weights_4, layer_weights_5, layer_weights_6, layer_weights_7,
                                 layer_bias, weight_offset,
                                 bias_offset, in_offset, out_offset,
                                 i_data_0, i_data_1, i_data_2, i_data_3, i_data_4, i_data_5, i_data_6, i_data_7,
                                 out_data_0, out_data_1, out_data_2, out_data_3, out_data_4, out_data_5, out_data_6, out_data_7);
};

/*
max_pool_acc_noact<data_type, data_type_w, data_type_o, 16, 16, 16, 2, 3> maxPoolAccNoact1;
void max_pool_layer_new_noact(
   int R_in,
   int C_in,
   int N,
   int K,
   int R,
   int C,
   int S,
   int P,
   data_type *in_data,
   data_type_o *out_data) {

   maxPoolAccNoact1.max_pool_layer_acc_noact(R_in, C_in, N, K, R, C, S, P, in_data, out_data);
}
*/

 max_pool_acc_1<data_type, data_type_w, data_type_o, 32, 27, 27, 2, 3> maxPool_1;
 void max_pool_layer_acc_1(
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
 
     maxPool_1.max_pool_layer_mbuf(R_in, C_in, N, K, R, C, S, P, act,
                       i_data_0, i_data_1, i_data_2, i_data_3, i_data_4, i_data_5, i_data_6, i_data_7,
                       o_data_0, o_data_1, o_data_2, o_data_3, o_data_4, o_data_5, o_data_6, o_data_7); 
};

conv_acc_1<data_type_itf, data_type, data_type_w, data_type_o, 128, 7, 27, 27, 1, 5, 32, 32, 32> conv_1;
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
        data_type_itf layer_weights_0[4536],
        data_type_itf layer_weights_1[4536],
        data_type_itf layer_weights_2[4536],
        data_type_itf layer_weights_3[4536],
        data_type_itf layer_weights_4[4536],
        data_type_itf layer_weights_5[4536],
        data_type_itf layer_weights_6[4536],
        data_type_itf layer_weights_7[4536],
        data_type_itf layer_bias[96],
        const int weight_offset,
        const int bias_offset,
        const int in_offset,
        const int out_offset,
        data_type i_data_0[27*27*12],
        data_type i_data_1[27*27*12],
        data_type i_data_2[27*27*12],
        data_type i_data_3[27*27*12],
        data_type i_data_4[27*27*12],
        data_type i_data_5[27*27*12],
        data_type i_data_6[27*27*12],
        data_type i_data_7[27*27*12],
        data_type_o out_data_0[27*27*32],
        data_type_o out_data_1[27*27*32],
        data_type_o out_data_2[27*27*32],
        data_type_o out_data_3[27*27*32],
        data_type_o out_data_4[27*27*32],
        data_type_o out_data_5[27*27*32],
        data_type_o out_data_6[27*27*32],
        data_type_o out_data_7[27*27*32]
) {
    conv_1.conv_layer_acc_mbuf(N, K, M, R_IN, C_IN, C_OUT, R_OUT, S, P, act,
                               layer_weights_0, layer_weights_1, layer_weights_2, layer_weights_3,
                               layer_weights_4, layer_weights_5, layer_weights_6, layer_weights_7,
                               layer_bias, weight_offset,
                               bias_offset, in_offset, out_offset,
                               i_data_0, i_data_1, i_data_2, i_data_3, i_data_4, i_data_5, i_data_6, i_data_7,
                               out_data_0, out_data_1, out_data_2, out_data_3, out_data_4, out_data_5, out_data_6, out_data_7);
};


max_pool_acc_2<data_type_itf, data_type, data_type_w, data_type_o, 32, 13, 13, 2, 3> maxPool_2;
void max_pool_layer_acc_2(
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
        data_type_itf *o_data_0,
        data_type_itf *o_data_1,
        data_type_itf *o_data_2,
        data_type_itf *o_data_3,
        data_type_itf *o_data_4,
        data_type_itf *o_data_5,
        data_type_itf *o_data_6,
        data_type_itf *o_data_7) {

    maxPool_2.max_pool_layer_mbuf(R_in, C_in, N, K, R, C, S, P, act,
                                  i_data_0, i_data_1, i_data_2, i_data_3, i_data_4, i_data_5, i_data_6, i_data_7,
                                  o_data_0, o_data_1, o_data_2, o_data_3, o_data_4, o_data_5, o_data_6, o_data_7);
};


//----------------------------------------inf_net_1 accelerator definitions ---------------------------//
conv_acc_2<data_type_itf, data_type, data_type_w, data_type_o, 128, 7, 13, 13, 1, 3, 32, 32, 32> conv_2;
// use this accelerator to process conv_layer_1_1
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
        data_type_itf layer_weights_0[384*48*9],
        data_type_itf layer_weights_1[384*48*9],
        data_type_itf layer_weights_2[384*48*9],
        data_type_itf layer_weights_3[384*48*9],
        data_type_itf layer_weights_4[384*48*9],
        data_type_itf layer_weights_5[384*48*9],
        data_type_itf layer_weights_6[384*48*9],
        data_type_itf layer_weights_7[384*48*9],
        data_type_itf layer_bias[384],
        const int weight_offset,
        const int bias_offset,
        const int in_offset,
        const int out_offset,
        data_type_itf i_data_0[13*13*32],
        data_type_itf i_data_1[13*13*32],
        data_type_itf i_data_2[13*13*32],
        data_type_itf i_data_3[13*13*32],
        data_type_itf i_data_4[13*13*32],
        data_type_itf i_data_5[13*13*32],
        data_type_itf i_data_6[13*13*32],
        data_type_itf i_data_7[13*13*32],
        data_type_o out_data_0[13*13*48],
        data_type_o out_data_1[13*13*48],
        data_type_o out_data_2[13*13*48],
        data_type_o out_data_3[13*13*48],
        data_type_o out_data_4[13*13*48],
        data_type_o out_data_5[13*13*48],
        data_type_o out_data_6[13*13*48],
        data_type_o out_data_7[13*13*48]
) {
    conv_2.conv_layer_acc_mbuf(N, K, M, R_IN, C_IN, C_OUT, R_OUT, S, P, act,
                               layer_weights_0, layer_weights_1, layer_weights_2, layer_weights_3,
                               layer_weights_4, layer_weights_5, layer_weights_6, layer_weights_7,
                               layer_bias, weight_offset,
                               bias_offset, in_offset, out_offset,
                               i_data_0, i_data_1, i_data_2, i_data_3, i_data_4, i_data_5, i_data_6, i_data_7,
                               out_data_0, out_data_1, out_data_2, out_data_3, out_data_4, out_data_5, out_data_6, out_data_7);
};

conv_acc_3<data_type_itf, data_type, data_type_w, data_type_o, 192, 7, 13, 13, 1, 3, 32, 32, 32> conv_3;
// use this accelerator to process conv_layer_1_1
void conv_layer_acc_3(
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
        data_type_itf layer_weights_0[384*48*9],
        data_type_itf layer_weights_1[384*48*9],
        data_type_itf layer_weights_2[384*48*9],
        data_type_itf layer_weights_3[384*48*9],
        data_type_itf layer_weights_4[384*48*9],
        data_type_itf layer_weights_5[384*48*9],
        data_type_itf layer_weights_6[384*48*9],
        data_type_itf layer_weights_7[384*48*9],
        data_type_itf layer_bias[384],
        const int weight_offset,
        const int bias_offset,
        const int in_offset,
        const int out_offset,
        data_type i_data_0[13*13*48],
        data_type i_data_1[13*13*48],
        data_type i_data_2[13*13*48],
        data_type i_data_3[13*13*48],
        data_type i_data_4[13*13*48],
        data_type i_data_5[13*13*48],
        data_type i_data_6[13*13*48],
        data_type i_data_7[13*13*48],
        data_type_itf out_data_0[13*13*48],
        data_type_itf out_data_1[13*13*48],
        data_type_itf out_data_2[13*13*48],
        data_type_itf out_data_3[13*13*48],
        data_type_itf out_data_4[13*13*48],
        data_type_itf out_data_5[13*13*48],
        data_type_itf out_data_6[13*13*48],
        data_type_itf out_data_7[13*13*48]
) {
    conv_3.conv_layer_acc_mbuf(N, K, M, R_IN, C_IN, C_OUT, R_OUT, S, P, act,
                               layer_weights_0, layer_weights_1, layer_weights_2, layer_weights_3,
                               layer_weights_4, layer_weights_5, layer_weights_6, layer_weights_7,
                               layer_bias, weight_offset,
                               bias_offset, in_offset, out_offset,
                               i_data_0, i_data_1, i_data_2, i_data_3, i_data_4, i_data_5, i_data_6, i_data_7,
                               out_data_0, out_data_1, out_data_2, out_data_3, out_data_4, out_data_5, out_data_6, out_data_7);
};




conv_acc_4<data_type_itf, data_type, data_type_w, data_type_o, 128, 8, 13, 13, 1, 3, 32, 32, 32> conv_4;
// use this accelerator to process conv_layer_1_1
void conv_layer_acc_4(
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
        data_type_itf layer_weights_0[384*48*9],
        data_type_itf layer_weights_1[384*48*9],
        data_type_itf layer_weights_2[384*48*9],
        data_type_itf layer_weights_3[384*48*9],
        data_type_itf layer_weights_4[384*48*9],
        data_type_itf layer_weights_5[384*48*9],
        data_type_itf layer_weights_6[384*48*9],
        data_type_itf layer_weights_7[384*48*9],
        data_type_itf layer_bias[384],
        const int weight_offset,
        const int bias_offset,
        const int in_offset,
        const int out_offset,
        data_type_itf i_data_0[13*13*48],
        data_type_itf i_data_1[13*13*48],
        data_type_itf i_data_2[13*13*48],
        data_type_itf i_data_3[13*13*48],
        data_type_itf i_data_4[13*13*48],
        data_type_itf i_data_5[13*13*48],
        data_type_itf i_data_6[13*13*48],
        data_type_itf i_data_7[13*13*48],
        data_type_o out_data_0[13*13*48],
        data_type_o out_data_1[13*13*48],
        data_type_o out_data_2[13*13*48],
        data_type_o out_data_3[13*13*48],
        data_type_o out_data_4[13*13*48],
        data_type_o out_data_5[13*13*48],
        data_type_o out_data_6[13*13*48],
        data_type_o out_data_7[13*13*48]
) {
    conv_4.conv_layer_acc_mbuf(N, K, M, R_IN, C_IN, C_OUT, R_OUT, S, P, act,
                               layer_weights_0, layer_weights_1, layer_weights_2, layer_weights_3,
                               layer_weights_4, layer_weights_5, layer_weights_6, layer_weights_7,
                               layer_bias, weight_offset,
                               bias_offset, in_offset, out_offset,
                               i_data_0, i_data_1, i_data_2, i_data_3, i_data_4, i_data_5, i_data_6, i_data_7,
                               out_data_0, out_data_1, out_data_2, out_data_3, out_data_4, out_data_5, out_data_6, out_data_7);
};

 max_pool_acc_1<data_type, data_type_w, data_type_o, 32, 6, 6, 2, 3> maxPool_3;
void max_pool_layer_acc_3(
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

    maxPool_3.max_pool_layer_mbuf(R_in, C_in, N, K, R, C, S, P, act,
                                  i_data_0, i_data_1, i_data_2, i_data_3, i_data_4, i_data_5, i_data_6, i_data_7,
                                  o_data_0, o_data_1, o_data_2, o_data_3, o_data_4, o_data_5, o_data_6, o_data_7);
};

 fc_acc_conv<data_type_itf, data_type, data_type_w, data_type_o, 128, 8, 1, 1, 6, 6, 32, 32, 32> fcConv1;
 void fc_layer_acc_0(
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
        data_type_itf fc_layer_weights_0[7327744],
        data_type_itf fc_layer_weights_1[7327744],
        data_type_itf fc_layer_weights_2[7327744],
        data_type_itf fc_layer_weights_3[7327744],
        data_type_itf fc_layer_weights_4[7327744],
        data_type_itf fc_layer_weights_5[7327744],
        data_type_itf fc_layer_weights_6[7327744],
        data_type_itf fc_layer_weights_7[7327744],
        data_type_itf fc_layer_bias[9192],    // b[M]
        int fc_w_offset,
        int fc_b_offset,
        int fc_i_offset,
        int fc_o_offset,
        data_type i_data_0[1152], // in_data[N][(R-1)*S + K][(C-1)*S + K] --> [N][(R-1)*S + K - 2*P][(C-1)*S + K - 2*P]
        data_type i_data_1[1152],
        data_type i_data_2[1152],
        data_type i_data_3[1152],
        data_type i_data_4[1152],
        data_type i_data_5[1152],
        data_type i_data_6[1152],
        data_type i_data_7[1152],
        data_type_o fc_o_data_0[512],
        data_type_o fc_o_data_1[512],
        data_type_o fc_o_data_2[512],
        data_type_o fc_o_data_3[512],
        data_type_o fc_o_data_4[512],
        data_type_o fc_o_data_5[512],
        data_type_o fc_o_data_6[512],
        data_type_o fc_o_data_7[512]) {

    fcConv1.fc_layer_acc_mbuf(N, K, M, R_IN, C_IN, R_OUT, C_OUT, S, P, act,
                                 fc_layer_weights_0, fc_layer_weights_1, fc_layer_weights_2, fc_layer_weights_3, 
				 fc_layer_weights_4, fc_layer_weights_5, fc_layer_weights_6, fc_layer_weights_7, 
				 fc_layer_bias,
                                 fc_w_offset, fc_b_offset, fc_i_offset, fc_o_offset,
                                 i_data_0, i_data_1, i_data_2, i_data_3, i_data_4, i_data_5, i_data_6, i_data_7,
                                 fc_o_data_0, fc_o_data_1, fc_o_data_2, fc_o_data_3,
                                 fc_o_data_4, fc_o_data_5, fc_o_data_6, fc_o_data_7);
}


#endif
