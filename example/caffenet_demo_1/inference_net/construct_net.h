//
// Created by yaochen on 24/12/16.
//

#ifndef _CONSTRUCT_NET_H_
#define _CONSTRUCT_NET_H_


#include <iostream>
#include <ap_fixed.h>

#include "config.h"
#include "conv_pool_layer_one_dim.h"
#include "conv_layer_one_dim_acc.h"
#include "pool_layer_one_dim.h"
#include "lrn_layer_one_dim.h"
#include "fc_layer_one_dim.h"

using namespace std;

// caffenet inference function
void inference_net(

	char activation_type,

	// input pic data
	data_type in_data_3D[3*227*227],

	// layer weights and bias inputs
	data_type_w *conv_weight_port,
	data_type_w *conv_bias_port,
	data_type_w *fc_weight_port,
	data_type_w *fc_bias_port,

	// output fc data
	data_type_o fc_8_out_a[1000*1*1]
) {

#if _HLS_MODE_

#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
#pragma HLS INTERFACE s_axilite port=activation_type bundle=CRTL_BUS

#pragma HLS INTERFACE m_axi depth=50 port=in_data_3D

#pragma HLS INTERFACE m_axi depth=50  port=conv_weight_port
#pragma HLS INTERFACE m_axi depth=50  port=conv_bias_port
#pragma HLS INTERFACE m_axi depth=50  port=fc_weight_port
#pragma HLS INTERFACE m_axi depth=50  port=fc_bias_port

#pragma HLS INTERFACE m_axi depth=50  port=fc_8_out_a

#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
	cout << "starting forward network process..........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif

	/******************************************************************************************/

	//construct network ----- caffenet
	conv_layer<data_type,data_type_w,data_type_o, 227, 11, 0, 4, 3, 96, 1> C1;
	pool_layer<data_type,data_type_w,data_type_o, 55, 3, 0, 2, 96> P1;
	lrn_layer<data_type_o, 96 ,5 ,27> L1;
	conv_layer<data_type,data_type_w,data_type_o, 27, 5, 2, 1, 96, 256, 2> C2;
	pool_layer<data_type,data_type_w,data_type_o, 27, 3, 0, 2, 256> P2;
	lrn_layer<data_type_o, 256, 5, 13> L2;
	conv_layer<data_type,data_type_w,data_type_o, 13, 3, 1, 1, 256, 384, 1> C3;
	conv_layer<data_type,data_type_w,data_type_o, 13, 3, 1, 1, 384, 384, 2> C4;
	conv_layer<data_type,data_type_w,data_type_o, 13, 3, 1, 1, 384, 256, 2> C5;
	pool_layer<data_type,data_type_w,data_type_o, 13, 3, 0, 2, 256> P5;
	fc_layer<data_type,data_type_w,data_type_o, 256, 6, 4096> F6;
	fc_layer<data_type,data_type_w,data_type_o, 4096, 1, 4096> F7;
	fc_layer<data_type,data_type_w,data_type_o, 4096, 1, 1000> F8;

	//temp storage space
	data_type_o in_data_buf[3*227*227];
	data_type_o fc_8_out_buf[1000];
	data_type_o  output_temp_1[96*55*55];
        data_type_o  output_temp_2[96*55*55];
    

        //internal memory initiallization
        for(int addr = 0; addr < 96*55*55; addr++){
            output_temp_1[addr] = data_type_o(0);
            output_temp_2[addr] = data_type_o(0);
        }
	for(int i = 0; i < 3; i++){
	    for(int j = 0; j < 227; j++){
		for(int k = 0; k < 227; k++){
		    in_data_buf[i*227*227 + j*227 + k] = in_data_3D[i*227*227 + j*227 + k];
	        }
	    }
	}


	//Forward propagation by layer
	C1.conv_layer_a(activation_type, in_data_buf, conv_weight_port, conv_bias_port, output_temp_1);
	P1.max_pooling_layer_a(activation_type, output_temp_1, output_temp_2);
	for(int addr = 0; addr < 96*55*55; addr++){
        	output_temp_1[addr] = data_type_o(0);
    	}
    L1.lrn_layer_a(nn_alpha_lrn[0], nn_beta_lrn[0], output_temp_2, output_temp_1);
    	for(int addr = 0; addr < 96*55*55; addr++){
        	output_temp_2[addr] = data_type_o(0);
    	}
    C2.conv_layer_a(activation_type, output_temp_1, conv_weight_port+288*11*11, conv_bias_port+96, output_temp_2);
    	for(int addr = 0; addr < 96*55*55; addr++){
        	output_temp_1[addr] = data_type_o(0);
    	}
    P2.max_pooling_layer_a(activation_type, output_temp_2, output_temp_1);
        for(int addr = 0; addr < 96*55*55; addr++){
        	output_temp_2[addr] = data_type_o(0);
    	}
    L2.lrn_layer_a(nn_alpha_lrn[1], nn_beta_lrn[1], output_temp_1, output_temp_2);
	for(int addr = 0; addr < 96*55*55; addr++){
        	output_temp_1[addr] = data_type_o(0);
    	}
	C3.conv_layer_a(activation_type, output_temp_2, conv_weight_port+288*11*11+12288*5*5, conv_bias_port+96+256, output_temp_1);
        for(int addr = 0; addr < 96*55*55; addr++){
        	output_temp_2[addr] = data_type_o(0);
    	}
	C4.conv_layer_a(activation_type, output_temp_1, conv_weight_port+288*11*11+12288*5*5+98304*3*3, conv_bias_port+96+256+384, output_temp_2);
    	for(int addr = 0; addr < 96*55*55; addr++){
        	output_temp_1[addr] = data_type_o(0);
    	}
	C5.conv_layer_a(activation_type, output_temp_2, conv_weight_port+288*11*11+12288*5*5+98304*3*3+73728*3*3, conv_bias_port+96+256+384+384, output_temp_1);
    	for(int addr = 0; addr < 96*55*55; addr++){
        	output_temp_2[addr] = data_type_o(0);
    	}
	P5.max_pooling_layer_a(activation_type, output_temp_1, output_temp_2);
    	for(int addr = 0; addr < 96*55*55; addr++){
        	output_temp_1[addr] = data_type_o(0);
    	}
	F6.fc_layer_a(activation_type, output_temp_2, fc_weight_port, fc_bias_port, output_temp_1);
    	for(int addr = 0; addr < 96*55*55; addr++){
        	output_temp_2[addr] = data_type_o(0);
    	}
	F7.fc_layer_a(activation_type, output_temp_1, fc_weight_port+1048576*6*6, fc_bias_port+4096, output_temp_2);
    	for(int addr = 0; addr < 96*55*55; addr++){
        	output_temp_1[addr] = data_type_o(0);
    	}
	F8.fc_layer_a_no_activation(output_temp_2, fc_weight_port+1048576*6*6+16777216*1*1, fc_bias_port+4096+4096, fc_8_out_buf);

	for(int i = 0; i < 1000; i++){
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
