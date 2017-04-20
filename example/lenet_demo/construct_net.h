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
#include "../../fpga_cnn/conv_pool_layer.h"
#include "../../fpga_cnn/conv_layer.h"
#include "../../fpga_cnn/pool_layer.h"
#include "../../fpga_cnn/fc_layer.h"


void inference_net(

        char activation_type,

        // input pic data
        data_type in_data_3D[1][32][32],

        // layer weights and bias inputs ------- conv2pool2fc
        data_type        conv_1_weight_a[6][5][5],
        data_type        conv_1_bias_a[6],
        data_type        conv_2_weight_a[96][5][5],
        data_type        conv_2_bias_a[16],
        data_type        fc_1_weight_a[160][5][5],
        data_type        fc_1_bias_a[10],

        // output fc data
        data_type fc_1_out_a[10][1][1]
) {


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    cout << "starting forward network process..........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif


    /******************************************************************************************/
    //construct network --------------conv(1-6) + max Pooling + conv(6-16) + max pooling + fc
    conv_pool_layer<data_type, 32, 5, 0, 1, 2, 0, 2, 1, 6> C1P2;
    conv_pool_layer<data_type, 14, 5, 0, 1, 2, 0, 2, 6, 16> C3P4;
    fc_layer<data_type, data_type, data_type, 16, 5, 10> F5;

    //temp storage space
    data_type  pool_1_out[6][14][14];
    data_type  pool_2_out[16][5][5];

    //internal memory initiallization
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 14; j++){
            for(int k = 0; k < 14; k++){
                pool_1_out[i][j][k] = (data_type)(0);
            }
        }
    }
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 5; j++){
            for(int k = 0; k < 5; k++){
                pool_2_out[i][j][k] = (data_type)(0);
            }
        }
    }

    //Forward propagation by layer
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
