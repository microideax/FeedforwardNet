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
#include "../../fpga_cnn/conv_layer.h"
#include "../../fpga_cnn/pool_layer.h"
#include "../../fpga_cnn/fc_layer.h"
//#include "../../fpga_cnn/dropout_layer.h"
#include "../../fpga_cnn/lrn_layer.h"

void inference_net(

	char activation_type,

	// input pic data
	data_type in_data_3D[3][224][224],

	// layer weights and bias inputs ------- vgg16
	data_type        conv_1_1_weight_a[192][3][3],
	data_type        conv_1_1_bias_a[64],
	data_type        conv_1_2_weight_a[4096][3][3],
	data_type        conv_1_2_bias_a[64],
	data_type        conv_2_1_weight_a[8192][3][3],
	data_type        conv_2_1_bias_a[128],
	data_type        conv_2_2_weight_a[16384][3][3],
	data_type        conv_2_2_bias_a[128],
	data_type        conv_3_1_weight_a[32768][3][3],
	data_type        conv_3_1_bias_a[256],
	data_type        conv_3_2_weight_a[65536][3][3],
	data_type        conv_3_2_bias_a[256],
	data_type        conv_3_3_weight_a[65536][3][3],
	data_type        conv_3_3_bias_a[256],
	data_type        conv_4_1_weight_a[131072][3][3],
	data_type        conv_4_1_bias_a[512],
	data_type        conv_4_2_weight_a[262144][3][3],
	data_type        conv_4_2_bias_a[512],
	data_type        conv_4_3_weight_a[262144][3][3],
	data_type        conv_4_3_bias_a[512],
	data_type        conv_5_1_weight_a[262144][3][3],
	data_type        conv_5_1_bias_a[512],
	data_type        conv_5_2_weight_a[262144][3][3],
	data_type        conv_5_2_bias_a[512],
	data_type        conv_5_3_weight_a[262144][3][3],
	data_type        conv_5_3_bias_a[512],
	data_type        fc_6_weight_a[2097152][7][7],
	data_type        fc_6_bias_a[4096],
	data_type        fc_7_weight_a[16777216][1][1],
	data_type        fc_7_bias_a[4096],
	data_type        fc_8_weight_a[4096000][1][1],
	data_type        fc_8_bias_a[1000],

	// output fc data
	data_type fc_8_out_a[1000][1][1]
) {


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
	cout << "starting forward network process..........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif

	/******************************************************************************************/

	//construct network --------------vgg16
	conv_layer<data_type, 224, 3, 1, 1, 3, 64, 1> C1_1;
	conv_layer<data_type, 224, 3, 1, 1, 64, 64, 1> C1_2;
	pool_layer<data_type, 224, 2, 0, 2, 64> P1;
	conv_layer<data_type, 112, 3, 1, 1, 64, 128, 1> C2_1;
	conv_layer<data_type, 112, 3, 1, 1, 128, 128, 1> C2_2;
	pool_layer<data_type, 112, 2, 0, 2, 128> P2;
	conv_layer<data_type, 56, 3, 1, 1, 128, 256, 1> C3_1;
	conv_layer<data_type, 56, 3, 1, 1, 256, 256, 1> C3_2;
	conv_layer<data_type, 56, 3, 1, 1, 256, 256, 1> C3_3;
	pool_layer<data_type, 56, 2, 0, 2, 256> P3;
	conv_layer<data_type, 28, 3, 1, 1, 256, 512, 1> C4_1;
	conv_layer<data_type, 28, 3, 1, 1, 512, 512, 1> C4_2;
	conv_layer<data_type, 28, 3, 1, 1, 512, 512, 1> C4_3;
	pool_layer<data_type, 28, 2, 0, 2, 512> P4;
	conv_layer<data_type, 14, 3, 1, 1, 512, 512, 1> C5_1;
	conv_layer<data_type, 14, 3, 1, 1, 512, 512, 1> C5_2;
	conv_layer<data_type, 14, 3, 1, 1, 512, 512, 1> C5_3;
	pool_layer<data_type, 14, 2, 0, 2, 512> P5;
	fc_layer<data_type, 512, 7, 4096> F6;
	/*dropout_layer<float, 4096, 1> D6;*/
	fc_layer<data_type, 4096, 1, 4096> F7;
	/*dropout_layer<float, 4096, 1> D7;*/
	fc_layer<data_type, 4096, 1, 1000> F8;

	//temp storage space
	data_type  conv_1_1_out[64][224][224];
	data_type  conv_1_2_out[64][224][224];
	data_type  pool_1_out[64][112][112];
	data_type  conv_2_1_out[128][112][112];
	data_type  conv_2_2_out[128][112][112];
	data_type  pool_2_out[128][56][56];
	data_type  conv_3_1_out[256][56][56];
	data_type  conv_3_2_out[256][56][56];
	data_type  conv_3_3_out[256][56][56];
	data_type  pool_3_out[256][28][28];
	data_type  conv_4_1_out[512][28][28];
	data_type  conv_4_2_out[512][28][28];
	data_type  conv_4_3_out[512][28][28];
	data_type  pool_4_out[512][14][14];
	data_type  conv_5_1_out[512][14][14];
	data_type  conv_5_2_out[512][14][14];
	data_type  conv_5_3_out[512][14][14];
	data_type  pool_5_out[512][7][7];
	data_type  fc_6_out[4096][1][1];
	/*float  drop_6_out[4096][1][1] = { 0 };*/
	data_type  fc_7_out[4096][1][1];
	/*float  drop_7_out[4096][1][1] = { 0 };*/

	//internal memory initiallization
	for(int i = 0; i < 64; i++){
        for(int j = 0; j < 224; j++){
            for(int k = 0; k < 224; k++){
                conv_1_1_out[i][j][k] = (data_type)(0);
            }
        }
    }
    for(int i = 0; i < 64; i++){
        for(int j = 0; j < 224; j++){
            for(int k = 0; k < 224; k++){
                conv_1_2_out[i][j][k] = (data_type)(0);
            }
        }
    }
    for(int i = 0; i < 64; i++){
        for(int j = 0; j < 112; j++){
            for(int k = 0; k < 112; k++){
                pool_1_out[i][j][k] = (data_type)(0);
            }
        }
    }
    for(int i = 0; i < 128; i++){
        for(int j = 0; j < 112; j++){
            for(int k = 0; k < 112; k++){
                conv_2_1_out[i][j][k] = (data_type)(0);
            }
        }
    }
    for(int i = 0; i < 128; i++){
        for(int j = 0; j < 112; j++){
            for(int k = 0; k < 112; k++){
                conv_2_2_out[i][j][k] = (data_type)(0);
            }
        }
    }
    for(int i = 0; i < 64; i++){
        for(int j = 0; j < 56; j++){
            for(int k = 0; k < 56; k++){
                pool_2_out[i][j][k] = (data_type)(0);
            }
        }
    }
    for(int i = 0; i < 256; i++){
        for(int j = 0; j < 56; j++){
            for(int k = 0; k < 56; k++){
                conv_3_1_out[i][j][k] = (data_type)(0);
            }
        }
    }
    for(int i = 0; i < 256; i++){
        for(int j = 0; j < 56; j++){
            for(int k = 0; k < 56; k++){
                conv_3_2_out[i][j][k] = (data_type)(0);
            }
        }
    }
    for(int i = 0; i < 256; i++){
        for(int j = 0; j < 56; j++){
            for(int k = 0; k < 56; k++){
                conv_3_3_out[i][j][k] = (data_type)(0);
            }
        }
    }
    for(int i = 0; i < 64; i++){
        for(int j = 0; j < 28; j++){
            for(int k = 0; k < 28; k++){
                pool_3_out[i][j][k] = (data_type)(0);
            }
        }
    }
    for(int i = 0; i < 512; i++){
        for(int j = 0; j < 28; j++){
            for(int k = 0; k < 28; k++){
                conv_4_1_out[i][j][k] = (data_type)(0);
            }
        }
    }
    for(int i = 0; i < 512; i++){
        for(int j = 0; j < 28; j++){
            for(int k = 0; k < 28; k++){
                conv_4_2_out[i][j][k] = (data_type)(0);
            }
        }
    }
    for(int i = 0; i < 512; i++){
        for(int j = 0; j < 28; j++){
            for(int k = 0; k < 28; k++){
                conv_4_3_out[i][j][k] = (data_type)(0);
            }
        }
    }
    for(int i = 0; i < 512; i++){
        for(int j = 0; j < 14; j++){
            for(int k = 0; k < 14; k++){
                pool_4_out[i][j][k] = (data_type)(0);
            }
        }
    }
    for(int i = 0; i < 512; i++){
        for(int j = 0; j < 14; j++){
            for(int k = 0; k < 14; k++){
                conv_5_1_out[i][j][k] = (data_type)(0);
            }
        }
    }
    for(int i = 0; i < 512; i++){
        for(int j = 0; j < 14; j++){
            for(int k = 0; k < 14; k++){
                conv_5_2_out[i][j][k] = (data_type)(0);
            }
        }
    }
    for(int i = 0; i < 512; i++){
        for(int j = 0; j < 14; j++){
            for(int k = 0; k < 14; k++){
                conv_5_3_out[i][j][k] = (data_type)(0);
            }
        }
    }
    for(int i = 0; i < 4096; i++){
        for(int j = 0; j < 7; j++){
            for(int k = 0; k < 7; k++){
                pool_5_out[i][j][k] = (data_type)(0);
            }
        }
    }
    for(int i = 0; i < 4096; i++){
        for(int j = 0; j < 1; j++){
            for(int k = 0; k < 1; k++){
                fc_6_out[i][j][k] = (data_type)(0);
            }
        }
    }
    for(int i = 0; i < 512; i++){
        for(int j = 0; j < 1; j++){
            for(int k = 0; k < 1; k++){
                fc_7_out[i][j][k] = (data_type)(0);
            }
        }
    }

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
