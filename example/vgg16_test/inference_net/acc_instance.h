#ifndef _ACC_INSTANCE_H_
#define _ACC_INSTANCE_H_

#include "conv_acc_innerdf_1.h"
#include "max_pool_acc_innerdf.h"
#include "config.h"

conv_acc<data_type, data_type_w, data_type_o, 64, 8, 20, 20, 7, 7> convAcc1;

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
   data_type_o *out_data_1,
   data_type_o *out_data_2) {

   convAcc1.conv_layer_acc(N, K, M, R_IN, C_IN, C_OUT, R_OUT, S, P, act, layer_weights, layer_bias, 
#if _BATCH_NORM_
   bn_mean, bn_denominator, 
#endif
#if _SCALE_
   scale_gamma, scale_beta, 
#endif
   weight_offset, bias_offset, in_offset, out_offset, in_data_1, in_data_2, out_data_1, out_data_2);

}

max_pool_acc<data_type, data_type_w, data_type_o, 64, 32, 32, 2, 2> maxPoolAcc1;

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
   data_type_o *out_data_1,
   data_type_o *out_data_2) {

   maxPoolAcc1.max_pool_layer_acc(R_in, C_in, N, K, R, C, S, P, act, in_data_1, in_data_2, out_data_1, out_data_2);

}



#endif
