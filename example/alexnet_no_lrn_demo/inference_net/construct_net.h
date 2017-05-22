//
// Created by yaochen on 24/12/16.
//

#ifndef _CONSTRUCT_NET_H_
#define _CONSTRUCT_NET_H_


#include <iostream>
#include <ap_fixed.h>

#include "config.h"
#include "conv_layer_one_dim_1.h"
#include "conv_layer_one_dim_2.h"
#include "conv_layer_one_dim_3.h"
#include "conv_layer_one_dim_4.h"
#include "conv_layer_one_dim_5.h"
#include "pool_layer_one_dim.h"
#include "fc_layer_one_dim.h"

using namespace std;

// caffenet inference function
void inference_net(
    char activation_type,

    // input pic data
    data_type in_data_3D[3*32*32],

    // layer weights and bias inputs
    data_type_w *conv_weight_port,
    data_type_w *conv_bias_port,
    data_type_w *fc_weight_port,
    data_type_w *fc_bias_port,

    // output fc data
    data_type_o fc_8_out_a[10*1*1],

    // temp output port
    data_type_o  output_temp_1[96*32*32],
    data_type_o  output_temp_2[96*32*32]
) {

#if _HLS_MODE_

#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
#pragma HLS INTERFACE s_axilite port=activation_type bundle=CRTL_BUS

#pragma HLS INTERFACE m_axi depth=3072 port=in_data_3D

#pragma HLS INTERFACE m_axi depth=2103840  port=conv_weight_port
#pragma HLS INTERFACE m_axi depth=1376  port=conv_bias_port
#pragma HLS INTERFACE m_axi depth=17866752  port=fc_weight_port
#pragma HLS INTERFACE m_axi depth=8202  port=fc_bias_port

#pragma HLS INTERFACE m_axi depth=10  port=fc_8_out_a
#pragma HLS INTERFACE m_axi depth=24576  port=output_temp_1
#pragma HLS INTERFACE m_axi depth=24576  port=output_temp_2

#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
	cout << "starting forward network process..........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif

	/******************************************************************************************/

	//construct network ----- caffenet
//	conv_pool_layer<data_type,data_type_w,data_type_o, 32, 3, 1, 1, 2, 0, 2, 3,  96,  1> C1P1;
    conv_layer_1<data_type, data_type_w, data_type_o, 32, 3, 1, 1, 3, 96, 1> C1;
    pool_layer<data_type, data_type_w, data_type_o, 32, 2, 0, 2, 96> P1;
//	conv_pool_layer<data_type,data_type_w,data_type_o, 16, 3, 1, 1, 2, 0, 2, 96, 256, 2> C2P2;
    conv_layer_2<data_type, data_type_w, data_type_o, 16, 3, 1, 1, 96, 256, 2> C2;
    pool_layer<data_type, data_type_w, data_type_o, 16, 2, 0, 2, 256> P2;
	conv_layer_3<data_type,data_type_w,data_type_o, 8, 3, 0, 1, 256, 384, 1> C3;
	conv_layer_4<data_type,data_type_w,data_type_o, 6, 3, 0, 1, 384, 384, 2> C4;
	conv_layer_5<data_type,data_type_w,data_type_o, 4, 3, 0, 1, 384, 256, 2> C5;
	pool_layer<data_type,data_type_w,data_type_o, 2, 2, 0, 2, 256> P5;
	fc_layer<data_type,data_type_w,data_type_o, 256, 1, 4096> F6;
	fc_layer<data_type,data_type_w,data_type_o, 4096, 1, 4096> F7;
	fc_layer<data_type,data_type_w,data_type_o, 4096, 1, 10> F8;

	//temp storage space
	data_type_o in_data_buf[3*32*32];
	data_type_o fc_8_out_buf[10];
	data_type_o local_temp_1[96*32*32];
    data_type_o local_temp_2[96*32*32];
    data_type_w weight_buffer[3*96*11*11];
    data_type_w bias_buffer[4096];


    //internal memory initiallization
    for(int addr = 0; addr < 96*32*32; addr++){
#pragma HLS PIPELINE
        local_temp_1[addr] = data_type_o(0);
        local_temp_2[addr] = data_type_o(0);
    }
	for(int i = 0; i < 3; i++){
	    for(int j = 0; j < 32; j++){
		    for(int k = 0; k < 32; k++){
#pragma HLS PIPELINE
		        in_data_buf[i*32*32 + j*32 + k] = in_data_3D[i*32*32 + j*32 + k];
	        }
	    }
	}
    for(int i = 0; i < 288*11*11; i++){
//        for(int j = 0; j < 11; j++){
//            for(int k = 0; k < 11; k++){
        weight_buffer[i] = *(conv_weight_port + i);
//            }
//        }
    }

	//Forward propagation by layer
//	C1P1.conv_layer_w_pool_a(activation_type, in_data_buf, conv_weight_port, conv_bias_port, output_temp_1);
    C1.conv_layer_a(activation_type, in_data_buf, weight_buffer, conv_bias_port, local_temp_1);
    P1.max_pooling_layer_a(activation_type, local_temp_1, local_temp_2);
    for(int addr = 0; addr < 96*32*32; addr++){
//#pragma HLS PIPELINE
        local_temp_1[addr] = data_type_o(0);
    }
//    C2P2.conv_layer_w_pool_a(activation_type, output_temp_1, conv_weight_port+288*3*3, conv_bias_port+96, local_temp_2);
    C2.conv_layer_a(activation_type, local_temp_2, conv_weight_port, conv_bias_port, local_temp_1);
    for(int addr = 0; addr < 96*32*32; addr++){
//#pragma HLS PIPELINE
        local_temp_2[addr] = data_type_o(0);
    }
    P1.max_pooling_layer_a(activation_type, local_temp_1, local_temp_2);
   	for(int addr = 0; addr < 96*32*32; addr++){
//#pragma HLS PIPELINE
        local_temp_1[addr] = data_type_o(0);
   	}
	C3.conv_layer_a(activation_type, local_temp_2, conv_weight_port+288*3*3+12288*3*3, conv_bias_port+96+256, local_temp_1);
    for(int addr = 0; addr < 96*32*32; addr++){
//#pragma HLS PIPELINE
        local_temp_2[addr] = data_type_o(0);
  	}
	C4.conv_layer_a(activation_type, local_temp_1, conv_weight_port+288*3*3+12288*3*3+98304*3*3, conv_bias_port+96+256+384, local_temp_2);
  	for(int addr = 0; addr < 96*32*32; addr++){
//#pragma HLS PIPELINE
        local_temp_1[addr] = data_type_o(0);
   	}
	C5.conv_layer_a(activation_type, local_temp_2, conv_weight_port+288*3*3+12288*3*3+98304*3*3+73728*3*3, conv_bias_port+96+256+384+384, local_temp_1);
   	for(int addr = 0; addr < 96*32*32; addr++){
//#pragma HLS PIPELINE
        local_temp_2[addr] = data_type_o(0);
   	}
	P5.max_pooling_layer_a(activation_type, local_temp_1, local_temp_2);
   	for(int addr = 0; addr < 96*32*32; addr++){
//#pragma HLS PIPELINE
        local_temp_1[addr] = data_type_o(0);
   	}
	F6.fc_layer_a(activation_type, local_temp_2, fc_weight_port, fc_bias_port, local_temp_1);
   	for(int addr = 0; addr < 96*32*32; addr++){
//#pragma HLS PIPELINE
        local_temp_2[addr] = data_type_o(0);
   	}
	F7.fc_layer_a(activation_type, local_temp_1, fc_weight_port+1048576*1*1, fc_bias_port+4096, local_temp_2);
   	for(int addr = 0; addr < 96*32*32; addr++){
//#pragma HLS PIPELINE
       	local_temp_1[addr] = data_type_o(0);
   	}
	F8.fc_layer_a_no_activation(local_temp_2, fc_weight_port+1048576*1*1+16777216*1*1, fc_bias_port+4096+4096, fc_8_out_buf);

	for(int i = 0; i < 10; i++){
//#pragma HLS PIPELINE
	    fc_8_out_a[i] = fc_8_out_buf[i];
	}
	/******************************************************************************************/


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
	cout << "Finished forward network process .........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif

}

#endif //_CONSTRUCT_NET_H_
