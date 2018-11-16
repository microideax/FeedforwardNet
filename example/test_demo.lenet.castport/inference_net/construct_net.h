// param_conv[] = { N, K, M, Rin, Cin, R, C, S, P, act, w_off, b_off, i_off, o_off, 0, 0}
#ifndef _CONSTRUCT_NET_H_
#define _CONSTRUCT_NET_H_

#include "config.h"
#include <iostream>
#include <ap_fixed.h>
#include "acc_instance.h"
using namespace std;

void inf_net_0(
    Tparam param_conv[16],
    Tparam param_pool[16],
    ap_fixed<32,26> bias_in[2048],
    data_type_itf weight_in[550],
    data_type_itf data_in[1024],
    data_type_itf data_out[1024]
   )
{
//4194304
//5242880
#if _HLS_MODE_
#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
#pragma HLS INTERFACE bram  port=param_conv
#pragma HLS INTERFACE bram  port=param_pool
#pragma HLS INTERFACE bram  port=bias_in depth=256
#pragma HLS INTERFACE m_axi port=weight_in depth=4096
#pragma HLS INTERFACE m_axi port=data_in  depth=1024
#pragma HLS INTERFACE m_axi port=data_out depth=1024

#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    layer_count++;
    cout << "Starting layer " << layer_count << " processing" << endl;
    cout << "LAYER ACC: Starting layer accelerator ......................................." << endl;
#endif
#endif

    data_type_itf tmp_0_0[1024];
/*
//#pragma HLS resource variable=tmp_0_0 core=XPM_MEMORY uram
//#pragma HLS resource variable=tmp_0_1 core=XPM_MEMORY uram
//#pragma HLS resource variable=tmp_0_2 core=XPM_MEMORY uram
//#pragma HLS resource variable=tmp_0_3 core=XPM_MEMORY uram
//#pragma HLS resource variable=tmp_0_4 core=XPM_MEMORY uram
//#pragma HLS resource variable=tmp_0_5 core=XPM_MEMORY uram
//#pragma HLS resource variable=tmp_0_6 core=XPM_MEMORY uram
//#pragma HLS resource variable=tmp_0_7 core=XPM_MEMORY uram
*/

    Tparam param_conv_local[16];
    Tparam param_pool_local[16];
#pragma HLS ARRAY_PARTITION variable=param_conv_local complete
#pragma HLS ARRAY_PARTITION variable=param_pool_local complete
    for (unsigned int i = 0; i < 16; i++){
#pragma HLS PIPELINE
        param_conv_local[i] = param_conv[i];
        param_pool_local[i] = param_pool[i];
    }
    if (param_pool_local[15] == 0) 
    {
        cout << "LAYER ACC: Execute conv layer without pool, enable_pool =  " << param_pool_local[15] << endl;
        conv_layer_acc_1(param_conv_local[0], // N
                         param_conv_local[1], // K
                         param_conv_local[2], // M
                         param_conv_local[3], // Rin
                         param_conv_local[4], // Cin
                         param_conv_local[5], // R
                         param_conv_local[6], // C
                         param_conv_local[7], // S
                         param_conv_local[8], // P
                         param_conv_local[9], // act
                         param_conv_local[10], // w_offset
                         param_conv_local[11], // b_offset
                         param_conv_local[12], // in_offset
                         param_conv_local[13], // out_offset
                         bias_in,
                         weight_in,
                         data_in,
                         data_out);
    } 
    else 
    {
        cout << "LAYER ACC: Execute pool layer with pooling enable: " << param_pool_local[15] << endl;
        max_pool_layer_new(param_pool_local[0], // R_in,
                           param_pool_local[1], // C_in
                           param_pool_local[2], // N
                           param_pool_local[3], // K
                           param_pool_local[4], // R
                           param_pool_local[5], // C
                           param_pool_local[6], // S
                           param_pool_local[7], // P
                           param_pool_local[8], // act
                           param_pool_local[9], // in_offset
                           param_pool_local[10], // out_offset
                           data_in,
                           data_out);
    }

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    cout << "LAYER ACC: Finished layer accelerator ......................................." << endl;
    cout << endl;
#endif
#endif
}

/*
void   inf_net_1(
        data_type_w conv_weight_port_0[2550],
        data_type_w conv_weight_port_1[2550],
        data_type_w conv_weight_port_2[2550],
        data_type_w conv_weight_port_3[2550],
        data_type_w conv_weight_port_4[2550],
        data_type_w conv_weight_port_5[2550],
        data_type_w conv_weight_port_6[2550],
        data_type_w conv_weight_port_7[2550],
        data_type_w conv_bias_port[22],

        data_type_o data_in_0[256],
        data_type_o data_in_1[256],
        data_type_o data_in_2[256],
        data_type_o data_in_3[256],
        data_type_o data_in_4[256],
        data_type_o data_in_5[256],
        data_type_o data_in_6[256],
        data_type_o data_in_7[256],

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
//#pragma HLS INTERFACE m_axi port=conv_weight_port_0 depth=2550
#pragma HLS INTERFACE bram port=conv_weight_port_0
#pragma HLS INTERFACE bram port=conv_weight_port_1
#pragma HLS INTERFACE bram port=conv_weight_port_2
#pragma HLS INTERFACE bram port=conv_weight_port_3
#pragma HLS INTERFACE bram port=conv_weight_port_4
#pragma HLS INTERFACE bram port=conv_weight_port_5
#pragma HLS INTERFACE bram port=conv_weight_port_6
#pragma HLS INTERFACE bram port=conv_weight_port_7

#pragma HLS INTERFACE m_axi port=conv_bias_port depth=22

#pragma HLS INTERFACE m_axi port=data_in_0 depth=256
#pragma HLS INTERFACE m_axi port=data_in_1 depth=256
#pragma HLS INTERFACE m_axi port=data_in_2 depth=256
#pragma HLS INTERFACE m_axi port=data_in_3 depth=256
#pragma HLS INTERFACE m_axi port=data_in_4 depth=256
#pragma HLS INTERFACE m_axi port=data_in_5 depth=256
#pragma HLS INTERFACE m_axi port=data_in_6 depth=256
#pragma HLS INTERFACE m_axi port=data_in_7 depth=256

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

    data_type_o tmp_1_0[256];
    data_type_o tmp_1_1[256];
    data_type_o tmp_1_2[256];
    data_type_o tmp_1_3[256];
    data_type_o tmp_1_4[256];
    data_type_o tmp_1_5[256];
    data_type_o tmp_1_6[256];
    data_type_o tmp_1_7[256];
#pragma HLS resource variable=tmp_1_0 core=XPM_MEMORY uram
#pragma HLS resource variable=tmp_1_1 core=XPM_MEMORY uram
#pragma HLS resource variable=tmp_1_2 core=XPM_MEMORY uram
#pragma HLS resource variable=tmp_1_3 core=XPM_MEMORY uram
#pragma HLS resource variable=tmp_1_4 core=XPM_MEMORY uram
#pragma HLS resource variable=tmp_1_5 core=XPM_MEMORY uram
#pragma HLS resource variable=tmp_1_6 core=XPM_MEMORY uram
#pragma HLS resource variable=tmp_1_7 core=XPM_MEMORY uram

    conv_layer_acc_2(6, 5, 16, 14, 14, 10, 10, 1, 0, 1,
                     conv_weight_port_0, conv_weight_port_1, conv_weight_port_2, conv_weight_port_3,
                     conv_weight_port_4, conv_weight_port_5, conv_weight_port_6, conv_weight_port_7,
                     conv_bias_port,
                     shift_weight_conv2_1, shift_bias_conv2_1, 0, 0,
                     data_in_0, data_in_1, data_in_2, data_in_3,
                     data_in_4, data_in_5, data_in_6, data_in_7,
                     tmp_1_0, tmp_1_1, tmp_1_2, tmp_1_3, tmp_1_4, tmp_1_5, tmp_1_6, tmp_1_7);

    max_pool_layer_new(10, 10, 16, 2, 5, 5, 2, 0, 1,
                       tmp_1_0, tmp_1_1, tmp_1_2, tmp_1_3, tmp_1_4, tmp_1_5, tmp_1_6, tmp_1_7,
                       data_out_0, data_out_1, data_out_2, data_out_3,
                       data_out_4, data_out_5, data_out_6, data_out_7);

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    cout << "Finished network portion 1 .........................." << endl;
#endif
#endif
}


void   inf_net_2(
        data_type_w fc_weight_port[4000],
        data_type_w fc_bias_port[10],

        data_type_o data_in_0[64],
        data_type_o data_in_1[64],
        data_type_o data_in_2[64],
        data_type_o data_in_3[64],
        data_type_o data_in_4[64],
        data_type_o data_in_5[64],
        data_type_o data_in_6[64],
        data_type_o data_in_7[64],

        data_type_o temp_out_0_4[4704]){

#if _HLS_MODE_

#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
#pragma HLS INTERFACE bram port=fc_weight_port
#pragma HLS RESOURCE variable=fc_weight_port core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=fc_bias_port
#pragma HLS RESOURCE variable=fc_bias_port core=RAM_1P_BRAM

#pragma HLS INTERFACE m_axi port=data_in_0 depth=64
#pragma HLS INTERFACE m_axi port=data_in_1 depth=64
#pragma HLS INTERFACE m_axi port=data_in_2 depth=64
#pragma HLS INTERFACE m_axi port=data_in_3 depth=64
#pragma HLS INTERFACE m_axi port=data_in_4 depth=64
#pragma HLS INTERFACE m_axi port=data_in_5 depth=64
#pragma HLS INTERFACE m_axi port=data_in_6 depth=64
#pragma HLS INTERFACE m_axi port=data_in_7 depth=64

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

    fc_layer_new(16, 5, 10, 5, 5, 1, 1, 5, 0, 1,
                 fc_weight_port, fc_bias_port, shift_weight_fc1, shift_bias_fc1, 0, 0,
                 data_in_0, data_in_1, data_in_2, data_in_3, data_in_4, data_in_5, data_in_6, data_in_7,
                 temp_out_0_4);

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    cout << "Finished network portion 2 .........................." << endl;
#endif
#endif
}*/

#endif //_CONSTRUCT_NET_H_
