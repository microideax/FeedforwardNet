
#ifndef _CONSTRUCT_NET_H_
#define _CONSTRUCT_NET_H_
#include <iostream>
#include <ap_fixed.h>
#include "config.h"
#include "acc_instance.h"

using namespace std;

void   inference_net(
   char   activation_type,
   data_type   in_data_3D[3*32*32],
   data_type_w *conv_weight_port,
   data_type_w *conv_bias_port,
   data_type_w *fc_weight_port,
   data_type_w *fc_bias_port,
#if _BATCH_NORM_
   float *gamma,
   float *beta,
#endif
#if _SCALE_
   float *mean,
   float *denominator,
#endif
   data_type_o   fc_4_out_a[10*1*1],
   data_type_o   temp_out_0_1[8192],
   data_type_o   temp_out_0_2[8192],
   data_type_o   temp_out_0_3[8192],
   data_type_o   temp_out_0_4[8192],
   data_type_o   temp_out_1_1[8192],
   data_type_o   temp_out_1_2[8192],
   data_type_o   temp_out_1_3[8192],
   data_type_o   temp_out_1_4[8192]){

#if _HLS_MODE_
#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
#pragma HLS INTERFACE s_axilite port=activation_type bundle=CRTL_BUS
#pragma HLS INTERFACE m_axi depth=50 port=in_data_3D
#pragma HLS INTERFACE m_axi depth=79200 port=conv_weight_port
#pragma HLS INTERFACE m_axi depth=128 port=conv_bias_port
#pragma HLS INTERFACE m_axi depth=10240 port=fc_weight_port
#pragma HLS INTERFACE m_axi depth=10 port=fc_bias_port
#pragma HLS INTERFACE m_axi depth=10 port=fc_4_out_a
#pragma HLS INTERFACE m_axi depth=8192 port=temp_out_0_1
#pragma HLS INTERFACE m_axi depth=8192 port=temp_out_0_2
#pragma HLS INTERFACE m_axi depth=8192 port=temp_out_0_3
#pragma HLS INTERFACE m_axi depth=8192 port=temp_out_0_4
#pragma HLS INTERFACE m_axi depth=8192 port=temp_out_1_1
#pragma HLS INTERFACE m_axi depth=8192 port=temp_out_1_2
#pragma HLS INTERFACE m_axi depth=8192 port=temp_out_1_3
#pragma HLS INTERFACE m_axi depth=8192 port=temp_out_1_4
#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
   cout << "starting forward network process..........................." << endl;
   cout << "..........................................................." << endl;
#endif
#endif


   int shift_weight_conv1_1 = 0;
   int shift_bias_conv1_1 = 0;

   int shift_weight_conv2_1 = 2400;
   int shift_bias_conv2_1 = 32;

   int shift_weight_conv3_1 = 28000;
   int shift_bias_conv3_1 = 64;

   int shift_weight_fc1 = 0;
   int shift_bias_fc1 = 0;


   conv_layer_new(3, 5, 32, 32, 32, 32, 32, 1, 2, 0, conv_weight_port, conv_bias_port, 
#if _BATCH_NORM_
   mean, denominator, 
#endif
#if _SCALE_
   gamma, beta, 
#endif
   shift_weight_conv1_1, shift_bias_conv1_1, 0, 0,  temp_out_0_1, temp_out_0_2, temp_out_0_3, temp_out_0_4,  temp_out_1_1, temp_out_1_2, temp_out_1_3, temp_out_1_4);
   max_pool_layer_new(32, 32, 32, 3, 16, 16, 2, 0, 1,  temp_out_1_1, temp_out_1_2, temp_out_1_3, temp_out_1_4,  temp_out_0_1, temp_out_0_2, temp_out_0_3, temp_out_0_4);
   conv_layer_new(32, 5, 32, 16, 16, 16, 16, 1, 2, 1, conv_weight_port, conv_bias_port, 
#if _BATCH_NORM_
   mean, denominator, 
#endif
#if _SCALE_
   gamma, beta, 
#endif
   shift_weight_conv2_1, shift_bias_conv2_1, 0, 0,  temp_out_0_1, temp_out_0_2, temp_out_0_3, temp_out_0_4,  temp_out_1_1, temp_out_1_2, temp_out_1_3, temp_out_1_4);
   ave_pool_layer_new(16, 16, 32, 3, 8, 8, 2, 0, 0,  temp_out_1_1, temp_out_1_2, temp_out_1_3, temp_out_1_4,  temp_out_0_1, temp_out_0_2, temp_out_0_3, temp_out_0_4);
   conv_layer_new(32, 5, 64, 8, 8, 8, 8, 1, 2, 1, conv_weight_port, conv_bias_port, 
#if _BATCH_NORM_
   mean, denominator, 
#endif
#if _SCALE_
   gamma, beta, 
#endif
   shift_weight_conv3_1, shift_bias_conv3_1, 0, 0,  temp_out_0_1, temp_out_0_2, temp_out_0_3, temp_out_0_4,  temp_out_1_1, temp_out_1_2, temp_out_1_3, temp_out_1_4);
   ave_pool_layer_new(8, 8, 64, 3, 4, 4, 2, 0, 0,  temp_out_1_1, temp_out_1_2, temp_out_1_3, temp_out_1_4,  temp_out_0_1, temp_out_0_2, temp_out_0_3, temp_out_0_4);
   conv_layer_new(64, 4, 10, 4, 4, 1, 1, 4, 0, 0, fc_weight_port, fc_bias_port, 
#if _BATCH_NORM_
   mean, denominator, 
#endif
#if _SCALE_
   gamma, beta, 
#endif
   shift_weight_fc1, shift_bias_fc1, 0, 0,  temp_out_0_1, temp_out_0_2, temp_out_0_3, temp_out_0_4,  temp_out_1_1, temp_out_1_2, temp_out_1_3, temp_out_1_4);
   for (int i = 0; i < 10; i += 4) {
      fc_4_out_a[i+0] = temp_out_1_1[i/4];
      fc_4_out_a[i+1] = temp_out_1_2[i/4];
      fc_4_out_a[i+2] = temp_out_1_3[i/4];
      fc_4_out_a[i+3] = temp_out_1_4[i/4];
      
   }


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
   cout << "Finished forward network process .........................." << endl;
   cout << "..........................................................." << endl;
#endif
#endif
}

#endif //_CONSTRUCT_NET_H_
