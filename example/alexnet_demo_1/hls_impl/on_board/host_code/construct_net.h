//
// Created by yaochen on 24/12/16.
//

#ifndef _CONSTRUCT_NET_H_
#define _CONSTRUCT_NET_H_

#include <iostream>
#include <ap_fixed.h>
#include "config.h"
//#include "weight_bias.h"
//#include "../../fpga_cnn/conv_lrn_pool_layer_one_dim.h"
#include "../../fpga_cnn/conv_layer_one_dim.h"
#include "../../fpga_cnn/pool_layer_one_dim.h"
#include "../../fpga_cnn/fc_layer_one_dim.h"
#include "../../fpga_cnn/pow_function.h"
//#include "../../fpga_cnn/dropout_layer.h"
#include "../../fpga_cnn/lrn_layer_one_dim.h"
#include "../../fpga_cnn/array_reset.h"

void inference_net(

	char activation_type,

	// input pic data
	data_type in_data_3D[3*227*227],

	// layer weights and bias inputs ------- Alexnet
	data_type_w *conv_weight_port,
	data_type_w *conv_bias_port,
	data_type_w *fc_weight_port,
	data_type_w *fc_bias_port,

	// output fc data
	data_type_o fc_8_out_a[1000*1*1],

	// temp data buffering interface
	data_type_o output_1[96*55*55],
	data_type_o output_2[96*55*55]
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

#pragma HLS INTERFACE m_axi depth=55 port=output_1
#pragma HLS INTERFACE m_axi depth=55 port=output_2

#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
	cout << "starting forward network process..........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif

	/******************************************************************************************/

	//construct network --------------alexnet
	conv_layer<data_type,data_type_w,data_type_o, 227, 11, 0, 4, 3, 96, 1> C1;
	lrn_layer<data_type_o, 96 ,5 ,55> L1;
	pool_layer<data_type,data_type_w,data_type_o, 55, 3, 0, 2, 96> P1;
	conv_layer<data_type,data_type_w,data_type_o, 27, 5, 2, 1, 96, 256, 2> C2;
	lrn_layer<data_type_o, 256, 5, 27> L2;
	pool_layer<data_type,data_type_w,data_type_o, 27, 3, 0, 2, 256> P2;
	conv_layer<data_type,data_type_w,data_type_o, 13, 3, 1, 1, 256, 384, 1> C3;
	conv_layer<data_type,data_type_w,data_type_o, 13, 3, 1, 1, 384, 384, 2> C4;
	conv_layer<data_type,data_type_w,data_type_o, 13, 3, 1, 1, 384, 256, 2> C5;
	pool_layer<data_type,data_type_w,data_type_o, 13, 3, 0, 2, 256> P5;
	fc_layer<data_type,data_type_w,data_type_o, 256, 6, 4096> F6;
	fc_layer<data_type,data_type_w,data_type_o, 4096, 1, 4096> F7;
	fc_layer<data_type,data_type_w,data_type_o, 4096, 1, 1000> F8;

	//temp storage space
    data_type_o  fc_8_out_buf[1000*1*1];
    data_type  in_data_buf[3*227*227];
//    data_type_o  output_1[96*27*27];
//    data_type_o  output_2[96*27*27];

    //internal memory initiallization
//    array_reset(output_1, 96*55*55);
//    array_reset(output_2, 96*55*55);
    for(int addr = 0; addr < 96*55*55; addr++){
        output_1[addr] = data_type_o(0);
        output_2[addr] = data_type_o(0);
    }
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 227; j++){
            for(int k = 0; k < 227; k++){
                in_data_buf[i*227*227 + j*227 + k] = in_data_3D[i*227*227 + j*227 + k];
            }
        }
    }

	//Forward propagation by layer
	C1.conv_layer_a(activation_type, in_data_buf, conv_weight_port, conv_bias_port, output_1);
    L1.lrn_layer_a(nn_alpha_lrn[0], nn_beta_lrn[0], output_1, output_2);
//    array_reset(output_1, 96*55*55);
    for(int addr = 0; addr < 96*55*55; addr++){
        output_1[addr] = data_type_o(0);
    }
    P1.max_pooling_layer_a(activation_type, output_2, output_1);
//    C1L1P1.conv_layer_w_lrn_w_pool_a(activation_type, in_data_3D, conv_1_weight_a, conv_1_bias_a, nn_alpha_lrn[0], nn_beta_lrn[0], output_1);
//    array_reset(output_2, 96*55*55);
    for(int addr = 0; addr < 96*55*55; addr++){
        output_2[addr] = data_type_o(0);
    }
	C2.conv_layer_a(activation_type, output_1, conv_weight_port+288*11*11, conv_bias_port+96, output_2);
//    array_reset(output_1, 96*55*55);
    for(int addr = 0; addr < 96*55*55; addr++){
        output_1[addr] = data_type_o(0);
    }
    L2.lrn_layer_a(nn_alpha_lrn[1], nn_beta_lrn[1], output_2, output_1);
//    C2L2P2.conv_layer_w_lrn_w_pool_a(activation_type, output_1, conv_2_weight_a, conv_2_bias_a, nn_alpha_lrn[1], nn_beta_lrn[1], output_2);
//    array_reset(output_2, 96*55*55);
    for(int addr = 0; addr < 96*55*55; addr++){
        output_2[addr] = data_type_o(0);
    }
    P2.max_pooling_layer_a(activation_type, output_1, output_2);
//    array_reset(output_1, 96*55*55);
    for(int addr = 0; addr < 96*55*55; addr++){
        output_1[addr] = data_type_o(0);
    }
	C3.conv_layer_a(activation_type, output_2, conv_weight_port+288*11*11+12288*5*5, conv_bias_port+96+256, output_1);
//    array_reset(output_2, 96*55*55);
    for(int addr = 0; addr < 96*55*55; addr++){
        output_2[addr] = data_type_o(0);
    }
	C4.conv_layer_a(activation_type, output_1, conv_weight_port+288*11*11+12288*5*5+98304*3*3, conv_bias_port+96+256+384, output_2);
//    array_reset(output_1, 96*55*55);
    for(int addr = 0; addr < 96*55*55; addr++){
        output_1[addr] = data_type_o(0);
    }
	C5.conv_layer_a(activation_type, output_2, conv_weight_port+288*11*11+12288*5*5+98304*3*3+73728*3*3, conv_bias_port+96+256+384+384, output_1);
//    array_reset(output_2, 96*55*55);
    for(int addr = 0; addr < 96*55*55; addr++){
        output_2[addr] = data_type_o(0);
    }
	P5.max_pooling_layer_a(activation_type, output_1, output_2);
//    array_reset(output_1, 96*55*55);
    for(int addr = 0; addr < 96*55*55; addr++){
        output_1[addr] = data_type_o(0);
    }
	F6.fc_layer_a(activation_type, output_2, fc_weight_port, fc_bias_port, output_1);
//    array_reset(output_2, 96*55*55);
    for(int addr = 0; addr < 96*55*55; addr++){
        output_2[addr] = data_type_o(0);
    }
	/*D6.dropout_layer_a(dropout_ratio, fc_6_out, drop_6_out);*/
	F7.fc_layer_a(activation_type, output_1, fc_weight_port+1048576*6*6, fc_bias_port+4096, output_2);
//    array_reset(output_1, 96*55*55);
    for(int addr = 0; addr < 96*55*55; addr++){
        output_1[addr] = data_type_o(0);
    }
	/*D7.dropout_layer_a(dropout_ratio, fc_7_out, drop_7_out);*/
	F8.fc_layer_a_no_activation(output_2, fc_weight_port+1048576*6*6+16777216*1*1, fc_bias_port+4096+4096, fc_8_out_buf);

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
