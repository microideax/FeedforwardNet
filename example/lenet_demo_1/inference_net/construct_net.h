//
// Created by yaochen on 24/12/16.
//

#ifndef _CONSTRUCT_NET_H_
#define _CONSTRUCT_NET_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

#include <ap_fixed.h>

#include "config.h"
#include "../../fpga_cnn/data_type.h"
#include "../../fpga_cnn/weight_bias.h"
#include "../../fpga_cnn/activation_functions.h"
//#include "../../fpga_cnn/conv_pool_layer.h"
#include "../../fpga_cnn/conv_layer.h"
#include "../../fpga_cnn/pool_layer.h"
#include "../../fpga_cnn/fc_layer.h"


void inference_net(

        char activation_type,

        // input pic data
        ap_fixed<64,32> in_data_3D[1][32][32],

        // layer weights and bias inputs ------- conv2pool2fc
        ap_fixed<64,32>        conv_1_weight_a[6][5][5],
        ap_fixed<64,32>        conv_1_bias_a[6],
        ap_fixed<64,32>        conv_2_weight_a[96][5][5],
        ap_fixed<64,32>        conv_2_bias_a[16],
        ap_fixed<64,32>        fc_1_weight_a[160][5][5],
        ap_fixed<64,32>        fc_1_bias_a[10],

        // output fc data
        ap_fixed<64,32> fc_1_out_a[10][1][1]
) {


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    cout << "starting forward network process..........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif


    /******************************************************************************************/
    //construct network --------------conv(1-6) + max Pooling + conv(6-16) + max pooling + fc
    conv_layer<ap_fixed<64,32>, 32, 5, 0, 1, 1, 6, 1> C1;
    pool_layer<ap_fixed<64,32>, 28, 2, 0, 2, 6> P2;
    conv_layer<ap_fixed<64,32>, 14, 5, 0, 1, 6, 16, 1> C3;
    pool_layer<ap_fixed<64,32>, 10, 2, 0, 2, 16> P4;
    fc_layer<ap_fixed<64,32>, 16, 5, 10> F5;

    //temp storage space
    ap_fixed<64,32>  conv_1_out[6][28][28];
    ap_fixed<64,32>  pool_1_out[6][14][14];
    ap_fixed<64,32>  conv_2_out[16][10][10];
    ap_fixed<64,32>  pool_2_out[16][5][5];


//internal memory initiallization
for(int i = 0; i < 6; i++){
    for(int j = 0; j < 28; j++){
        for(int k = 0; k < 28; k++){
            conv_1_out[i][j][k] = ap_fixed<64,32>(0);
        }
    }
}
for(int i = 0; i < 6; i++){
    for(int j = 0; j < 14; j++){
        for(int k = 0; k < 14; k++){
            pool_1_out[i][j][k] = ap_fixed<64,32>(0);
        }
    }
}
for(int i = 0; i < 16; i++){
    for(int j = 0; j < 10; j++){
        for(int k = 0; k < 10; k++){
            conv_2_out[i][j][k] = ap_fixed<64,32>(0);
        }
    }
}
for(int i = 0; i < 16; i++){
    for(int j = 0; j < 5; j++){
        for(int k = 0; k < 5; k++){
            pool_2_out[i][j][k] = ap_fixed<64,32>(0);
        }
    }
}

    //Forward propagation by layer
    C1.conv_layer_a(activation_type, in_data_3D, conv_1_weight_a, conv_1_bias_a, conv_1_out);
    P2.max_pooling_layer_a(activation_type, conv_1_out, pool_1_out);
    C3.conv_layer_a(activation_type, pool_1_out, conv_2_weight_a, conv_2_bias_a, conv_2_out);
    P4.max_pooling_layer_a(activation_type, conv_2_out, pool_2_out);
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
