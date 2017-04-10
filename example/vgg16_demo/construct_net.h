//
// Created by yaochen on 24/12/16.
//

#ifndef _CONSTRUCT_NET_H_
#define _CONSTRUCT_NET_H_

#include <iostream>
#include "config.h"
//#include "weight_bias.h"
#include "../../fpga_cnn/conv_pool_layer.h"
#include "../../fpga_cnn/conv_layer.h"
#include "../../fpga_cnn/pool_layer.h"
#include "../../fpga_cnn/fc_layer.h"
//#include "../../fpga_cnn/dropout_layer.h"
#include "../../fpga_cnn/lrn_layer.h"

void inference_net(

	char activation_type,

	// input pic data
	float in_data_3D[3][224][224],

	// layer weights and bias inputs ------- vgg16
	float        conv_1_1_weight_a[192][3][3],
	float        conv_1_1_bias_a[64],
	float        conv_1_2_weight_a[4096][3][3],
	float        conv_1_2_bias_a[64],
	float        conv_2_1_weight_a[8192][3][3],
	float        conv_2_1_bias_a[128],
	float        conv_2_2_weight_a[16384][3][3],
	float        conv_2_2_bias_a[128],
	float        conv_3_1_weight_a[32768][3][3],
	float        conv_3_1_bias_a[256],
	float        conv_3_2_weight_a[65536][3][3],
	float        conv_3_2_bias_a[256],
	float        conv_3_3_weight_a[65536][3][3],
	float        conv_3_3_bias_a[256],
	float        conv_4_1_weight_a[131072][3][3],
	float        conv_4_1_bias_a[512],
	float        conv_4_2_weight_a[262144][3][3],
	float        conv_4_2_bias_a[512],
	float        conv_4_3_weight_a[262144][3][3],
	float        conv_4_3_bias_a[512],
	float        conv_5_1_weight_a[262144][3][3],
	float        conv_5_1_bias_a[512],
	float        conv_5_2_weight_a[262144][3][3],
	float        conv_5_2_bias_a[512],
	float        conv_5_3_weight_a[262144][3][3],
	float        conv_5_3_bias_a[512],
	float        fc_6_weight_a[2097152][7][7],
	float        fc_6_bias_a[4096],
	float        fc_7_weight_a[16777216][1][1],
	float        fc_7_bias_a[4096],
	float        fc_8_weight_a[4096000][1][1],
	float        fc_8_bias_a[1000],

	// output fc data
	float fc_8_out_a[1000][1][1]
) {


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
	cout << "starting forward network process..........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif

	/******************************************************************************************/

	//construct network --------------vgg16
	conv_layer<float, 224, 3, 1, 1, 3, 64, 1> C1_1;
	conv_layer<float, 224, 3, 1, 1, 64, 64, 1> C1_2;
	pool_layer<float, 224, 2, 0, 2, 64> P1;
	conv_layer<float, 112, 3, 1, 1, 64, 128, 1> C2_1;
	conv_layer<float, 112, 3, 1, 1, 128, 128, 1> C2_2;
	pool_layer<float, 112, 2, 0, 2, 128> P2;
	conv_layer<float, 56, 3, 1, 1, 128, 256, 1> C3_1;
	conv_layer<float, 56, 3, 1, 1, 256, 256, 1> C3_2;
	conv_layer<float, 56, 3, 1, 1, 256, 256, 1> C3_3;
	pool_layer<float, 56, 2, 0, 2, 256> P3;
	conv_layer<float, 28, 3, 1, 1, 256, 512, 1> C4_1;
	conv_layer<float, 28, 3, 1, 1, 512, 512, 1> C4_2;
	conv_layer<float, 28, 3, 1, 1, 512, 512, 1> C4_3;
	pool_layer<float, 28, 2, 0, 2, 512> P4;
	conv_layer<float, 14, 3, 1, 1, 512, 512, 1> C5_1;
	conv_layer<float, 14, 3, 1, 1, 512, 512, 1> C5_2;
	conv_layer<float, 14, 3, 1, 1, 512, 512, 1> C5_3;
	pool_layer<float, 14, 2, 0, 2, 512> P5;
	fc_layer<float, 512, 7, 4096> F6;
	/*dropout_layer<float, 4096, 1> D6;*/
	fc_layer<float, 4096, 1, 4096> F7;
	/*dropout_layer<float, 4096, 1> D7;*/
	fc_layer<float, 4096, 1, 1000> F8;

	//temp storage space
	float  conv_1_1_out[64][224][224] = { 0 };
	float  conv_1_2_out[64][224][224] = { 0 };
	float  pool_1_out[64][112][112] = { 0 };
	float  conv_2_1_out[128][112][112] = { 0 };
	float  conv_2_2_out[128][112][112] = { 0 };
	float  pool_2_out[128][56][56] = { 0 };
	float  conv_3_1_out[256][56][56] = { 0 };
	float  conv_3_2_out[256][56][56] = { 0 };
	float  conv_3_3_out[256][56][56] = { 0 };
	float  pool_3_out[256][28][28] = { 0 };
	float  conv_4_1_out[512][28][28] = { 0 };
	float  conv_4_2_out[512][28][28] = { 0 };
	float  conv_4_3_out[512][28][28] = { 0 };
	float  pool_4_out[512][14][14] = { 0 };
	float  conv_5_1_out[512][14][14] = { 0 };
	float  conv_5_2_out[512][14][14] = { 0 };
	float  conv_5_3_out[512][14][14] = { 0 };
	float  pool_5_out[512][7][7] = { 0 };
	float  fc_6_out[4096][1][1] = { 0 };
	/*float  drop_6_out[4096][1][1] = { 0 };*/
	float  fc_7_out[4096][1][1] = { 0 };
	/*float  drop_7_out[4096][1][1] = { 0 };*/

	//Forward propagation by layer
	C1_1.conv_layer_a(activation_type, in_data_3D, conv_1_1_weight_a, conv_1_1_bias_a, conv_1_1_out);
	C1_2.conv_layer_a(activation_type, conv_1_1_out, conv_1_2_weight_a, conv_1_2_bias_a, conv_1_2_out);
	P1.max_pooling_layer_a(activation_type, conv_1_2_out, pool_1_out);
	C2_1.conv_layer_a(activation_type, pool_1_out, conv_2_1_weight_a, conv_2_1_bias_a, conv_2_1_out);
	C2_2.conv_layer_a(activation_type, conv_2_1_out, conv_2_2_weight_a, conv_2_2_bias_a, conv_2_2_out);
	P2.max_pooling_layer_a(activation_type, conv_2_2_out, pool_2_out);
	C3_1.conv_layer_a(activation_type, pool_2_out, conv_3_1_weight_a, conv_3_1_bias_a, conv_3_1_out);
	C3_2.conv_layer_a(activation_type, conv_3_1_out, conv_3_2_weight_a, conv_3_2_bias_a, conv_3_2_out);
	C3_3.conv_layer_a(activation_type, conv_3_2_out, conv_3_3_weight_a, conv_3_3_bias_a, conv_3_3_out);
	P3.max_pooling_layer_a(activation_type, conv_3_3_out, pool_3_out);
	C4_1.conv_layer_a(activation_type, pool_3_out, conv_4_1_weight_a, conv_4_1_bias_a, conv_4_1_out);
	C4_2.conv_layer_a(activation_type, conv_4_1_out, conv_4_2_weight_a, conv_4_2_bias_a, conv_4_2_out);
	C4_3.conv_layer_a(activation_type, conv_4_2_out, conv_4_3_weight_a, conv_4_3_bias_a, conv_4_3_out);
	P4.max_pooling_layer_a(activation_type, conv_4_3_out, pool_4_out);
	C5_1.conv_layer_a(activation_type, pool_4_out, conv_5_1_weight_a, conv_5_1_bias_a, conv_5_1_out);
	C5_2.conv_layer_a(activation_type, conv_5_1_out, conv_5_2_weight_a, conv_5_2_bias_a, conv_5_2_out);
	C5_3.conv_layer_a(activation_type, conv_5_2_out, conv_5_3_weight_a, conv_5_3_bias_a, conv_5_3_out);
	P5.max_pooling_layer_a(activation_type, conv_5_3_out, pool_5_out);
	F6.fc_layer_a(activation_type, pool_5_out, fc_6_weight_a, fc_6_bias_a, fc_6_out);
	/*D6.dropout_layer_a(dropout_ratio, fc_6_out, drop_6_out);*/
	F7.fc_layer_a(activation_type, fc_6_out, fc_7_weight_a, fc_7_bias_a, fc_7_out);
	/*D7.dropout_layer_a(dropout_ratio, fc_7_out, drop_7_out);*/
	F8.fc_layer_a_no_activation(activation_type, fc_7_out, fc_8_weight_a, fc_8_bias_a, fc_8_out_a);

	/******************************************************************************************/


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
	cout << "Finished forward network process .........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif

}

#endif //_CONSTRUCT_NET_H_
