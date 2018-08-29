
#ifndef _CONSTRUCT_NET_H_
#define _CONSTRUCT_NET_H_

#include "config.h"
#include <iostream>
#include <ap_fixed.h>
#include "acc_instance.h"

using namespace std;

void   inf_net_0(

   data_type_w conv_weight_port_0[2550],
   data_type_w conv_weight_port_1[2550],
   data_type_w conv_bias_port[22],

   data_type_o data_in_0[1024],
   data_type_o data_in_1[1024],
   data_type_o data_in_2[1024],
   data_type_o data_in_3[1024],
   data_type_o data_in_4[1024],
   data_type_o data_in_5[1024],
   data_type_o data_in_6[1024],
   data_type_o data_in_7[1024],

   data_type_o data_out_0[256],
   data_type_o data_out_1[256],
   data_type_o data_out_2[256],
   data_type_o data_out_3[256],
   data_type_o data_out_4[256],
   data_type_o data_out_5[256],
   data_type_o data_out_6[256],
   data_type_o data_out_7[256]){

#if _HLS_MODE_

#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS

#pragma HLS INTERFACE m_axi port=conv_weight_port_0 depth=2550
#pragma HLS INTERFACE m_axi port=conv_weight_port_1 depth=2550
#pragma HLS INTERFACE m_axi port=conv_bias_port depth=22

#pragma HLS INTERFACE m_axi port=data_in_0 depth=1024
#pragma HLS INTERFACE m_axi port=data_in_1 depth=1024
#pragma HLS INTERFACE m_axi port=data_in_2 depth=1024
#pragma HLS INTERFACE m_axi port=data_in_3 depth=1024
#pragma HLS INTERFACE m_axi port=data_in_4 depth=1024
#pragma HLS INTERFACE m_axi port=data_in_5 depth=1024
#pragma HLS INTERFACE m_axi port=data_in_6 depth=1024
#pragma HLS INTERFACE m_axi port=data_in_7 depth=1024

#pragma HLS INTERFACE bram port=data_out_0
#pragma HLS RESOURCE variable=data_out_0 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_1
#pragma HLS RESOURCE variable=data_out_1 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_2
#pragma HLS RESOURCE variable=data_out_2 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_3
#pragma HLS RESOURCE variable=data_out_3 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_4
#pragma HLS RESOURCE variable=data_out_4 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_5
#pragma HLS RESOURCE variable=data_out_5 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_6
#pragma HLS RESOURCE variable=data_out_6 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_7
#pragma HLS RESOURCE variable=data_out_7 core=RAM_1P_BRAM
#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
   cout << "Starting Network portion 0 ......................................." << endl;
#endif
#endif


    unsigned int shift_weight_conv1_1 = 0;
    unsigned int shift_bias_conv1_1 = 0;

//    data_type_o net_0_tmp[4704];
    data_type_o tmp_0[1024];
    data_type_o tmp_1[1024];
    data_type_o tmp_2[1024];
    data_type_o tmp_3[1024];
    data_type_o tmp_4[1024];
    data_type_o tmp_5[1024];
    data_type_o tmp_6[1024];
    data_type_o tmp_7[1024];

//#pragma HLS resource variable=net_0_tmp core=XPM_MEMORY uram
#pragma HLS resource variable=tmp_0 core=XPM_MEMORY uram
#pragma HLS resource variable=tmp_1 core=XPM_MEMORY uram
#pragma HLS resource variable=tmp_2 core=XPM_MEMORY uram
#pragma HLS resource variable=tmp_3 core=XPM_MEMORY uram
#pragma HLS resource variable=tmp_4 core=XPM_MEMORY uram
#pragma HLS resource variable=tmp_5 core=XPM_MEMORY uram
#pragma HLS resource variable=tmp_6 core=XPM_MEMORY uram
#pragma HLS resource variable=tmp_7 core=XPM_MEMORY uram

   conv_layer_acc_1(1, 5, 6, 28, 28, 28, 28, 1, 2, 1,
                    conv_weight_port_0, conv_weight_port_1,
                    conv_bias_port, shift_weight_conv1_1, shift_bias_conv1_1, 0, 0,
                    data_in_0, data_in_1, data_in_2, data_in_3, data_in_4, data_in_5, data_in_6, data_in_7, //input data
                    tmp_0, tmp_1, tmp_2, tmp_3, tmp_4, tmp_5, tmp_6, tmp_7); // in accelerator cache

   max_pool_layer_new(28, 28, 6, 2, 14, 14, 2, 0, 1,
                      tmp_0, tmp_1, tmp_2, tmp_3, tmp_4, tmp_5, tmp_6, tmp_7,
                      data_out_0, data_out_1, data_out_2, data_out_3, data_out_4, data_out_5, data_out_6, data_out_7);

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
   cout << "Finished network portion 0 .........................." << endl;
#endif
#endif
}

void   inf_net_1(
        data_type_w conv_weight_port_0[2400],
        data_type_w conv_weight_port_1[2400],
        data_type_w conv_bias_port[22],

        data_type_o i_data_0[256],
        data_type_o i_data_1[256],
        data_type_o i_data_2[256],
        data_type_o i_data_3[256],
        data_type_o i_data_4[256],
        data_type_o i_data_5[256],
        data_type_o i_data_6[256],
        data_type_o i_data_7[256],

        data_type_o data_out_0[64],
        data_type_o data_out_1[64],
        data_type_o data_out_2[64],
        data_type_o data_out_3[64],
        data_type_o data_out_4[64],
        data_type_o data_out_5[64],
        data_type_o data_out_6[64],
        data_type_o data_out_7[64]){

#if _HLS_MODE_

#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
#pragma HLS INTERFACE m_axi port=conv_weight_port_0 depth=2550
#pragma HLS INTERFACE m_axi port=conv_weight_port_1 depth=2550
#pragma HLS INTERFACE m_axi port=conv_bias_port depth=22

#pragma HLS INTERFACE m_axi port=i_data_0 depth=256
#pragma HLS INTERFACE m_axi port=i_data_1 depth=256
#pragma HLS INTERFACE m_axi port=i_data_2 depth=256
#pragma HLS INTERFACE m_axi port=i_data_3 depth=256
#pragma HLS INTERFACE m_axi port=i_data_4 depth=256
#pragma HLS INTERFACE m_axi port=i_data_5 depth=256
#pragma HLS INTERFACE m_axi port=i_data_6 depth=256
#pragma HLS INTERFACE m_axi port=i_data_7 depth=256

#pragma HLS INTERFACE bram port=data_out_0
#pragma HLS RESOURCE variable=data_out_0 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_1
#pragma HLS RESOURCE variable=data_out_1 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_2
#pragma HLS RESOURCE variable=data_out_2 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_3
#pragma HLS RESOURCE variable=data_out_3 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_4
#pragma HLS RESOURCE variable=data_out_4 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_5
#pragma HLS RESOURCE variable=data_out_5 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_6
#pragma HLS RESOURCE variable=data_out_6 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_7
#pragma HLS RESOURCE variable=data_out_7 core=RAM_1P_BRAM
#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    cout << "Starting Network portion 1 ......................................." << endl;
#endif
#endif

    unsigned int shift_weight_conv2_1 = 150;
    unsigned int shift_bias_conv2_1 = 6;

    data_type_o tmp_0[256];
    data_type_o tmp_1[256];
    data_type_o tmp_2[256];
    data_type_o tmp_3[256];
    data_type_o tmp_4[256];
    data_type_o tmp_5[256];
    data_type_o tmp_6[256];
    data_type_o tmp_7[256];
#pragma HLS resource core=XPM_MEMORY variable=tmp_0
#pragma HLS resource core=XPM_MEMORY variable=tmp_1
#pragma HLS resource core=XPM_MEMORY variable=tmp_2
#pragma HLS resource core=XPM_MEMORY variable=tmp_3
#pragma HLS resource core=XPM_MEMORY variable=tmp_4
#pragma HLS resource core=XPM_MEMORY variable=tmp_5
#pragma HLS resource core=XPM_MEMORY variable=tmp_6
#pragma HLS resource core=XPM_MEMORY variable=tmp_7

    conv_layer_acc_2(6, 5, 16, 14, 14, 10, 10, 1, 0, 1,
                     conv_weight_port_0, conv_weight_port_1, conv_bias_port,
                     shift_weight_conv2_1, shift_bias_conv2_1, 0, 0,
                     i_data_0, i_data_1, i_data_2, i_data_3, i_data_4, i_data_5, i_data_6, i_data_7,
                     tmp_0, tmp_1, tmp_2, tmp_3, tmp_4, tmp_5, tmp_6, tmp_7);

    max_pool_layer_new(10, 10, 16, 2, 5, 5, 2, 0, 1,
                       tmp_0, tmp_1, tmp_2, tmp_3, tmp_4, tmp_5, tmp_6, tmp_7,
                       data_out_0, data_out_1, data_out_2, data_out_3,
                       data_out_4, data_out_5, data_out_6, data_out_7);

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    cout << "Finished network portion 1 .........................." << endl;
#endif
#endif
}

/*
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
*/
#endif //_CONSTRUCT_NET_H_
