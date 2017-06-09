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
#include "conv_acc.h"
#include "max_pool_acc.h"

// vgg16 inference function
void inference_net(

	char activation_type,

	// input pic data
	data_type in_data_3D[3*224*224],
/*
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
*/
	/********************************************************/
    data_type_w *conv_weight_port,
    data_type_w *conv_bias_port,
    data_type_w *fc_weight_port,
    data_type_w *fc_bias_port,
    /********************************************************/
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

	//construct network --------------vgg16
	//conv_layer<data_type,data_type_w,data_type_o, 224, 3, 1, 1, 3, 64, 1> C1_1;
	conv_acc<data_type, data_type_w, data_type_o, 64, 7, 224, 224> convAcc1;//{0<Tm<=M;0<Tn<=N;0<Tr<=R;0<Tc<=C;}
	//conv_layer<data_type,data_type_w,data_type_o, 224, 3, 1, 1, 64, 64, 1> C1_2;
	//pool_layer<data_type,data_type_w,data_type_o, 224, 2, 0, 2, 64> P1;
	max_pool_acc<data_type, data_type_w, data_type_o, 64, 112, 112> maxPoolAcc1;//{0<Tm<=M;0<Tn<=N;0<Tr<=R;0<Tc<=C;}
	//conv_layer<data_type,data_type_w,data_type_o, 112, 3, 1, 1, 64, 128, 1> C2_1;
	//conv_layer<data_type,data_type_w,data_type_o, 112, 3, 1, 1, 128, 128, 1> C2_2;
	//pool_layer<data_type,data_type_w,data_type_o, 112, 2, 0, 2, 128> P2;
	//conv_layer<data_type,data_type_w,data_type_o, 56, 3, 1, 1, 128, 256, 1> C3_1;
	//conv_layer<data_type,data_type_w,data_type_o, 56, 3, 1, 1, 256, 256, 1> C3_2;
	//conv_layer<data_type,data_type_w,data_type_o, 56, 3, 1, 1, 256, 256, 1> C3_3;
	//pool_layer<data_type,data_type_w,data_type_o, 56, 2, 0, 2, 256> P3;
	//conv_layer<data_type,data_type_w,data_type_o, 28, 3, 1, 1, 256, 512, 1> C4_1;
	//conv_layer<data_type,data_type_w,data_type_o, 28, 3, 1, 1, 512, 512, 1> C4_2;
	//conv_layer<data_type,data_type_w,data_type_o, 28, 3, 1, 1, 512, 512, 1> C4_3;
	//pool_layer<data_type,data_type_w,data_type_o, 28, 2, 0, 2, 512> P4;
	//conv_layer<data_type,data_type_w,data_type_o, 14, 3, 1, 1, 512, 512, 1> C5_1;
	//conv_layer<data_type,data_type_w,data_type_o, 14, 3, 1, 1, 512, 512, 1> C5_2;
	//conv_layer<data_type,data_type_w,data_type_o, 14, 3, 1, 1, 512, 512, 1> C5_3;
	//pool_layer<data_type,data_type_w,data_type_o, 14, 2, 0, 2, 512> P5;
	fc_layer<data_type,data_type_w,data_type_o, 512, 7, 4096> F6;
	/*dropout_layer<float, 4096, 1> D6;*/
	fc_layer<data_type,data_type_w,data_type_o, 4096, 1, 4096> F7;
	/*dropout_layer<float, 4096, 1> D7;*/
	fc_layer<data_type,data_type_w,data_type_o, 4096, 1, 1000> F8;

	//temp storage space
	data_type in_data_buf[3*224*224];
	data_type_o fc_8_out_buf[1000];
    data_type_o  output_temp_1[64*224*224];
    data_type_o  output_temp_2[64*224*224];

    TRANS_DATA: for(int i = 0; i < 3; i++){
	    for(int j = 0; j < 224; j++){
		for(int k = 0; k < 224; k++){
		    in_data_buf[i*224*224 + j*224 + k] = in_data_3D[i*224*224 + j*224 + k];
	        }
	    }
	}

	//weight&bias offset each layer
	int conv_1_2_weight_offset = 192*3*3;
	int conv_1_2_bias_offset = 64;
	int conv_2_1_weight_offset = conv_1_2_weight_offset + 4096*3*3;
	int conv_2_1_bias_offset = conv_1_2_bias_offset + 64;
	int conv_2_2_weight_offset = conv_2_1_weight_offset + 8192*3*3;
	int conv_2_2_bias_offset = conv_2_1_bias_offset + 128;
	int conv_3_1_weight_offset = conv_2_2_weight_offset + 16384*3*3;
	int conv_3_1_bias_offset = conv_2_2_bias_offset + 128;
	int conv_3_2_weight_offset = conv_3_1_weight_offset + 32768*3*3;
	int conv_3_2_bias_offset = conv_3_1_bias_offset + 256;
	int conv_3_3_weight_offset = conv_3_2_weight_offset + 65536*3*3;
	int conv_3_3_bias_offset = conv_3_2_bias_offset + 256;
	int conv_4_1_weight_offset = conv_3_3_weight_offset + 65536*3*3;
	int conv_4_1_bias_offset = conv_3_3_bias_offset + 256;
	int conv_4_2_weight_offset = conv_4_1_weight_offset + 131072*3*3;
	int conv_4_2_bias_offset = conv_4_1_bias_offset + 512;
	int conv_4_3_weight_offset = conv_4_2_weight_offset + 262144*3*3;
	int conv_4_3_bias_offset = conv_4_2_bias_offset + 512;
	int conv_5_1_weight_offset = conv_4_3_weight_offset + 262144*3*3;
	int conv_5_1_bias_offset = conv_4_3_bias_offset + 512;
	int conv_5_2_weight_offset = conv_5_1_weight_offset + 262144*3*3;
	int conv_5_2_bias_offset = conv_5_1_bias_offset + 512;
	int conv_5_3_weight_offset = conv_5_2_weight_offset + 262144*3*3;
	int conv_5_3_bias_offset = conv_5_2_bias_offset + 512;
	int fc_7_weight_offset = 2097152*7*7;
	int fc_7_bias_offset = 4096;
	int fc_8_weight_offset = fc_7_weight_offset + 16777216*1*1;
	int fc_8_bias_offset = fc_7_bias_offset + 4096;

	//internal memory initiallization:must do it!
	RESET_fc_8_out_buf: for(int i = 0; i < 1000; i++){
        fc_8_out_buf[i] = data_type_o(0);
    }
    RESET_temp:for(int addr = 0; addr < 64*224*224; addr++){
        output_temp_1[addr] = data_type_o(0);
        output_temp_2[addr] = data_type_o(0);
    }

	//Forward propagation by layer
	//C1_1.conv_layer_a(activation_type, in_data_3D, conv_1_1_weight_a, conv_1_1_bias_a, output_temp_1);
	convAcc1.conv_layer_acc(3, 3, 64, 224, 224, 1, 1, in_data_buf,conv_weight_port, conv_bias_port, output_temp_1);
	//C1_2.conv_layer_a(activation_type, output_temp_1, conv_1_2_weight_a, conv_1_2_bias_a, output_temp_2);
	convAcc1.conv_layer_acc(64, 3, 64, 224, 224, 1, 1, output_temp_1,conv_weight_port+conv_1_2_weight_offset, conv_bias_port+conv_1_2_bias_offset, output_temp_2);
    for(int i = 0; i < 64*224*224; i++){
            output_temp_1[i] = (data_type_o)(0);
}
	//P1.max_pooling_layer_a(activation_type, output_temp_2, output_temp_1);
	maxPoolAcc1.max_pool_layer_acc(64, 2, 112, 112, 2, 0, output_temp_2, output_temp_1);
    for(int i = 0; i < 64*224*224; i++){
            output_temp_2[i] = (data_type_o)(0);
}
	//C2_1.conv_layer_a(activation_type, output_temp_1, conv_2_1_weight_a, conv_2_1_bias_a, output_temp_2);
	convAcc1.conv_layer_acc(64, 3, 128, 112, 112, 1, 1, output_temp_1,conv_weight_port+conv_2_1_weight_offset, conv_bias_port+conv_2_1_bias_offset, output_temp_2);
    for(int i = 0; i < 64*224*224; i++){
            output_temp_1[i] = (data_type_o)(0);
}
	//C2_2.conv_layer_a(activation_type, output_temp_2, conv_2_2_weight_a, conv_2_2_bias_a, output_temp_1);
	convAcc1.conv_layer_acc(128, 3, 128, 112, 112, 1, 1, output_temp_2,conv_weight_port+conv_2_2_weight_offset, conv_bias_port+conv_2_2_bias_offset, output_temp_1);
    for(int i = 0; i < 64*224*224; i++){
            output_temp_2[i] = (data_type_o)(0);
}
	//P2.max_pooling_layer_a(activation_type, output_temp_1, output_temp_2);
	maxPoolAcc1.max_pool_layer_acc(128, 2, 56, 56, 2, 0, output_temp_1, output_temp_2);
    for(int i = 0; i < 64*224*224; i++){
            output_temp_1[i] = (data_type_o)(0);
}
	//C3_1.conv_layer_a(activation_type, output_temp_2, conv_3_1_weight_a, conv_3_1_bias_a, output_temp_1);
	convAcc1.conv_layer_acc(128, 3, 256, 56, 56, 1, 1, output_temp_2,conv_weight_port+conv_3_1_weight_offset, conv_bias_port+conv_3_1_bias_offset, output_temp_1);
    for(int i = 0; i < 64*224*224; i++){
            output_temp_2[i] = (data_type_o)(0);
}
	//C3_2.conv_layer_a(activation_type, output_temp_1, conv_3_2_weight_a, conv_3_2_bias_a, output_temp_2);
	convAcc1.conv_layer_acc(256, 3, 256, 56, 56, 1, 1, output_temp_1,conv_weight_port+conv_3_2_weight_offset, conv_bias_port+conv_3_2_bias_offset, output_temp_2);
    for(int i = 0; i < 64*224*224; i++){
            output_temp_1[i] = (data_type_o)(0);
}
	//C3_3.conv_layer_a(activation_type, output_temp_2, conv_3_3_weight_a, conv_3_3_bias_a, output_temp_1);
	convAcc1.conv_layer_acc(256, 3, 256, 56, 56, 1, 1, output_temp_2,conv_weight_port+conv_3_3_weight_offset, conv_bias_port+conv_3_3_bias_offset, output_temp_1);
    for(int i = 0; i < 64*224*224; i++){
            output_temp_2[i] = (data_type_o)(0);
}
	//P3.max_pooling_layer_a(activation_type, output_temp_1, output_temp_2);
	maxPoolAcc1.max_pool_layer_acc(256, 2, 28, 28, 2, 0, output_temp_1, output_temp_2);
    for(int i = 0; i < 64*224*224; i++){
            output_temp_1[i] = (data_type_o)(0);
}
	//C4_1.conv_layer_a(activation_type, output_temp_2, conv_4_1_weight_a, conv_4_1_bias_a, output_temp_1);
	convAcc1.conv_layer_acc(256, 3, 512, 28, 28, 1, 1, output_temp_2,conv_weight_port+conv_4_1_weight_offset, conv_bias_port+conv_4_1_bias_offset, output_temp_1);
    for(int i = 0; i < 64*224*224; i++){
            output_temp_2[i] = (data_type_o)(0);
}
	//C4_2.conv_layer_a(activation_type, output_temp_1, conv_4_2_weight_a, conv_4_2_bias_a, output_temp_2);
	convAcc1.conv_layer_acc(512, 3, 512, 28, 28, 1, 1, output_temp_1,conv_weight_port+conv_4_2_weight_offset, conv_bias_port+conv_4_2_bias_offset, output_temp_2);
    for(int i = 0; i < 64*224*224; i++){
            output_temp_1[i] = (data_type_o)(0);
}
	//C4_3.conv_layer_a(activation_type, output_temp_2, conv_4_3_weight_a, conv_4_3_bias_a, output_temp_1);
	convAcc1.conv_layer_acc(512, 3, 512, 28, 28, 1, 1, output_temp_2,conv_weight_port+conv_4_3_weight_offset, conv_bias_port+conv_4_3_bias_offset, output_temp_1);
    for(int i = 0; i < 64*224*224; i++){
            output_temp_2[i] = (data_type_o)(0);
}
	//P4.max_pooling_layer_a(activation_type, output_temp_1, output_temp_2);
	maxPoolAcc1.max_pool_layer_acc(512, 2, 14, 14, 2, 0, output_temp_1, output_temp_2);
    for(int i = 0; i < 64*224*224; i++){
            output_temp_1[i] = (data_type_o)(0);
}
	//C5_1.conv_layer_a(activation_type, output_temp_2, conv_5_1_weight_a, conv_5_1_bias_a, output_temp_1);
	convAcc1.conv_layer_acc(512, 3, 512, 14, 14, 1, 1, output_temp_2,conv_weight_port+conv_5_1_weight_offset, conv_bias_port+conv_5_1_bias_offset, output_temp_1);
    for(int i = 0; i < 64*224*224; i++){
            output_temp_2[i] = (data_type_o)(0);
}
	//C5_2.conv_layer_a(activation_type, output_temp_1, conv_5_2_weight_a, conv_5_2_bias_a, output_temp_2);
	convAcc1.conv_layer_acc(512, 3, 512, 14, 14, 1, 1, output_temp_1,conv_weight_port+conv_5_2_weight_offset, conv_bias_port+conv_5_2_bias_offset, output_temp_2);
    for(int i = 0; i < 64*224*224; i++){
            output_temp_1[i] = (data_type_o)(0);
}
	//C5_3.conv_layer_a(activation_type, output_temp_2, conv_5_3_weight_a, conv_5_3_bias_a, output_temp_1);
	convAcc1.conv_layer_acc(512, 3, 512, 14, 14, 1, 1, output_temp_2,conv_weight_port+conv_5_3_weight_offset, conv_bias_port+conv_5_3_bias_offset, output_temp_1);
    for(int i = 0; i < 64*224*224; i++){
            output_temp_2[i] = (data_type_o)(0);
}
	//P5.max_pooling_layer_a(activation_type, output_temp_1, output_temp_2);
	maxPoolAcc1.max_pool_layer_acc(512, 2, 7, 7, 2, 0, output_temp_1, output_temp_2);
    for(int i = 0; i < 64*224*224; i++){
            output_temp_1[i] = (data_type_o)(0);
}
	F6.fc_layer_a(activation_type, output_temp_2, fc_weight_port, fc_bias_port, output_temp_1);
    for(int i = 0; i < 64*224*224; i++){
            output_temp_2[i] = (data_type_o)(0);
}
	/*D6.dropout_layer_a(dropout_ratio, fc_6_out, drop_6_out);*/
	F7.fc_layer_a(activation_type, output_temp_1, fc_weight_port+fc_7_weight_offset, fc_bias_port+fc_7_bias_offset, output_temp_2);
	/*D7.dropout_layer_a(dropout_ratio, fc_7_out, drop_7_out);*/
	F8.fc_layer_a_no_activation(output_temp_2, fc_weight_port+fc_8_weight_offset, fc_bias_port+fc_8_bias_offset, fc_8_out_buf);
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
