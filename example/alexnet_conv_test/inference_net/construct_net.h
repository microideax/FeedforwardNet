//
// Created by yaochen on 24/12/16.
//

#ifndef _CONSTRUCT_NET_H_
#define _CONSTRUCT_NET_H_


#include <iostream>
#include <ap_fixed.h>

#include "config.h"
#include "conv_pool_layer_one_dim.h"
#include "conv_layer_one_dim.h"
#include "pool_layer_one_dim.h"
#include "lrn_layer_one_dim.h"
#include "fc_layer_one_dim.h"
#include "conv_acc_break.h"
#include "max_pool_acc.h"

using namespace std;

// alexnet inference function
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
	data_type_o  fc_8_out_a[1000*1*1],
    data_type_o  output_temp_1[96*55*55],
    data_type_o  output_temp_2[96*55*55]
) {

#if _HLS_MODE_

#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
#pragma HLS INTERFACE s_axilite port=activation_type bundle=CRTL_BUS

#pragma HLS INTERFACE m_axi depth=154587 port=in_data_3D

#pragma HLS INTERFACE m_axi depth=2332704  port=conv_weight_port
#pragma HLS INTERFACE m_axi depth=1376     port=conv_bias_port
#pragma HLS INTERFACE m_axi depth=50       port=fc_weight_port
#pragma HLS INTERFACE m_axi depth=50       port=fc_bias_port

#pragma HLS INTERFACE m_axi depth=1000     port=fc_8_out_a

#pragma HLS INTERFACE m_axi depth=96*55*55 port=output_temp_1
#pragma HLS INTERFACE m_axi depth=96*55*55 port=output_temp_2

#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
	cout << "starting forward network process..........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif

	/******************************************************************************************/

	//construct network --------------alexnet
	conv_acc<data_type, data_type_w, data_type_o, 16, 4, 16, 16> convAcc1;//{0<Tm<=M;0<Tn<=N;0<Tr<=R;0<Tc<=C;}
	lrn_layer<data_type_o, 96 ,5 ,55> L1;
    max_pool_acc<data_type, data_type_w, data_type_o, 96, 20, 20> maxPoolAcc1;//{0<Tm<=M;0<Tn<=N;0<Tr<=R;0<Tc<=C;}
    lrn_layer<data_type_o, 256, 5, 27> L2;
	fc_layer<data_type,data_type_w,data_type_o, 256, 6, 4096> F6;
	fc_layer<data_type,data_type_w,data_type_o, 4096, 1, 4096> F7;
	fc_layer<data_type,data_type_w,data_type_o, 4096, 1, 1000> F8;

	//temp storage space
	data_type_o  fc_8_out_buf[1000];

	//Forward propagation by layer
//	convAcc1.conv_layer_acc(3, 11, 96, 55, 55, 4, 0, output_temp_1, conv_weight_port, conv_bias_port, output_temp_2);
    convAcc1.conv_layer_acc(3, 11, 96, 55, 55, 4, 0, output_temp_1, conv_weight_port, conv_bias_port, output_temp_2);
    for(int addr = 0; addr < 96*55*55; addr++){
        output_temp_1[addr] = data_type_o(0);
    }
    L1.lrn_layer_a(nn_alpha_lrn[0], nn_beta_lrn[0], output_temp_2, output_temp_1);
    for(int addr = 0; addr < 96*55*55; addr++){
        	output_temp_2[addr] = data_type_o(0);
    }
   	maxPoolAcc1.max_pool_layer_acc(96, 3, 27, 27, 2, 0, output_temp_1, output_temp_2);
	for(int addr = 0; addr < 96*55*55; addr++){
        	output_temp_1[addr] = data_type_o(0);
    	}
    convAcc1.conv_layer_acc(48, 5, 128, 27, 27, 1, 2, output_temp_2, conv_weight_port+288*11*11, conv_bias_port+96, output_temp_1);
    convAcc1.conv_layer_acc(48, 5, 128, 27, 27, 1, 2, output_temp_2+48*27*27, conv_weight_port+288*11*11+12288*5*5/2, conv_bias_port+96+256/2, output_temp_1+128*27*27);
   	for(int addr = 0; addr < 96*55*55; addr++){
       	output_temp_2[addr] = data_type_o(0);
   	}
    L2.lrn_layer_a(nn_alpha_lrn[1], nn_beta_lrn[1], output_temp_1, output_temp_2);
	for(int addr = 0; addr < 96*55*55; addr++){
        	output_temp_1[addr] = data_type_o(0);
    	}
    maxPoolAcc1.max_pool_layer_acc(256, 3, 13, 13, 2, 0, output_temp_2, output_temp_1);
    for(int addr = 0; addr < 96*55*55; addr++){
     	output_temp_2[addr] = data_type_o(0);
   	}
	convAcc1.conv_layer_acc(256, 3, 384, 13, 13, 1, 1, output_temp_1, conv_weight_port+288*11*11+12288*5*5, conv_bias_port+96+256, output_temp_2);
    for(int addr = 0; addr < 96*55*55; addr++){
     	output_temp_1[addr] = data_type_o(0);
   	}
	convAcc1.conv_layer_acc(192, 3, 192, 13, 13, 1, 1, output_temp_2, conv_weight_port+288*11*11+12288*5*5+98304*3*3, conv_bias_port+96+256+384, output_temp_1);
    convAcc1.conv_layer_acc(192, 3, 192, 13, 13, 1, 1, output_temp_2+192*13*13, conv_weight_port+288*11*11+12288*5*5+98304*3*3+73728*3*3/2, conv_bias_port+96+256+384+384/2, output_temp_1+192*13*13);
   	for(int addr = 0; addr < 96*55*55; addr++){
       	output_temp_2[addr] = data_type_o(0);
   	}
	convAcc1.conv_layer_acc(192, 3, 128, 13, 13, 1, 1, output_temp_1, conv_weight_port+288*11*11+12288*5*5+98304*3*3+73728*3*3, conv_bias_port+96+256+384+384, output_temp_2);
    convAcc1.conv_layer_acc(192, 3, 128, 13, 13, 1, 1, output_temp_1+192*13*13, conv_weight_port+288*11*11+12288*5*5+98304*3*3+73728*3*3+49152*3*3/2, conv_bias_port+96+256+384+384+256/2, output_temp_2+128*13*13);
   	for(int addr = 0; addr < 96*55*55; addr++){
       	output_temp_1[addr] = data_type_o(0);
   	}
	maxPoolAcc1.max_pool_layer_acc(256, 3, 6, 6, 2, 0, output_temp_2, output_temp_1);
   	for(int addr = 0; addr < 96*55*55; addr++){
       	output_temp_2[addr] = data_type_o(0);
   	}
	F6.fc_layer_a(activation_type, output_temp_2, fc_weight_port, fc_bias_port, output_temp_1);
//	convAcc1.conv_layer_acc(256, 6, 4096, 1, 1, 6, 0, output_temp_2,fc_weight_port, fc_bias_port, output_temp_1);
    	for(int addr = 0; addr < 96*55*55; addr++){
        	output_temp_2[addr] = data_type_o(0);
    	}
	F7.fc_layer_a(activation_type, output_temp_1, fc_weight_port+1048576*6*6, fc_bias_port+4096, output_temp_2);
	//fcAcc7.conv_layer_acc(4096, 1, 4096, 1, 1, 1, 0, output_temp_1, fc_weight_port+1048576*6*6, fc_bias_port+4096, output_temp_2);
    	for(int addr = 0; addr < 96*55*55; addr++){
        	output_temp_1[addr] = data_type_o(0);
    	}
	F8.fc_layer_a_no_activation(output_temp_2, fc_weight_port+1048576*6*6+16777216*1*1, fc_bias_port+4096+4096, fc_8_out_buf);
	//fcAcc8.conv_layer_acc(4096, 1, 1000, 1, 1, 1, 0, output_temp_2,fc_weight_port+1048576*6*6+16777216*1*1, fc_bias_port+4096+4096, fc_8_out_buf);
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