//
// Created by yaochen on 24/12/16.
//

#ifndef _CONSTRUCT_NET_H_
#define _CONSTRUCT_NET_H_

#include <iostream>
#include "config.h"
#include "weight_bias.h"
#include "conv_pool_layer.h"
#include "conv_layer.h"
#include "pool_layer.h"
#include "fc_layer.h"
#include "lrn_layer.h"
#include "dropout_layer.h"

void inference_net(

	char activation_type,

	// input pic data
	//float in_data_3D[1][32][32],
	//float in_data_3D[1][28][28],
	float in_data_3D[3][227][227],

	// layer weights and bias inputs ------- Alexnet
	float        conv_1_weight_a[288][11][11],
	float        conv_1_bias_a[96],
	float        conv_2_weight_a[12288][5][5],
	float        conv_2_bias_a[256],
	float        conv_3_weight_a[98304][3][3],
	float        conv_3_bias_a[384],
	float        conv_4_weight_a[73728][3][3],
	float        conv_4_bias_a[384],
	float        conv_5_weight_a[49152][3][3],
	float        conv_5_bias_a[256],
	float        fc_6_weight_a[1048576][6][6],
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

	//construct network --------------alexnet
	conv_layer<float, 227, 11, 0, 4, 3, 96, 1> C1;
	lrn_layer<float, 96 ,5 ,55> L1;
	pool_layer<float, 55, 3, 0, 2, 96> P1;
	conv_layer<float, 27, 5, 2, 1, 96, 256, 2> C2;
	lrn_layer<float, 256, 5, 27> L2;
	pool_layer<float, 27, 3, 0, 2, 256> P2;
	conv_layer<float, 13, 3, 1, 1, 256, 384, 1> C3;
	conv_layer<float, 13, 3, 1, 1, 384, 384, 2> C4;
	conv_layer<float, 13, 3, 1, 1, 384, 256, 2> C5;
	pool_layer<float, 13, 3, 0, 2, 256> P5;
	fc_layer<float, 256, 6, 4096> F6;
	dropout_layer<float, 4096, 1> D6;
	fc_layer<float, 4096, 1, 4096> F7;
	dropout_layer<float, 4096, 1> D7;
	fc_layer<float, 4096, 1, 1000> F8;

	//temp storage space
	float  conv_1_out[96][55][55] = { 0 };
	float  lrn_1_out[96][55][55] = { 0 };
	float  pool_1_out[96][27][27] = { 0 };
	float  conv_2_out[256][27][27] = { 0 };
	float  lrn_2_out[256][27][27] = { 0 };
	float  pool_2_out[256][13][13] = { 0 };
	float  conv_3_out[384][13][13] = { 0 };
	float  conv_4_out[384][13][13] = { 0 };
	float  conv_5_out[256][13][13] = { 0 };
	float  pool_5_out[256][6][6] = { 0 };
	float  fc_6_out[4096][1][1] = { 0 };
	float  drop_6_out[4096][1][1] = { 0 };
	float  fc_7_out[4096][1][1] = { 0 };
	float  drop_7_out[4096][1][1] = { 0 };

	//Forward propagation by layer
	C1.conv_layer_a(activation_type, in_data_3D, conv_1_weight_a, conv_1_bias_a, conv_1_out);
	L1.lrn_layer_a(alpha,beta, conv_1_out, lrn_1_out);
	P1.max_pooling_layer_a(activation_type, lrn_1_out, pool_1_out);
	C2.conv_layer_a(activation_type, pool_1_out, conv_2_weight_a, conv_2_bias_a, conv_2_out);
	L2.lrn_layer_a(alpha, beta, conv_2_out, lrn_2_out);
	P2.max_pooling_layer_a(activation_type, lrn_2_out, pool_2_out);
	C3.conv_layer_a(activation_type, pool_2_out, conv_3_weight_a, conv_3_bias_a, conv_3_out);
	C4.conv_layer_a(activation_type, conv_3_out, conv_4_weight_a, conv_4_bias_a, conv_4_out);
	C5.conv_layer_a(activation_type, conv_4_out, conv_5_weight_a, conv_5_bias_a, conv_5_out);
	P5.max_pooling_layer_a(activation_type, conv_5_out, pool_5_out);
	F6.fc_layer_a(activation_type, pool_5_out, fc_6_weight_a, fc_6_bias_a, fc_6_out);
	D6.dropout_layer_a(dropout_ratio, fc_6_out, drop_6_out);
	F7.fc_layer_a(activation_type, fc_6_out, fc_7_weight_a, fc_7_bias_a, fc_7_out);
	D7.dropout_layer_a(dropout_ratio, fc_7_out, drop_7_out);
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
