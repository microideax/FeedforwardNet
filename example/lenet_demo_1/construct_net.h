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
        float fc_1_out_a[10][1][1]
) {


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    cout << "starting forward network process..........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif


    /******************************************************************************************/
    //construct network --------------conv(1-6) + max Pooling + conv(6-16) + max pooling + fc
    conv_layer<float, 32, 5, 0, 1, 1, 6, 1> C1;
    pool_layer<float, 28, 2, 0, 2, 6> P2;
    conv_layer<float, 14, 5, 0, 1, 6, 16, 1> C3;
    pool_layer<float, 10, 2, 0, 2, 16> P4;
    fc_layer<float, 16, 5, 10> F5;

    //temp storage space
    float  conv_1_out[6][28][28] = { 0 };
    float  pool_1_out[6][14][14] = { 0 };
    float  conv_2_out[16][10][10] = { 0 };
    float  pool_2_out[16][5][5] = { 0 };

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
