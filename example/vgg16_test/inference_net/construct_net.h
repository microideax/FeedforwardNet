
#ifndef _CONSTRUCT_NET_H_
#define _CONSTRUCT_NET_H_
#include <iostream>
#include <ap_fixed.h>
#include "config.h"
#include "acc_instance.h"

using namespace std;

void   inference_net(
   char   activation_type,
   data_type   in_data_3D[3*224*224],
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
   data_type_o   fc_16_out_a[1000*1*1],
   data_type_o   temp_out_0_1[1605632],
   data_type_o   temp_out_0_2[1605632],
   data_type_o   temp_out_1_1[1605632],
   data_type_o   temp_out_1_2[1605632]){

#if _HLS_MODE_
#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
#pragma HLS INTERFACE s_axilite port=activation_type bundle=CRTL_BUS
#pragma HLS INTERFACE m_axi depth=50 port=in_data_3D
#pragma HLS INTERFACE m_axi depth=14710464 port=conv_weight_port
#pragma HLS INTERFACE m_axi depth=4224 port=conv_bias_port
#pragma HLS INTERFACE m_axi depth=123633664 port=fc_weight_port
#pragma HLS INTERFACE m_axi depth=9192 port=fc_bias_port
#pragma HLS INTERFACE m_axi depth=1000 port=fc_16_out_a
#pragma HLS INTERFACE m_axi depth=1605632 port=temp_out_0_1
#pragma HLS INTERFACE m_axi depth=1605632 port=temp_out_0_2
#pragma HLS INTERFACE m_axi depth=1605632 port=temp_out_1_1
#pragma HLS INTERFACE m_axi depth=1605632 port=temp_out_1_2
#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
   cout << "starting forward network process..........................." << endl;
   cout << "..........................................................." << endl;
#endif
#endif


   int shift_weight_conv1_1 = 0;
   int shift_bias_conv1_1 = 0;

   int shift_weight_conv2_1 = 1728;
   int shift_bias_conv2_1 = 64;

   int shift_weight_conv3_1 = 38592;
   int shift_bias_conv3_1 = 128;

   int shift_weight_conv4_1 = 112320;
   int shift_bias_conv4_1 = 256;

   int shift_weight_conv5_1 = 259776;
   int shift_bias_conv5_1 = 384;

   int shift_weight_conv6_1 = 554688;
   int shift_bias_conv6_1 = 640;

   int shift_weight_conv7_1 = 1144512;
   int shift_bias_conv7_1 = 896;

   int shift_weight_conv8_1 = 1734336;
   int shift_bias_conv8_1 = 1152;

   int shift_weight_conv9_1 = 2913984;
   int shift_bias_conv9_1 = 1664;

   int shift_weight_conv10_1 = 5273280;
   int shift_bias_conv10_1 = 2176;

   int shift_weight_conv11_1 = 7632576;
   int shift_bias_conv11_1 = 2688;

   int shift_weight_conv12_1 = 9991872;
   int shift_bias_conv12_1 = 3200;

   int shift_weight_conv13_1 = 12351168;
   int shift_bias_conv13_1 = 3712;

   int shift_weight_fc1 = 0;
   int shift_bias_fc1 = 0;

   int shift_weight_fc2 = 102760448;
   int shift_bias_fc2 = 4096;

   int shift_weight_fc3 = 119537664;
   int shift_bias_fc3 = 8192;


   conv_layer_new(3, 3, 64, 224, 224, 224, 224, 1, 1, 1, conv_weight_port, conv_bias_port, 
#if _BATCH_NORM_
   mean, denominator, 
#endif
#if _SCALE_
   gamma, beta, 
#endif
   shift_weight_conv1_1, shift_bias_conv1_1, 0, 0,  temp_out_0_1, temp_out_0_2,  temp_out_1_1, temp_out_1_2);
   conv_layer_new(64, 3, 64, 224, 224, 224, 224, 1, 1, 1, conv_weight_port, conv_bias_port, 
#if _BATCH_NORM_
   mean, denominator, 
#endif
#if _SCALE_
   gamma, beta, 
#endif
   shift_weight_conv2_1, shift_bias_conv2_1, 0, 0,  temp_out_1_1, temp_out_1_2,  temp_out_0_1, temp_out_0_2);
   max_pool_layer_new(224, 224, 64, 2, 112, 112, 2, 0, 0,  temp_out_0_1, temp_out_0_2,  temp_out_1_1, temp_out_1_2);
   conv_layer_new(64, 3, 128, 112, 112, 112, 112, 1, 1, 1, conv_weight_port, conv_bias_port, 
#if _BATCH_NORM_
   mean, denominator, 
#endif
#if _SCALE_
   gamma, beta, 
#endif
   shift_weight_conv3_1, shift_bias_conv3_1, 0, 0,  temp_out_1_1, temp_out_1_2,  temp_out_0_1, temp_out_0_2);
   conv_layer_new(128, 3, 128, 112, 112, 112, 112, 1, 1, 1, conv_weight_port, conv_bias_port, 
#if _BATCH_NORM_
   mean, denominator, 
#endif
#if _SCALE_
   gamma, beta, 
#endif
   shift_weight_conv4_1, shift_bias_conv4_1, 0, 0,  temp_out_0_1, temp_out_0_2,  temp_out_1_1, temp_out_1_2);
   max_pool_layer_new(112, 112, 128, 2, 56, 56, 2, 0, 0,  temp_out_1_1, temp_out_1_2,  temp_out_0_1, temp_out_0_2);
   conv_layer_new(128, 3, 256, 56, 56, 56, 56, 1, 1, 1, conv_weight_port, conv_bias_port, 
#if _BATCH_NORM_
   mean, denominator, 
#endif
#if _SCALE_
   gamma, beta, 
#endif
   shift_weight_conv5_1, shift_bias_conv5_1, 0, 0,  temp_out_0_1, temp_out_0_2,  temp_out_1_1, temp_out_1_2);
   conv_layer_new(256, 3, 256, 56, 56, 56, 56, 1, 1, 1, conv_weight_port, conv_bias_port, 
#if _BATCH_NORM_
   mean, denominator, 
#endif
#if _SCALE_
   gamma, beta, 
#endif
   shift_weight_conv6_1, shift_bias_conv6_1, 0, 0,  temp_out_1_1, temp_out_1_2,  temp_out_0_1, temp_out_0_2);
   conv_layer_new(256, 3, 256, 56, 56, 56, 56, 1, 1, 1, conv_weight_port, conv_bias_port, 
#if _BATCH_NORM_
   mean, denominator, 
#endif
#if _SCALE_
   gamma, beta, 
#endif
   shift_weight_conv7_1, shift_bias_conv7_1, 0, 0,  temp_out_0_1, temp_out_0_2,  temp_out_1_1, temp_out_1_2);
   max_pool_layer_new(56, 56, 256, 2, 28, 28, 2, 0, 0,  temp_out_1_1, temp_out_1_2,  temp_out_0_1, temp_out_0_2);
   conv_layer_new(256, 3, 512, 28, 28, 28, 28, 1, 1, 1, conv_weight_port, conv_bias_port, 
#if _BATCH_NORM_
   mean, denominator, 
#endif
#if _SCALE_
   gamma, beta, 
#endif
   shift_weight_conv8_1, shift_bias_conv8_1, 0, 0,  temp_out_0_1, temp_out_0_2,  temp_out_1_1, temp_out_1_2);
   conv_layer_new(512, 3, 512, 28, 28, 28, 28, 1, 1, 1, conv_weight_port, conv_bias_port, 
#if _BATCH_NORM_
   mean, denominator, 
#endif
#if _SCALE_
   gamma, beta, 
#endif
   shift_weight_conv9_1, shift_bias_conv9_1, 0, 0,  temp_out_1_1, temp_out_1_2,  temp_out_0_1, temp_out_0_2);
   conv_layer_new(512, 3, 512, 28, 28, 28, 28, 1, 1, 1, conv_weight_port, conv_bias_port, 
#if _BATCH_NORM_
   mean, denominator, 
#endif
#if _SCALE_
   gamma, beta, 
#endif
   shift_weight_conv10_1, shift_bias_conv10_1, 0, 0,  temp_out_0_1, temp_out_0_2,  temp_out_1_1, temp_out_1_2);
   max_pool_layer_new(28, 28, 512, 2, 14, 14, 2, 0, 0,  temp_out_1_1, temp_out_1_2,  temp_out_0_1, temp_out_0_2);
   conv_layer_new(512, 3, 512, 14, 14, 14, 14, 1, 1, 1, conv_weight_port, conv_bias_port, 
#if _BATCH_NORM_
   mean, denominator, 
#endif
#if _SCALE_
   gamma, beta, 
#endif
   shift_weight_conv11_1, shift_bias_conv11_1, 0, 0,  temp_out_0_1, temp_out_0_2,  temp_out_1_1, temp_out_1_2);
   conv_layer_new(512, 3, 512, 14, 14, 14, 14, 1, 1, 1, conv_weight_port, conv_bias_port, 
#if _BATCH_NORM_
   mean, denominator, 
#endif
#if _SCALE_
   gamma, beta, 
#endif
   shift_weight_conv12_1, shift_bias_conv12_1, 0, 0,  temp_out_1_1, temp_out_1_2,  temp_out_0_1, temp_out_0_2);
   conv_layer_new(512, 3, 512, 14, 14, 14, 14, 1, 1, 1, conv_weight_port, conv_bias_port, 
#if _BATCH_NORM_
   mean, denominator, 
#endif
#if _SCALE_
   gamma, beta, 
#endif
   shift_weight_conv13_1, shift_bias_conv13_1, 0, 0,  temp_out_0_1, temp_out_0_2,  temp_out_1_1, temp_out_1_2);
   max_pool_layer_new(14, 14, 512, 2, 7, 7, 2, 0, 0,  temp_out_1_1, temp_out_1_2,  temp_out_0_1, temp_out_0_2);
   conv_layer_new(512, 7, 4096, 7, 7, 1, 1, 7, 0, 1, fc_weight_port, fc_bias_port, 
#if _BATCH_NORM_
   mean, denominator, 
#endif
#if _SCALE_
   gamma, beta, 
#endif
   shift_weight_fc1, shift_bias_fc1, 0, 0,  temp_out_0_1, temp_out_0_2,  temp_out_1_1, temp_out_1_2);
   conv_layer_new(4096, 1, 4096, 1, 1, 1, 1, 1, 0, 1, fc_weight_port, fc_bias_port, 
#if _BATCH_NORM_
   mean, denominator, 
#endif
#if _SCALE_
   gamma, beta, 
#endif
   shift_weight_fc2, shift_bias_fc2, 0, 0,  temp_out_1_1, temp_out_1_2,  temp_out_0_1, temp_out_0_2);
   conv_layer_new(4096, 1, 1000, 1, 1, 1, 1, 1, 0, 0, fc_weight_port, fc_bias_port, 
#if _BATCH_NORM_
   mean, denominator, 
#endif
#if _SCALE_
   gamma, beta, 
#endif
   shift_weight_fc3, shift_bias_fc3, 0, 0,  temp_out_0_1, temp_out_0_2,  temp_out_1_1, temp_out_1_2);
   for (int i = 0; i < 1000; i += 2) {
      fc_16_out_a[i+0] = temp_out_1_1[i/2];
      fc_16_out_a[i+1] = temp_out_1_2[i/2];
      
   }


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
   cout << "Finished forward network process .........................." << endl;
   cout << "..........................................................." << endl;
#endif
#endif
}

#endif //_CONSTRUCT_NET_H_
