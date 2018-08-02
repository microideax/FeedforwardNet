
#ifndef _CONSTRUCT_NET_H_
#define _CONSTRUCT_NET_H_

#include "config.h"
#include <iostream>
#include <ap_fixed.h>
#include "acc_instance.h"

using namespace std;

void   inference_net_0(
   data_type_w conv_weight_port[2550],
   data_type_w conv_bias_port[22],
//   data_type_o *data_in_port,
   data_type_o temp_out_0_1[4704],
   data_type_o temp_out_0_2[4704]){

#if _HLS_MODE_

#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
#pragma HLS INTERFACE bram port=conv_weight_port
#pragma HLS INTERFACE bram port=conv_bias_port
//#pragma HLS INTERFACE m_axi port=data_in_port depth=4704
#pragma HLS INTERFACE bram port=temp_out_0_1
#pragma HLS RESOURCE variable=temp_out_0_1 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=temp_out_0_2
#pragma HLS RESOURCE variable=temp_out_0_2 core=RAM_1P_BRAM

#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
   cout << "Starting Network portion 0 ......................................." << endl;
#endif
#endif


    unsigned int shift_weight_conv1_1 = 0;
    unsigned int shift_bias_conv1_1 = 0;

    data_type_o net_0_tmp[4704];
#pragma HLS resource core=XPM_MEMORY variable=net_0_tmp

   conv_layer_acc_1(1, 5, 6, 28, 28, 28, 28, 1, 2, 1, conv_weight_port, conv_bias_port, shift_weight_conv1_1, shift_bias_conv1_1, 0, 0,  temp_out_0_1,  net_0_tmp);

   max_pool_layer_new(28, 28, 6, 2, 14, 14, 2, 0, 1,  net_0_tmp,  temp_out_0_2);

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
   cout << "Finished network portion 0 .........................." << endl;
#endif
#endif
}

void   inference_net_1(
        data_type_w conv_weight_port[2550],
        data_type_w conv_bias_port[22],
        data_type_o temp_out_0_2[4704],
        data_type_o temp_out_0_3[4704]){

#if _HLS_MODE_

#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
#pragma HLS INTERFACE bram port=conv_weight_port
#pragma HLS INTERFACE bram port=conv_bias_port

#pragma HLS INTERFACE bram port=temp_out_0_2
#pragma HLS RESOURCE variable=temp_out_0_2 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=temp_out_0_3
#pragma HLS RESOURCE variable=temp_out_0_3 core=RAM_1P_BRAM

#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    cout << "Starting Network portion 1 ......................................." << endl;
#endif
#endif

    unsigned int shift_weight_conv2_1 = 150;
    unsigned int shift_bias_conv2_1 = 6;

    data_type_o net_1_tmp[4704];
#pragma HLS resource core=XPM_MEMORY variable=net_1_tmp

    conv_layer_acc_2(6, 5, 16, 14, 14, 10, 10, 1, 0, 1, conv_weight_port, conv_bias_port, shift_weight_conv2_1, shift_bias_conv2_1, 0, 0,  temp_out_0_2,  net_1_tmp);
    max_pool_layer_new(10, 10, 16, 2, 5, 5, 2, 0, 1,  net_1_tmp,  temp_out_0_3);

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    cout << "Finished network portion 1 .........................." << endl;
#endif
#endif
}

void   inference_net_2(
        data_type_w fc_weight_port[4000],
        data_type_w fc_bias_port[10],
        data_type_o temp_out_0_3[4704],
        data_type_o temp_out_0_4[4704]){

#if _HLS_MODE_

#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
#pragma HLS INTERFACE bram port=fc_weight_port
#pragma HLS RESOURCE variable=fc_weight_port core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=fc_bias_port
#pragma HLS RESOURCE variable=fc_bias_port core=RAM_1P_BRAM

#pragma HLS INTERFACE bram port=temp_out_0_3
#pragma HLS RESOURCE variable=temp_out_0_3 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=temp_out_0_4
#pragma HLS RESOURCE variable=temp_out_0_4 core=RAM_1P_BRAM

//#pragma HLS DATAFLOW

#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    cout << "Starting Network portion 2 ......................................." << endl;
#endif
#endif

    unsigned int shift_weight_fc1 = 0;
    unsigned int shift_bias_fc1 = 0;

//    data_type_o temp_out_2_tmp[4704];

    fc_layer_new(16, 5, 10, 5, 5, 1, 1, 5, 0, 1, fc_weight_port, fc_bias_port, shift_weight_fc1, shift_bias_fc1, 0, 0,  temp_out_0_3,  temp_out_0_4);

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    cout << "Finished network portion 2 .........................." << endl;
#endif
#endif
}

#endif //_CONSTRUCT_NET_H_
