//
// Created by yaochen on 24/12/16.
//

#ifndef _CONSTRUCT_NET_H_
#define _CONSTRUCT_NET_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

#include "config.h"
#include "data_type.h"
#include "weight_bias.h"
#include "activation_functions.h"
#include "conv_pool_layer.h"
#include "conv_layer.h"
#include "pool_layer.h"
#include "fc_layer.h"


void inference_net(

	char activation_type,

	// input pic data
	float in_data_3D[1][32][32],

	// layer weights and bias inputs ------- conv2pool2fc
	float        conv_1_weight_a[6][5][5],
	float        conv_1_bias_a[6],
	float        conv_2_weight_a[96][5][5],
	float        conv_2_bias_a[16],
	float        fc_1_weight_a[160][5][5],
	float        fc_1_bias_a[10],

	// output fc data
	float fc_1_out_a[10]
) {


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
	cout << "starting forward network process..........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif


	/******************************************************************************************/
	//construct network --------------conv(1-6) + max Pooling + conv(6-16) + max pooling + fc
    conv_pool_layer<float, 32, 5, 0, 1, 2, 0, 2, 1, 6> C1P2;
	conv_pool_layer<float, 14, 5, 0, 1, 2, 0, 2, 6, 16> C3P4;
	fc_layer<float, 16, 5, 10> F5;

	//temp storage space
	float  pool_1_out[6][14][14] = { 0 };
	float  pool_2_out[16][5][5] = { 0 };

	//Forward propagation process
	C1P2.conv_layer_w_pool_a(activation_type, in_data_3D, conv_1_weight_a, conv_1_bias_a, pool_1_out);
	C3P4.conv_layer_w_pool_a(activation_type, pool_1_out, conv_2_weight_a, conv_2_bias_a, pool_2_out);
	F5.fc_layer_a(activation_type, pool_2_out, fc_1_weight_a, fc_1_bias_a, fc_1_out_a);

	/******************************************************************************************/


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
	cout << "Finished forward network process .........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif

}

#endif //_CONSTRUCT_NET_H_
