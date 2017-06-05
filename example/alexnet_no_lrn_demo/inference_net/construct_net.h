//
// Created by yaochen on 24/12/16.
//

#ifndef _CONSTRUCT_NET_H_
#define _CONSTRUCT_NET_H_


#include <iostream>
#include <ap_fixed.h>

#include "config.h"
#include "conv_layer_one_dim.h"
#include "pool_layer_one_dim.h"
#include "fc_layer_one_dim.h"
#include "conv_acc.h"

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

#pragma HLS allocation instances=conv_layer_acc limit=1 function

	/******************************************************************************************/

	//construct network ----- caffenet
    //-------------------------------conv layer 1 ----------------------------------//
    conv_layer<data_type, data_type_w, data_type_o, 32, 3, 1, 1, 3, 96, 1> C1;
    conv_acc<data_type, data_type_w, data_type_o, 32, 3, 16, 16> ACC_1;
    //-------------------------------pooling layer 1 -------------------------------//

    pool_layer<data_type, data_type_w, data_type_o, 32, 2, 0, 2, 96> P1;
/*
    //-------------------------------conv layer 2 ----------------------------------//
    conv_layer<data_type, data_type_w, data_type_o, 16, 3, 1, 1, 96, 256, 2> C2;
    //-------------------------------pooling layer 2 -------------------------------//
    pool_layer<data_type, data_type_w, data_type_o, 16, 2, 0, 2, 256> P2;
    //-------------------------------conv layer 3 ----------------------------------//
	conv_layer<data_type,data_type_w,data_type_o, 8, 3, 0, 1, 256, 384, 1> C3;
    //-------------------------------conv layer 4 ----------------------------------//
	conv_layer<data_type,data_type_w,data_type_o, 6, 3, 0, 1, 384, 384, 2> C4;
    //-------------------------------conv layer 5 ----------------------------------//
	conv_layer<data_type,data_type_w,data_type_o, 4, 3, 0, 1, 384, 256, 2> C5;
    //-------------------------------pooling layer 3 -------------------------------//
	pool_layer<data_type,data_type_w,data_type_o, 2, 2, 0, 2, 256> P5;
    //-------------------------------fc layer 6 ------------------------------------//
	fc_layer<data_type,data_type_w,data_type_o, 256, 1, 4096> F6;
    //-------------------------------fc layer 7 ------------------------------------//
	fc_layer<data_type,data_type_w,data_type_o, 4096, 1, 4096> F7;
    //-------------------------------fc layer 8 ------------------------------------//
	fc_layer<data_type,data_type_w,data_type_o, 4096, 1, 10> F8;
*/
	//temp storage space
//	data_type_o in_buf[3*32*32];
	data_type_o fc_8_out_buf[10];

//	data_type_o local_temp_1[96*32*32];
//    data_type_o local_temp_2[96*32*32];
//    data_type_o local_temp_3[96*32*32];
//    data_type_o local_temp_4[96*32*32];

//    TRANS_DATA: for(int i = 0; i < 3*32*32; i++) {
//        in_buf[i] = *(in_data_3D + i);
//    }
    //internal memory initiallization
    TEMP_RESET: for(int addr = 0; addr < 96*32*32; addr++){

//        local_temp_1[addr] = data_type_o(0);
//        local_temp_2[addr] = data_type_o(0);
        output_temp_1[addr] = data_type_o(0);
        output_temp_2[addr] = data_type_o(0);
    }

	//Forward propagation by layer

//    C1.conv_layer_a(activation_type, in_buf, conv_weight_port, conv_bias_port, local_temp_1);
    ACC_1.conv_layer_acc(3, 3, 96, 32, 32, 1, 1, in_data_3D, conv_weight_port, conv_bias_port, output_temp_1);
    /*
//    P1.max_pooling_layer_a(activation_type, local_temp_1, local_temp_2);
    P1.max_pooling_layer_a(activation_type, output_temp_1, output_temp_2);
    RESET_1: for(int addr = 0; addr < 96*32*32; addr++){
//        local_temp_1[addr] = data_type_o(0);
        output_temp_1[addr] = data_type_o(0);
    }
//    C2.conv_layer_a(activation_type, local_temp_2, conv_weight_port+288*3*3, conv_bias_port+96, local_temp_1);
    ACC_1.conv_layer_acc(96, 3, 256, 16, 16, 1, 1, output_temp_2, conv_weight_port+288*3*3, conv_bias_port+96, output_temp_1);
    RESET_2: for(int addr = 0; addr < 96*32*32; addr++){
//        local_temp_2[addr] = data_type_o(0);
        output_temp_2[addr] = data_type_o(0);
    }
//    P2.max_pooling_layer_a(activation_type, local_temp_1, local_temp_2);
    P2.max_pooling_layer_a(activation_type, output_temp_1, output_temp_2);
    RESET_3: for(int addr = 0; addr < 96*32*32; addr++){
//        local_temp_1[addr] = data_type_o(0);
        output_temp_1[addr] = data_type_o(0);
   	}
//	C3.conv_layer_a(activation_type, local_temp_2, conv_weight_port+288*3*3+12288*3*3, conv_bias_port+96+256, local_temp_1);
    ACC_1.conv_layer_acc(256, 3, 384, 6, 6, 1, 0, output_temp_2, conv_weight_port+288*3*3+12288*3*3, conv_bias_port+96+256, output_temp_1);
    RESET_4: for(int addr = 0; addr < 96*32*32; addr++){
//        local_temp_2[addr] = data_type_o(0);
        output_temp_2[addr] = data_type_o(0);
  	}
//	C4.conv_layer_a(activation_type, local_temp_1, conv_weight_port+288*3*3+12288*3*3+98304*3*3, conv_bias_port+96+256+384, local_temp_2);
    ACC_1.conv_layer_acc(384, 3, 384, 4, 4, 1, 0, output_temp_1, conv_weight_port+288*3*3+12288*3*3+98304*3*3, conv_bias_port+96+256+384, output_temp_2);
    RESET_5: for(int addr = 0; addr < 96*32*32; addr++){
//        local_temp_1[addr] = data_type_o(0);
        output_temp_1[addr] = data_type_o(0);
   	}
//	C5.conv_layer_a(activation_type, local_temp_2, conv_weight_port+288*3*3+12288*3*3+98304*3*3+73728*3*3, conv_bias_port+96+256+384+384, local_temp_1);
    ACC_1.conv_layer_acc(384, 3, 256, 2, 2, 1, 0, output_temp_2, conv_weight_port+288*3*3+12288*3*3+98304*3*3+73728*3*3, conv_bias_port+96+256+384+384, output_temp_1);
    RESET_6: for(int addr = 0; addr < 96*32*32; addr++){
//        local_temp_2[addr] = data_type_o(0);
        output_temp_2[addr] = data_type_o(0);
   	}
//	P5.max_pooling_layer_a(activation_type, local_temp_1, local_temp_2);
    P5.max_pooling_layer_a(activation_type, output_temp_1, output_temp_2);
    RESET_7: for(int addr = 0; addr < 96*32*32; addr++){
//        local_temp_1[addr] = data_type_o(0);
        output_temp_1[addr] = data_type_o(0);
   	}
//	F6.fc_layer_a(activation_type, local_temp_2, fc_weight_port, fc_bias_port, local_temp_1);
    F6.fc_layer_a(activation_type, output_temp_2, fc_weight_port, fc_bias_port, output_temp_1);
    RESET_8: for(int addr = 0; addr < 96*32*32; addr++){
//        local_temp_2[addr] = data_type_o(0);
        output_temp_2[addr] = data_type_o(0);
   	}
//	F7.fc_layer_a(activation_type, local_temp_1, fc_weight_port+1048576*1*1, fc_bias_port+4096, local_temp_2);
    F7.fc_layer_a(activation_type, output_temp_1, fc_weight_port+1048576*1*1, fc_bias_port+4096, output_temp_2);
    RESET_9: for(int addr = 0; addr < 96*32*32; addr++){
//       	local_temp_1[addr] = data_type_o(0);
        output_temp_1[addr] = data_type_o(0);
   	}
*/
//	F8.fc_layer_a_no_activation(local_temp_2, fc_weight_port+1048576*1*1+16777216*1*1, fc_bias_port+4096+4096, fc_8_out_buf);
    F8.fc_layer_a_no_activation(output_temp_2, fc_weight_port+1048576*1*1+16777216*1*1, fc_bias_port+4096+4096, fc_8_out_buf);
    RESET_10: for(int i = 0; i < 10; i++){
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
