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
#include "../../fpga_cnn/weight_bias_one_dim.h"
#include "../../fpga_cnn/activation_functions.h"
#include "../../fpga_cnn/conv_pool_layer_one_dim.h"
#include "../../fpga_cnn/conv_layer_one_dim.h"
#include "../../fpga_cnn/pool_layer_one_dim.h"
#include "../../fpga_cnn/fc_layer_one_dim.h"
#include "../../fpga_cnn/array_reset.h"

void inference_net(

        char activation_type,

        // input pic data
        data_type in_data_3D[1024],

        // layer weights and bias inputs ------- conv2pool2fc
        data_type_w        conv_1_weight_a[150],
        data_type_w        conv_1_bias_a[6],
        data_type_w        conv_2_weight_a[2400],
        data_type_w        conv_2_bias_a[16],
        data_type_w        fc_1_weight_a[4000],
        data_type_w        fc_1_bias_a[10],

        // output fc data
        data_type_o fc_1_out_a[10]
) {


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    cout << "starting forward network process..........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif

    /******************************************************************************************/
    //construct network --------------conv(1-6) + max Pooling + conv(6-16) + max pooling + fc
    //conv_layer<data_type,data_type_w,data_type_o, 32, 5, 0, 1, 1, 6, 1> C1;
    //pool_layer<data_type,data_type_w,data_type_o, 28, 2, 0, 2, 6> P2;
    //conv_layer<data_type,data_type_w,data_type_o, 14, 5, 0, 1, 6, 16, 1> C3;
    //pool_layer<data_type,data_type_w,data_type_o, 10, 2, 0, 2, 16> P4;
    conv_pool_layer<data_type,data_type_w,data_type_o, 32, 5, 0, 1, 2, 0, 2, 1, 6,1> C1P2;
    conv_pool_layer<data_type,data_type_w,data_type_o, 14, 5, 0, 1, 2, 0, 2, 6, 16,1> C3P4;
    fc_layer<data_type,data_type_w,data_type_o, 16, 5, 10> F5;

    //temp storage space
    //data_type_o  conv_1_out[4704];
    data_type_o  pool_1_out[1176];
    //data_type_o  conv_2_out[1600];
    data_type_o  pool_2_out[400];

    //internal memory initiallization
    //array_reset(conv_1_out);
    array_reset(pool_1_out);
    //array_reset(conv_2_out);
    array_reset(pool_2_out);

    //Forward propagation by layer
    //C1.conv_layer_a(activation_type, in_data_3D, conv_1_weight_a, conv_1_bias_a, conv_1_out);
    //P2.max_pooling_layer_a(activation_type, conv_1_out, pool_1_out);
    //C3.conv_layer_a(activation_type, pool_1_out, conv_2_weight_a, conv_2_bias_a, conv_2_out);
    //P4.max_pooling_layer_a(activation_type, conv_2_out, pool_2_out);
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
