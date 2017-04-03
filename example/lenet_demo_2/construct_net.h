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
#include "../../fpga_cnn/data_type.h"
#include "../../fpga_cnn/weight_bias.h"
#include "../../fpga_cnn/activation_functions.h"
#include "../../fpga_cnn/conv_pool_layer.h"
#include "../../fpga_cnn/conv_layer.h"
#include "../../fpga_cnn/pool_layer.h"
#include "../../fpga_cnn/fc_layer.h"

bool has_connection_table[3] = { false, true, false };

void inference_net(

        char activation_type,

        // input pic data
        float in_data_3D[1][32][32],

	// layer weights and bias inputs ------- LeNet-5
	float        conv_1_weight_a[6][5][5],
	float        conv_1_bias_a[6],
	float        pool_1_weight_a[24],
	float        pool_1_bias_a[6],
	float        conv_2_weight_a[96][5][5],
	float        conv_2_bias_a[16],
	float        pool_2_weight_a[64],
	float        pool_2_bias_a[16],
	float        conv_3_weight_a[1920][5][5],
	float        conv_3_bias_a[120],
	float        fc_1_weight_a[1200][1][1],
	float        fc_1_bias_a[10],

        // output fc data
        float fc_1_out_a[10][1][1]
) {


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    cout << "starting forward network process..........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif


	/*****************************************************************************************/
	//construct network --------------LeNet-5
	conv_layer<float, 32, 5, 0, 1, 1, 6, 1> C1;      //1@32x32-in, 6@28x28-out
	pool_layer<float, 28, 2, 0, 2, 6> P2;         //6@28x28-in, 6@14x14-out
	conv_layer<float, 14, 5, 0, 1, 6, 16, 1> C3;     //6@14x14-in, 16@10x10-out
	pool_layer<float, 10, 2, 0, 2, 16> P4;        //16@10x10-in, 16@5x5-out
	conv_layer<float, 5, 5, 0, 1, 16, 120, 1> C5;    //16@5x5-in, 120@1x1-out
	fc_layer<float, 120, 1, 10> F6;         //120@1x1-in, 10@1x1-out

											//temp storage space for LeNet-5
	float  conv_1_out_a[6][28][28] = { 0 };
	float  pool_1_out_a[6][14][14] = { 0 };
	float  conv_2_out_a[16][10][10] = { 0 };
	float  pool_2_out_a[16][5][5] = { 0 };
	float  conv_3_out_a[120][1][1] = { 0 };

	//Forward propagation process
	C1.conv_layer_a(activation_type, in_data_3D, conv_1_weight_a, conv_1_bias_a, conv_1_out_a);
	P2.pooling_layer_a(activation_type, conv_1_out_a, pool_1_weight_a, pool_1_bias_a, pool_1_out_a);
	C3.conv_layer_a_table(activation_type, has_connection_table[1], pool_1_out_a, conv_2_weight_a, conv_2_bias_a, conv_2_out_a);
	P4.pooling_layer_a(activation_type, conv_2_out_a, pool_2_weight_a, pool_2_bias_a, pool_2_out_a);
	C5.conv_layer_a(activation_type, pool_2_out_a, conv_3_weight_a, conv_3_bias_a, conv_3_out_a);
	F6.fc_layer_a(activation_type, conv_3_out_a, fc_1_weight_a, fc_1_bias_a, fc_1_out_a);
	/******************************************************************************************/


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    cout << "Finished forward network process .........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif

}

#endif //_CONSTRUCT_NET_H_
