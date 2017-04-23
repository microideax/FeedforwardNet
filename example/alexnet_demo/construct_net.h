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
	data_type in_data_3D[3][227][227],

	// layer weights and bias inputs ------- Alexnet
	data_type_w        conv_1_weight_a[288][11][11],
	data_type_w        conv_1_bias_a[96],
	data_type_w        conv_2_weight_a[12288][5][5],
	data_type_w        conv_2_bias_a[256],
	data_type_w        conv_3_weight_a[98304][3][3],
	data_type_w        conv_3_bias_a[384],
	data_type_w        conv_4_weight_a[73728][3][3],
	data_type_w        conv_4_bias_a[384],
	data_type_w        conv_5_weight_a[49152][3][3],
	data_type_w        conv_5_bias_a[256],
	data_type_w        fc_6_weight_a[1048576][6][6],
	data_type_w        fc_6_bias_a[4096],
	data_type_w        fc_7_weight_a[16777216][1][1],
	data_type_w        fc_7_bias_a[4096],
	data_type_w        fc_8_weight_a[4096000][1][1],
	data_type_w        fc_8_bias_a[1000],

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

	//construct network --------------alexnet
	conv_layer<data_type,data_type_w,data_type_o, 227, 11, 0, 4, 3, 96, 1> C1;
    lrn_layer<float, 96 ,5 ,55> L1;
    pool_layer<data_type,data_type_w,data_type_o, 55, 3, 0, 2, 96> P1;
    conv_layer<data_type,data_type_w,data_type_o, 27, 5, 2, 1, 96, 256, 2> C2;
    lrn_layer<float, 256, 5, 27> L2;
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
	data_type  conv_1_out[96][55][55];
	float  conv_1_out_float[96][55][55];
	data_type  lrn_1_out[96][55][55];
	float  lrn_1_out_float[96][55][55];
	data_type  pool_1_out[96][27][27];
	data_type  conv_2_out[256][27][27];
	float  conv_2_out_float[256][27][27];
	data_type  lrn_2_out[256][27][27];
	float  lrn_2_out_float[256][27][27];
	data_type  pool_2_out[256][13][13];
	data_type  conv_3_out[384][13][13];
	data_type  conv_4_out[384][13][13];
	data_type  conv_5_out[256][13][13];
	data_type  pool_5_out[256][6][6];
	data_type  fc_6_out[4096][1][1];
	/*float  drop_6_out[4096][1][1] = { 0 };*/
	data_type  fc_7_out[4096][1][1];
	/*float  drop_7_out[4096][1][1] = { 0 };*/

    //internal memory initiallization
    for(int i = 0; i < 96; i++){
    for(int j = 0; j < 55; j++){
        for(int k = 0; k < 55; k++){
            conv_1_out[i][j][k] = (data_type)(0);
        }
    }
}
for(int i = 0; i < 96; i++){
    for(int j = 0; j < 55; j++){
        for(int k = 0; k < 55; k++){
            conv_1_out_float[i][j][k] = (float)(0);
        }
    }
}
    for(int i = 0; i < 96; i++){
    for(int j = 0; j < 55; j++){
        for(int k = 0; k < 55; k++){
            lrn_1_out[i][j][k] = (data_type)(0);
        }
    }
}
for(int i = 0; i < 96; i++){
    for(int j = 0; j < 55; j++){
        for(int k = 0; k < 55; k++){
            lrn_1_out_float[i][j][k] = (float)(0);
        }
    }
}
for(int i = 0; i < 96; i++){
    for(int j = 0; j < 27; j++){
        for(int k = 0; k < 27; k++){
            pool_1_out[i][j][k] = (data_type)(0);
        }
    }
}
for(int i = 0; i < 256; i++){
    for(int j = 0; j < 27; j++){
        for(int k = 0; k < 27; k++){
            conv_2_out[i][j][k] = (data_type)(0);
        }
    }
}
for(int i = 0; i < 256; i++){
    for(int j = 0; j < 27; j++){
        for(int k = 0; k < 27; k++){
            conv_2_out_float[i][j][k] = (float)(0);
        }
    }
}
for(int i = 0; i < 256; i++){
    for(int j = 0; j < 27; j++){
        for(int k = 0; k < 27; k++){
            lrn_2_out_float[i][j][k] = (float)(0);
        }
    }
}
for(int i = 0; i < 256; i++){
    for(int j = 0; j < 27; j++){
        for(int k = 0; k < 27; k++){
            lrn_2_out[i][j][k] = (data_type)(0);
        }
    }
}
for(int i = 0; i < 256; i++){
    for(int j = 0; j < 13; j++){
        for(int k = 0; k < 13; k++){
            pool_2_out[i][j][k] = (data_type)(0);
        }
    }
}
for(int i = 0; i < 384; i++){
    for(int j = 0; j < 13; j++){
        for(int k = 0; k < 13; k++){
            conv_3_out[i][j][k] = (data_type)(0);
        }
    }
}
for(int i = 0; i < 384; i++){
    for(int j = 0; j < 13; j++){
        for(int k = 0; k < 13; k++){
            conv_4_out[i][j][k] = (data_type)(0);
        }
    }
}
for(int i = 0; i < 256; i++){
    for(int j = 0; j < 13; j++){
        for(int k = 0; k < 13; k++){
            conv_5_out[i][j][k] = (data_type)(0);
        }
    }
}
for(int i = 0; i < 256; i++){
    for(int j = 0; j < 6; j++){
        for(int k = 0; k < 6; k++){
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
for(int i = 0; i < 4096; i++){
    for(int j = 0; j < 1; j++){
        for(int k = 0; k < 1; k++){
            fc_7_out[i][j][k] = (data_type)(0);
        }
    }
}

	//Forward propagation by layer
	C1.conv_layer_a(activation_type, in_data_3D, conv_1_weight_a, conv_1_bias_a, conv_1_out);
	for(int i = 0; i < 96; i++){
    for(int j = 0; j < 55; j++){
        for(int k = 0; k < 55; k++){
            conv_1_out_float[i][j][k] = (float)conv_1_out[i][j][k];
        }
    }
}
	data_type output_min_lrn_1 = (data_type)0;
    data_type output_max_lrn_1 = (data_type)0;
    L1.lrn_layer_a(nn_alpha_lrn[0], nn_beta_lrn[0], conv_1_out_float, lrn_1_out_float);
	for(int i = 0; i < 96; i++){
    for(int j = 0; j < 55; j++){
        for(int k = 0; k < 55; k++){
            lrn_1_out[i][j][k] = (data_type)lrn_1_out_float[i][j][k];
            if(lrn_1_out[i][j][k] < output_min_lrn_1){
                output_min_lrn_1 = lrn_1_out[i][j][k];
            }
            if(lrn_1_out[i][j][k] > output_max_lrn_1){
                output_max_lrn_1 = lrn_1_out[i][j][k];
            }
        }
    }
}
	ofstream output_range;
    output_range.open("lrn_layer_output_range_a.txt", ios::app);
    output_range << "output range from lrn_1 layer .........................." << endl;
    output_range << output_min_lrn_1 << "~~~" << output_max_lrn_1 << endl;
    output_range << endl;
    output_range.close();

    P1.max_pooling_layer_a(activation_type, lrn_1_out, pool_1_out);
	C2.conv_layer_a(activation_type, pool_1_out, conv_2_weight_a, conv_2_bias_a, conv_2_out);
	for(int i = 0; i < 256; i++){
    for(int j = 0; j < 27; j++){
        for(int k = 0; k < 27; k++){
            conv_2_out_float[i][j][k] = (float)conv_2_out[i][j][k];
        }
    }
}
	data_type output_min_lrn_2 = (data_type)0;
    data_type output_max_lrn_2 = (data_type)0;
    L2.lrn_layer_a(nn_alpha_lrn[1], nn_beta_lrn[1], conv_2_out_float, lrn_2_out_float);
	for(int i = 0; i < 256; i++){
    for(int j = 0; j < 27; j++){
        for(int k = 0; k < 27; k++){
            lrn_2_out[i][j][k] = (data_type)lrn_2_out_float[i][j][k];
            if(lrn_2_out[i][j][k] < output_min_lrn_2){
                output_min_lrn_2 = lrn_2_out[i][j][k];
            }
            if(lrn_2_out[i][j][k] > output_max_lrn_2){
                output_max_lrn_2 = lrn_2_out[i][j][k];
            }
        }
    }
}
    output_range.open("lrn_layer_output_range_a.txt", ios::app);
    output_range << "output range from lrn_2 layer .........................." << endl;
    output_range << output_min_lrn_2 << "~~~" << output_max_lrn_2 << endl;
    output_range << endl;
    output_range.close();

    P2.max_pooling_layer_a(activation_type, lrn_2_out, pool_2_out);
	C3.conv_layer_a(activation_type, pool_2_out, conv_3_weight_a, conv_3_bias_a, conv_3_out);
	C4.conv_layer_a(activation_type, conv_3_out, conv_4_weight_a, conv_4_bias_a, conv_4_out);
	C5.conv_layer_a(activation_type, conv_4_out, conv_5_weight_a, conv_5_bias_a, conv_5_out);
	P5.max_pooling_layer_a(activation_type, conv_5_out, pool_5_out);
	F6.fc_layer_a(activation_type, pool_5_out, fc_6_weight_a, fc_6_bias_a, fc_6_out);
	/*D6.dropout_layer_a(dropout_ratio, fc_6_out, drop_6_out);*/
	F7.fc_layer_a(activation_type, fc_6_out, fc_7_weight_a, fc_7_bias_a, fc_7_out);
	/*D7.dropout_layer_a(dropout_ratio, fc_7_out, drop_7_out);*/
	F8.fc_layer_a_no_activation(fc_7_out, fc_8_weight_a, fc_8_bias_a, fc_8_out_a);

	/******************************************************************************************/


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
	cout << "Finished forward network process .........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif

}

#endif //_CONSTRUCT_NET_H_
