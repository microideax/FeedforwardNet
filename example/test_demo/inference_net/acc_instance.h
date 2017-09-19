#ifndef _ACC_INSTANCE_H_
#define _ACC_INSTANCE_H_

#include "conv_acc_innerdf_1.h"
#include "max_pool_acc_innerdf.h"
#include "ave_pool_acc_innerdf.h"
#include "config.h"

conv_acc<data_type, data_type_w, data_type_o, 16, 8, 28, 28, 4, 5> convAcc1;

void conv_layer_new(
   int N,
   int K,
   int M,
   int R_IN,
   int C_IN,
   int C_OUT,
   int R_OUT,
   int S,
   int P,
   bool act,
   data_type_w *layer_weights,
   data_type_w *layer_bias,
#if _BATCH_NORM_
   data_type_w *bn_mean,
   data_type_w *bn_denominator,
#endif
#if _SCALE_
   data_type_w *scale_gamma,
   data_type_w *scale_beta,
#endif
   int weight_offset,
   int bias_offset,
   int in_offset,
   int out_offset,
   data_type *in_data_1,
   data_type *in_data_2,
   data_type *in_data_3,
   data_type *in_data_4,
   data_type_o *out_data_1,
   data_type_o *out_data_2,
   data_type_o *out_data_3,
   data_type_o *out_data_4) {

   convAcc1.conv_layer_acc(N, K, M, R_IN, C_IN, C_OUT, R_OUT, S, P, act, layer_weights, layer_bias, 
#if _BATCH_NORM_
   bn_mean, bn_denominator, 
#endif
#if _SCALE_
   scale_gamma, scale_beta, 
#endif
   weight_offset, bias_offset, in_offset, out_offset, in_data_1, in_data_2, in_data_3, in_data_4, out_data_1, out_data_2, out_data_3, out_data_4);

}

max_pool_acc<data_type, data_type_w, data_type_o, 16, 28, 28, 2, 3> maxPoolAcc1;

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
   data_type *in_data_1,
   data_type *in_data_2,
   data_type *in_data_3,
   data_type *in_data_4,
   data_type_o *out_data_1,
   data_type_o *out_data_2,
   data_type_o *out_data_3,
   data_type_o *out_data_4) {

   maxPoolAcc1.max_pool_layer_acc(R_in, C_in, N, K, R, C, S, P, act, in_data_1, in_data_2, in_data_3, in_data_4, out_data_1, out_data_2, out_data_3, out_data_4);

}

ave_pool_acc<data_type, data_type_w, data_type_o, 16, 28, 28, 2, 3> avePoolAcc1;

void ave_pool_layer_new(
   int R_in,
   int C_in,
   int N,
   int K,
   int R,
   int C,
   int S,
   int P,
   bool act,
   data_type *in_data_1,
   data_type *in_data_2,
   data_type *in_data_3,
   data_type *in_data_4,
   data_type_o *out_data_1,
   data_type_o *out_data_2,
   data_type_o *out_data_3,
   data_type_o *out_data_4) {

   avePoolAcc1.ave_pool_layer_acc(R_in, C_in, N, K, R, C, S, P, act, in_data_1, in_data_2, in_data_3, in_data_4, out_data_1, out_data_2, out_data_3, out_data_4);

}



#endif
