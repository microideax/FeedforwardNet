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
	data_type in_data_3D[3*224*224],

	// layer weights and bias inputs ------- vgg16
	data_type_w        conv_1_1_weight_a[192*3*3],
	data_type_w        conv_1_1_bias_a[64],
	data_type_w        conv_1_2_weight_a[4096*3*3],
	data_type_w        conv_1_2_bias_a[64],
	data_type_w        conv_2_1_weight_a[8192*3*3],
	data_type_w        conv_2_1_bias_a[128],
	data_type_w        conv_2_2_weight_a[16384*3*3],
	data_type_w        conv_2_2_bias_a[128],
	data_type_w        conv_3_1_weight_a[32768*3*3],
	data_type_w        conv_3_1_bias_a[256],
	data_type_w        conv_3_2_weight_a[65536*3*3],
	data_type_w        conv_3_2_bias_a[256],
	data_type_w        conv_3_3_weight_a[65536*3*3],
	data_type_w        conv_3_3_bias_a[256],
	data_type_w        conv_4_1_weight_a[131072*3*3],
	data_type_w        conv_4_1_bias_a[512],
	data_type_w        conv_4_2_weight_a[262144*3*3],
	data_type_w        conv_4_2_bias_a[512],
	data_type_w        conv_4_3_weight_a[262144*3*3],
	data_type_w        conv_4_3_bias_a[512],
	data_type_w        conv_5_1_weight_a[262144*3*3],
	data_type_w        conv_5_1_bias_a[512],
	data_type_w        conv_5_2_weight_a[262144*3*3],
	data_type_w        conv_5_2_bias_a[512],
	data_type_w        conv_5_3_weight_a[262144*3*3],
	data_type_w        conv_5_3_bias_a[512],
	data_type_w        fc_6_weight_a[2097152*7*7],
	data_type_w        fc_6_bias_a[4096],
	data_type_w        fc_7_weight_a[16777216*1*1],
	data_type_w        fc_7_bias_a[4096],
	data_type_w        fc_8_weight_a[4096000*1*1],
	data_type_w        fc_8_bias_a[1000],

	// output fc data
	data_type fc_8_out_a[1000*1*1]
) {


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
	cout << "starting forward network process..........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif

	/******************************************************************************************/

	//construct network --------------vgg16
	conv_layer<data_type,data_type_w,data_type_o, 224, 3, 1, 1, 3, 64, 1> C1_1;
	conv_layer<data_type,data_type_w,data_type_o, 224, 3, 1, 1, 64, 64, 1> C1_2;
	pool_layer<data_type,data_type_w,data_type_o, 224, 2, 0, 2, 64> P1;
	conv_layer<data_type,data_type_w,data_type_o, 112, 3, 1, 1, 64, 128, 1> C2_1;
	conv_layer<data_type,data_type_w,data_type_o, 112, 3, 1, 1, 128, 128, 1> C2_2;
	pool_layer<data_type,data_type_w,data_type_o, 112, 2, 0, 2, 128> P2;
	conv_layer<data_type,data_type_w,data_type_o, 56, 3, 1, 1, 128, 256, 1> C3_1;
	conv_layer<data_type,data_type_w,data_type_o, 56, 3, 1, 1, 256, 256, 1> C3_2;
	conv_layer<data_type,data_type_w,data_type_o, 56, 3, 1, 1, 256, 256, 1> C3_3;
	pool_layer<data_type,data_type_w,data_type_o, 56, 2, 0, 2, 256> P3;
	conv_layer<data_type,data_type_w,data_type_o, 28, 3, 1, 1, 256, 512, 1> C4_1;
	conv_layer<data_type,data_type_w,data_type_o, 28, 3, 1, 1, 512, 512, 1> C4_2;
	conv_layer<data_type,data_type_w,data_type_o, 28, 3, 1, 1, 512, 512, 1> C4_3;
	pool_layer<data_type,data_type_w,data_type_o, 28, 2, 0, 2, 512> P4;
	conv_layer<data_type,data_type_w,data_type_o, 14, 3, 1, 1, 512, 512, 1> C5_1;
	conv_layer<data_type,data_type_w,data_type_o, 14, 3, 1, 1, 512, 512, 1> C5_2;
	conv_layer<data_type,data_type_w,data_type_o, 14, 3, 1, 1, 512, 512, 1> C5_3;
	pool_layer<data_type,data_type_w,data_type_o, 14, 2, 0, 2, 512> P5;
	fc_layer<data_type,data_type_w,data_type_o, 512, 7, 4096> F6;
	/*dropout_layer<float, 4096, 1> D6;*/
	fc_layer<data_type,data_type_w,data_type_o, 4096, 1, 4096> F7;
	/*dropout_layer<float, 4096, 1> D7;*/
	fc_layer<data_type,data_type_w,data_type_o, 4096, 1, 1000> F8;

	//temp storage space
    data_type_o  output_1[64*224*224];
    data_type_o  output_2[64*224*224];
	/*float  drop_6_out[4096][1][1] = { 0 };*/
	//data_type  fc_7_out[4096][1][1];
	/*float  drop_7_out[4096][1][1] = { 0 };*/

	//internal memory initiallization
	for(int i = 0; i < 64*224*224; i++){
            output_1[i] = (data_type_o)(0);
            output_2[i] = (data_type_o)(0);
    }

	//Forward propagation by layer
	C1_1.conv_layer_a(activation_type, in_data_3D, conv_1_1_weight_a, conv_1_1_bias_a, output_1);
	C1_2.conv_layer_a(activation_type, output_1, conv_1_2_weight_a, conv_1_2_bias_a, output_2);
    for(int i = 0; i < 64*224*224; i++){
            output_1[i] = (data_type_o)(0);
}
	P1.max_pooling_layer_a(activation_type, output_2, output_1);
    for(int i = 0; i < 64*224*224; i++){
            output_2[i] = (data_type_o)(0);
}
	C2_1.conv_layer_a(activation_type, output_1, conv_2_1_weight_a, conv_2_1_bias_a, output_2);
    for(int i = 0; i < 64*224*224; i++){
            output_1[i] = (data_type_o)(0);
}
	C2_2.conv_layer_a(activation_type, output_2, conv_2_2_weight_a, conv_2_2_bias_a, output_1);
    for(int i = 0; i < 64*224*224; i++){
            output_2[i] = (data_type_o)(0);
}
	P2.max_pooling_layer_a(activation_type, output_1, output_2);
    for(int i = 0; i < 64*224*224; i++){
            output_1[i] = (data_type_o)(0);
}
	C3_1.conv_layer_a(activation_type, output_2, conv_3_1_weight_a, conv_3_1_bias_a, output_1);
    for(int i = 0; i < 64*224*224; i++){
            output_2[i] = (data_type_o)(0);
}
	C3_2.conv_layer_a(activation_type, output_1, conv_3_2_weight_a, conv_3_2_bias_a, output_2);
    for(int i = 0; i < 64*224*224; i++){
            output_1[i] = (data_type_o)(0);
}
	C3_3.conv_layer_a(activation_type, output_2, conv_3_3_weight_a, conv_3_3_bias_a, output_1);
    for(int i = 0; i < 64*224*224; i++){
            output_2[i] = (data_type_o)(0);
}
	P3.max_pooling_layer_a(activation_type, output_1, output_2);
    for(int i = 0; i < 64*224*224; i++){
            output_1[i] = (data_type_o)(0);
}
	C4_1.conv_layer_a(activation_type, output_2, conv_4_1_weight_a, conv_4_1_bias_a, output_1);
    for(int i = 0; i < 64*224*224; i++){
            output_2[i] = (data_type_o)(0);
}
	C4_2.conv_layer_a(activation_type, output_1, conv_4_2_weight_a, conv_4_2_bias_a, output_2);
    for(int i = 0; i < 64*224*224; i++){
            output_1[i] = (data_type_o)(0);
}
	C4_3.conv_layer_a(activation_type, output_2, conv_4_3_weight_a, conv_4_3_bias_a, output_1);
    for(int i = 0; i < 64*224*224; i++){
            output_2[i] = (data_type_o)(0);
}
	P4.max_pooling_layer_a(activation_type, output_1, output_2);
    for(int i = 0; i < 64*224*224; i++){
            output_1[i] = (data_type_o)(0);
}
	C5_1.conv_layer_a(activation_type, output_2, conv_5_1_weight_a, conv_5_1_bias_a, output_1);
    for(int i = 0; i < 64*224*224; i++){
            output_2[i] = (data_type_o)(0);
}
	C5_2.conv_layer_a(activation_type, output_1, conv_5_2_weight_a, conv_5_2_bias_a, output_2);
    for(int i = 0; i < 64*224*224; i++){
            output_1[i] = (data_type_o)(0);
}
	C5_3.conv_layer_a(activation_type, output_2, conv_5_3_weight_a, conv_5_3_bias_a, output_1);
    for(int i = 0; i < 64*224*224; i++){
            output_2[i] = (data_type_o)(0);
}
	P5.max_pooling_layer_a(activation_type, output_1, output_2);
    for(int i = 0; i < 64*224*224; i++){
            output_1[i] = (data_type_o)(0);
}
	F6.fc_layer_a(activation_type, output_2, fc_6_weight_a, fc_6_bias_a, output_1);
    for(int i = 0; i < 64*224*224; i++){
            output_2[i] = (data_type_o)(0);
}
	/*D6.dropout_layer_a(dropout_ratio, fc_6_out, drop_6_out);*/
	F7.fc_layer_a(activation_type, output_1, fc_7_weight_a, fc_7_bias_a, output_2);
	/*D7.dropout_layer_a(dropout_ratio, fc_7_out, drop_7_out);*/
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
