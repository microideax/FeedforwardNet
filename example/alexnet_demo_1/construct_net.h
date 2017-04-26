//
// Created by yaochen on 24/12/16.
//

#ifndef _CONSTRUCT_NET_H_
#define _CONSTRUCT_NET_H_

#include <iostream>
#include <ap_fixed.h>
#include "config.h"
//#include "weight_bias.h"
#include "../../fpga_cnn/conv_pool_layer.h"
#include "../../fpga_cnn/conv_layer_one_dim.h"
#include "../../fpga_cnn/pool_layer_one_dim.h"
#include "../../fpga_cnn/fc_layer_one_dim.h"
//#include "../../fpga_cnn/dropout_layer.h"
#include "../../fpga_cnn/lrn_layer_one_dim.h"

void inference_net(

	char activation_type,

	// input pic data
	data_type in_data_3D[3*227*227],

	// layer weights and bias inputs ------- Alexnet
	data_type_w        conv_1_weight_a[288*11*11],
	data_type_w        conv_1_bias_a[96],
	data_type_w        conv_2_weight_a[12288*5*5],
	data_type_w        conv_2_bias_a[256],
	data_type_w        conv_3_weight_a[98304*3*3],
	data_type_w        conv_3_bias_a[384],
	data_type_w        conv_4_weight_a[73728*3*3],
	data_type_w        conv_4_bias_a[384],
	data_type_w        conv_5_weight_a[49152*3*3],
	data_type_w        conv_5_bias_a[256],
	data_type_w        fc_6_weight_a[1048576*6*6],
	data_type_w        fc_6_bias_a[4096],
	data_type_w        fc_7_weight_a[16777216*1*1],
	data_type_w        fc_7_bias_a[4096],
	data_type_w        fc_8_weight_a[4096000*1*1],
	data_type_w        fc_8_bias_a[1000],

	// output fc data
	data_type_o fc_8_out_a[1000*1*1]
) {


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
	/*dropout_layer<float, 4096, 1> D6;*/
	fc_layer<data_type,data_type_w,data_type_o, 4096, 1, 4096> F7;
	/*dropout_layer<float, 4096, 1> D7;*/
	fc_layer<data_type,data_type_w,data_type_o, 4096, 1, 1000> F8;

	//temp storage space
	data_type_o  output_1[96*55*55];
    data_type_o  output_2[96*55*55];
    //internal memory initiallization
    for(int i = 0; i < 96*55*55; i++){
            output_1[i] = (data_type_o)(0);
            output_2[i] = (data_type_o)(0);
    }

	//Forward propagation by layer
	C1.conv_layer_a(activation_type, in_data_3D, conv_1_weight_a, conv_1_bias_a, output_1);
    L1.lrn_layer_a(nn_alpha_lrn[0], nn_beta_lrn[0], output_1, output_2);
    for(int i = 0; i < 96*55*55; i++){
            output_1[i] = (data_type_o)(0);
    }
    P1.max_pooling_layer_a(activation_type, output_2, output_1);
    for(int i = 0; i < 96*55*55; i++){
            output_2[i] = (data_type_o)(0);
    }
	C2.conv_layer_a(activation_type, output_1, conv_2_weight_a, conv_2_bias_a, output_2);
    for(int i = 0; i < 96*55*55; i++){
            output_1[i] = (data_type_o)(0);
    }
	L2.lrn_layer_a(nn_alpha_lrn[1], nn_beta_lrn[1], output_2, output_1);
	for(int i = 0; i < 96*55*55; i++){
            output_2[i] = (data_type_o)(0);
    }
    P2.max_pooling_layer_a(activation_type, output_1, output_2);
    for(int i = 0; i < 96*55*55; i++){
            output_1[i] = (data_type_o)(0);
    }
	C3.conv_layer_a(activation_type, output_2, conv_3_weight_a, conv_3_bias_a, output_1);
    for(int i = 0; i < 96*55*55; i++){
            output_2[i] = (data_type_o)(0);
    }
	C4.conv_layer_a(activation_type, output_1, conv_4_weight_a, conv_4_bias_a, output_2);
    for(int i = 0; i < 96*55*55; i++){
            output_1[i] = (data_type_o)(0);
    }
	C5.conv_layer_a(activation_type, output_2, conv_5_weight_a, conv_5_bias_a, output_1);
    for(int i = 0; i < 96*55*55; i++){
            output_2[i] = (data_type_o)(0);
    }
	P5.max_pooling_layer_a(activation_type, output_1, output_2);
    for(int i = 0; i < 96*55*55; i++){
            output_1[i] = (data_type_o)(0);
    }
	F6.fc_layer_a(activation_type, output_2, fc_6_weight_a, fc_6_bias_a, output_1);
    for(int i = 0; i < 96*55*55; i++){
            output_2[i] = (data_type_o)(0);
    }
	F7.fc_layer_a(activation_type, output_1, fc_7_weight_a, fc_7_bias_a, output_2);
    for(int i = 0; i < 96*55*55; i++){
            output_1[i] = (data_type_o)(0);
    }
	F8.fc_layer_a_no_activation(output_2, fc_8_weight_a, fc_8_bias_a, fc_8_out_a);
    /******************************************************************************************/


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
	cout << "Finished forward network process .........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif

}

#endif //_CONSTRUCT_NET_H_
