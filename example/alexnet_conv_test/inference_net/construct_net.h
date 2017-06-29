//
// Created by yaochen on 24/12/16.
//

#ifndef _CONSTRUCT_NET_H_
#define _CONSTRUCT_NET_H_


#include <iostream>
#include <ap_fixed.h>

#include "config.h"
//#include "conv_pool_layer_one_dim.h"
//#include "conv_layer_one_dim.h"
//#include "pool_layer_one_dim.h"
#include "lrn_layer_one_dim.h"
#include "fc_layer_one_dim.h"
#include "conv_acc_break.h"
#include "max_pool_acc.h"
#include "acc_instance.h"

using namespace std;

// alexnet inference function
void inference_net(

	char activation_type,

	// input pic data
	data_type in_data_3D[3*227*227],

	// layer weights and bias inputs
	data_type_w conv_weight_port[2332704],
	data_type_w conv_bias_port[1376],
	data_type_w fc_weight_port[58621952],
	data_type_w fc_bias_port[13288],

	// output fc data
	data_type_o  fc_8_out_a[1000*1*1],
        data_type_o  ex_buf_1[96*55*55],
        data_type_o  ex_buf_2[96*55*55]
) {

#if _HLS_MODE_
#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
#pragma HLS INTERFACE s_axilite port=activation_type bundle=CRTL_BUS

#pragma HLS INTERFACE m_axi depth=154587 port=in_data_3D

#pragma HLS INTERFACE m_axi depth=2332704  port=conv_weight_port
#pragma HLS INTERFACE m_axi depth=1376     port=conv_bias_port
#pragma HLS INTERFACE m_axi depth=58621952 port=fc_weight_port
#pragma HLS INTERFACE m_axi depth=13288    port=fc_bias_port

#pragma HLS INTERFACE m_axi depth=1000     port=fc_8_out_a

#pragma HLS INTERFACE m_axi depth=96*55*55 port=ex_buf_1
#pragma HLS INTERFACE m_axi depth=96*55*55 port=ex_buf_2
#endif

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
	cout << "starting forward network process..........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif

	/******************************************************************************************/

	//construct network --------------alexnet
//	conv_acc<data_type, data_type_w, data_type_o, 16, 4, 16, 16> convAcc1;//{0<Tm<=M;0<Tn<=N;0<Tr<=R;0<Tc<=C;}
//	lrn_layer<data_type_o, 96 ,5 ,55> L1;
//	max_pool_acc<data_type, data_type_w, data_type_o, 16, 16, 16> maxPoolAcc1;//{0<Tm<=M;0<Tn<=N;0<Tr<=R;0<Tc<=C;}
//	lrn_layer<data_type_o, 256, 5, 27> L2;
//	fc_layer<data_type,data_type_w,data_type_o, 256, 6, 4096> F6;
//	fc_layer<data_type,data_type_w,data_type_o, 4096, 1, 4096> F7;
//	fc_layer<data_type,data_type_w,data_type_o, 4096, 1, 1000> F8;

    //temp storage space
	data_type_o  fc_8_out_buf[1000];

    //Address shifts
    // conv 2
	int shift_weight_conv2_1 = 288*11*11;
	int shift_bias_conv2_1   = 96;
	int out_shift_1          = 48*27*27;
	int shift_weight_conv2_2 = 288*11*11+12288*5*5/2;
	int shift_bias_conv2_2   = 96+256/2;
	// conv 3
	int shift_weight_conv3   = 288*11*11+12288*5*5;
	int shift_bias_conv3     = 96+256;
	// conv 4
	int shift_weight_conv4_1 = 288*11*11+12288*5*5+98304*3*3;
	int shift_bias_conv4_1   = 96+256+384;
	int shift_weight_conv4_2 = 288*11*11+12288*5*5+98304*3*3+73728*3*3/2;
	int shift_bias_conv4_2   = 96+256+384+384/2;
	// conv 5
	int shift_weight_conv5_1 = 288*11*11+12288*5*5+98304*3*3+73728*3*3;
	int shift_bias_conv5_1   = 96+256+384+384;
	int shift_weight_conv5_2 = 288*11*11+12288*5*5+98304*3*3+73728*3*3+49152*3*3/2;
	int shift_bias_conv5_2   = 96+256+384+384+256/2;
	// fc 2
	int shift_weight_fc2     = 1048576*6*6;
	int shift_bias_fc2       = 4096;
	// fc 3
    int shift_weight_fc3     = 1048576*6*6+16777216*1*1;
	int shift_bias_fc3       = 8192;

#pragma HLS ALLOCATION instances=conv_layer_new limit=1 function

/*    
	//Forward propagation by layer
    // conv 1
//    convAcc1.conv_layer_acc(3, 11, 96, 55, 55, 4, 0, ex_buf_1, conv_weight_port, conv_bias_port, ex_buf_2);
    conv_layer_new(3, 11, 96, 55, 55, 4, 0, ex_buf_1, conv_weight_port, conv_bias_port, ex_buf_2, 0, 0);
    Reset_1: for(int addr = 0; addr < 96*55*55; addr++){ ex_buf_1[addr] = data_type_o(0); }
    L1.lrn_layer_a(nn_alpha_lrn[0], nn_beta_lrn[0], ex_buf_2, ex_buf_1);
    Reset_2: for(int addr = 0; addr < 96*55*55; addr++){ ex_buf_2[addr] = data_type_o(0); }
    maxPoolAcc1.max_pool_layer_acc(96, 3, 27, 27, 2, 0, ex_buf_1, ex_buf_2);
    Reset_3: for(int addr = 0; addr < 96*55*55; addr++){ ex_buf_1[addr] = data_type_o(0); }
    // conv 2
    convAcc1.conv_layer_acc(48, 5, 128, 27, 27, 1, 2, ex_buf_2, conv_weight_port+shift_weight_conv2_1, conv_bias_port+shift_bias_conv2_1, ex_buf_1, 0, 0);
//    conv_layer_new(48, 5, 128, 27, 27, 1, 2, ex_buf_2, conv_weight_port, conv_bias_port, ex_buf_1, shift_weight_conv2_1, shift_bias_conv2_1);
//    convAcc1.conv_layer_acc(48, 5, 128, 27, 27, 1, 2, ex_buf_1+48*27*27, conv_weight_port+shift_weight_conv2_2, conv_bias_port+shift_bias_conv2_2, ex_buf_2+128*27*27);
    for(int addr = 0; addr < 96*55*55; addr++){ ex_buf_2[addr] = data_type_o(0); }
    L2.lrn_layer_a(nn_alpha_lrn[1], nn_beta_lrn[1], ex_buf_1, ex_buf_2);
    for(int addr = 0; addr < 96*55*55; addr++){ ex_buf_1[addr] = data_type_o(0); }
    maxPoolAcc1.max_pool_layer_acc(256, 3, 13, 13, 2, 0, ex_buf_2, ex_buf_1);
    for(int addr = 0; addr < 96*55*55; addr++){ ex_buf_2[addr] = data_type_o(0); }
    // conv 3
    convAcc1.conv_layer_acc(256, 3, 384, 13, 13, 1, 1, ex_buf_1, conv_weight_port+shift_weight_conv3, conv_bias_port+shift_bias_conv3, ex_buf_2, 0, 0);
    for(int addr = 0; addr < 96*55*55; addr++){ ex_buf_1[addr] = data_type_o(0); }
    // conv 4
    convAcc1.conv_layer_acc(192, 3, 192, 13, 13, 1, 1, ex_buf_2,           conv_weight_port+shift_weight_conv4_1, conv_bias_port+shift_bias_conv4_1, ex_buf_1, 0, 0);
    convAcc1.conv_layer_acc(192, 3, 192, 13, 13, 1, 1, ex_buf_2+192*13*13, conv_weight_port+shift_weight_conv4_2, conv_bias_port+shift_bias_conv4_2, ex_buf_1+192*13*13, 0, 0);
    for(int addr = 0; addr < 96*55*55; addr++){ ex_buf_2[addr] = data_type_o(0); }
    //conv 5
    convAcc1.conv_layer_acc(192, 3, 128, 13, 13, 1, 1, ex_buf_1,           conv_weight_port+shift_weight_conv5_1, conv_bias_port+shift_bias_conv5_1, ex_buf_2, 0, 0);
    convAcc1.conv_layer_acc(192, 3, 128, 13, 13, 1, 1, ex_buf_1+192*13*13, conv_weight_port+shift_weight_conv5_2, conv_bias_port+shift_bias_conv5_2, ex_buf_2+128*13*13, 0, 0);
    for(int addr = 0; addr < 96*55*55; addr++){ ex_buf_1[addr] = data_type_o(0); }
    maxPoolAcc1.max_pool_layer_acc(256, 3, 6, 6, 2, 0, ex_buf_2, ex_buf_1);
    for(int addr = 0; addr < 96*55*55; addr++){ ex_buf_2[addr] = data_type_o(0); }
    //fc 1
    //F6.fc_layer_a(activation_type, output_temp_1, fc_weight_port, fc_bias_port, output_temp_2);
    convAcc1.conv_layer_acc(256, 6, 4096, 1, 1, 6, 0, ex_buf_1, fc_weight_port, fc_bias_port, ex_buf_2, 0, 0);
    for(int addr = 0; addr < 96*55*55; addr++){ ex_buf_1[addr] = data_type_o(0); }
    // fc 2
    //F7.fc_layer_a(activation_type, output_temp_2, fc_weight_port+shift_weight_fc2, fc_bias_port+shift_bias_fc2, output_temp_1);
    convAcc1.conv_layer_acc(4096, 1, 4096, 1, 1, 1, 0, ex_buf_2, fc_weight_port+shift_weight_fc2, fc_bias_port+shift_bias_fc2, ex_buf_1, 0, 0);
    for(int addr = 0; addr < 96*55*55; addr++){ ex_buf_2[addr] = data_type_o(0); }
    // fc 3
    //F8.fc_layer_a_no_activation(output_temp_1, fc_weight_port+shift_weight_fc3, fc_bias_port+shift_bias_fc3, fc_8_out_buf);
    convAcc1.conv_layer_acc(4096, 1, 1000, 1, 1, 1, 0, ex_buf_1, fc_weight_port+shift_weight_fc3, fc_bias_port+shift_bias_fc3, ex_buf_2, 0, 0);
  */  

/*
    // conv 1
    conv_layer_new(3, 11, 96, 55, 55, 4, 0, ex_buf_1, conv_weight_port, conv_bias_port, ex_buf_2, 0, 0);
    Reset_1: for(int addr = 0; addr < 96*55*55; addr++){ ex_buf_1[addr] = data_type_o(0); }
    max_pool_layer_new(96, 3, 27, 27, 2, 0, ex_buf_2, ex_buf_1);
    Reset_3: for(int addr = 0; addr < 96*55*55; addr++){ ex_buf_2[addr] = data_type_o(0); }
*/
    // conv 2
    conv_layer_new(48, 5, 128, 27, 27, 1, 2, ex_buf_1, conv_weight_port, conv_bias_port, ex_buf_2, shift_weight_conv2_1, shift_bias_conv2_1, 0, 0);
    for(int addr = 0; addr < 96*55*55; addr++){ ex_buf_1[addr] = data_type_o(0); }
    max_pool_layer_new(256, 3, 13, 13, 2, 0, ex_buf_2, ex_buf_1);
    for(int addr = 0; addr < 96*55*55; addr++){ ex_buf_2[addr] = data_type_o(0); }

    // conv 3
    conv_layer_new(256, 3, 384, 13, 13, 1, 1, ex_buf_1, conv_weight_port, conv_bias_port, ex_buf_2, shift_weight_conv3, shift_bias_conv3, 0, 0);
    // conv 4
/*
    conv_layer_new(192, 3, 192, 13, 13, 1, 1, ex_buf_2, conv_weight_port, conv_bias_port, ex_buf_1, shift_weight_conv4_1, shift_bias_conv4_1);
    conv_layer_new(192, 3, 192, 13, 13, 1, 1, ex_buf_2+192*13*13, conv_weight_port, conv_bias_port, ex_buf_1+192*13*13, shift_weight_conv4_2, shift_bias_conv4_2);
    // conv 5
    conv_layer_new(192, 3, 128, 13, 13, 1, 1, ex_buf_1, conv_weight_port, conv_bias_port, ex_buf_2, shift_weight_conv5_1, shift_bias_conv5_1);
    convAcc1.conv_layer_acc(192, 3, 128, 13, 13, 1, 1, ex_buf_1+192*13*13, conv_weight_port, conv_bias_port, ex_buf_2+128*13*13, shift_weight_conv5_2, shift_bias_conv5_2);
*/
    for(int i = 0; i < 1000; i++){
	    fc_8_out_a[i] = ex_buf_2[i];
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
