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
#include "data_type.h"

#include "weight_bias.h"

#include "activation_functions.h"
#include "conv_layer.h"
#include "pool_layer.h"
#include "fc_layer.h"

const int nn_in_data_size_conv[3] = { 32,14,5 };
int nn_in_number_conv[3] = { 1,6,16 };
const int nn_channel_size_conv = 5;
int nn_channel_number_conv[3] = { 6,16,120 };
bool has_connection_table[3] = { false, true, false };

int nn_in_data_size_pooling[2] = { 28,10 };
int nn_in_number_pooling[2] = { 6,16 };
int nn_channel_number_pooling[2] = { 6,16 };
int nn_channel_size_pooling = 2;

int nn_in_data_size_fc[1] = { 1 };
int nn_in_number_fc[1] = { 120 };
int nn_channel_size_fc = 1;
int nn_channel_number_fc[1] = { 10 };

/*
// construct nets
nn << convolutional_layer<tan_h>(32, 32, 5, 1, 6)  // C1, 1@32x32-in, 6@28x28-out
<< average_pooling_layer<tan_h>(28, 28, 6, 2)   // S2, 6@28x28-in, 6@14x14-out
<< convolutional_layer<tan_h>(14, 14, 5, 6, 16,
connection_table(tbl, 6, 16))              // C3, 6@14x14-in, 16@10x10-out
<< average_pooling_layer<tan_h>(10, 10, 16, 2)  // S4, 16@10x10-in, 16@5x5-out
<< convolutional_layer<tan_h>(5, 5, 5, 16, 120) // C5, 16@5x5-in, 120@1x1-out
<< fully_connected_layer<tan_h>(120, 10);       // F6, 120-in, 10-out
}
*/

void inference_net(

        char activation_type,
        const int nn_in_data_size_conv[3],
        const int nn_channel_size_conv,
        bool has_connection_table[3],
        int nn_in_number_conv[3],
        int nn_channel_number_conv[3],

        // input pic data
        tensor_t_3d& in_data3D,

        // output fc data
        tensor_t_3d& fc_1_out_data,

        // layer weights and bias inputs
        tensor_t_3d& conv_1_weight2D,
        vec_t&       conv_1_bias2D,
        vec_t&       pooling_1_weight,
        vec_t&       pooling_1_bias2D,
        tensor_t_3d& conv_2_weight2D,
        vec_t&       conv_2_bias2D,
        vec_t&       pooling_2_weight,
        vec_t&       pooling_2_bias2D,
        tensor_t_3d& conv_3_weight2D,
        vec_t&       conv_3_bias2D,
        tensor_t_3d& fc_1_weight2D,
        vec_t&       fc_1_bias2D,

        //temporary data storage with AXI bus interface
        tensor_t_3d& conv_1_out_data,
        tensor_t_3d& pooling_1_out_data,
        tensor_t_3d& conv_2_out_data,
        tensor_t_3d& pooling_2_out_data,
        tensor_t_3d& conv_3_out_data
) {

#if _HLS_MODE_
//#pragma HLS RESOURCE core=AXI4LiteS variable=return

#pragma HLS INTERFACE ap_bus port=in_data3D
#pragma HLS RESOURCE core=AXI4M variable=in_data3D

#pragma HLS INTERFACE ap_bus port=conv_1_weight2D
#pragma HLS RESOURCE core=AXI4M variable=conv_1_weight2D

#pragma HLS INTERFACE ap_bus port=conv_2_weight2D
#pragma HLS RESOURCE core=AXI4M variable=conv_2_weight2D

#pragma HLS INTERFACE ap_bus port=conv_3_weight2D
#pragma HLS RESOURCE core=AXI4M variable=conv_3_weight2D

#pragma HLS INTERFACE ap_bus port=fc_1_weight2D
#pragma HLS RESOURCE core=AXI4M variable=fc_1_weight2D

#pragma HLS INTERFACE ap_bus port=conv_1_out_data
#pragma HLS RESOURCE core=AXI4M variable=conv_1_out_data

#pragma HLS INTERFACE ap_bus port=pooling_1_out_data
#pragma HLS RESOURCE core=AXI4M variable=pooling_1_out_data

#pragma HLS INTERFACE ap_bus port=conv_2_out_data
#pragma HLS RESOURCE core=AXI4M variable=conv_2_out_data

#pragma HLS INTERFACE ap_bus port=pooling_2_out_data
#pragma HLS RESOURCE core=AXI4M variable=pooling_2_out_data

#pragma HLS INTERFACE ap_bus port=conv_3_out_data
#pragma HLS RESOURCE core=AXI4M variable=conv_3_out_data
#endif

    //construct network --------------LeNet-5
    conv_layer<32, 5, 1, 6> conv_layer_1;
    pool_layer< 28, 2, 6 > pooling_layer_1;
    conv_layer<14, 5, 6, 16> conv_layer_2;
    pool_layer<10, 2, 16> pooling_layer_2;
    conv_layer<5, 5, 16, 120> conv_layer_3;
    fc_layer<1, 120, 10> fc_layer_1;


    //Forward propagation process
    //convolution layer 1
    conv_layer_1.convolution_layer_with_table(
            activation_type,
            in_data3D,
            has_connection_table[0],
            conv_1_weight2D,
            conv_1_bias2D,
            conv_1_out_data);

#if _C_DEBUG_MODE_
    cout << "Finished conv_layer 1" << endl;
    cout << "..........................................................." << endl;
#endif

    //pooling layer 1
    pooling_layer_1.pooling_layer(
            activation_type,
            conv_1_out_data,
            pooling_1_weight,
            pooling_1_bias2D,
            pooling_1_out_data);

#if _C_DEBUG_MODE_
    cout << "Finished pool_layer 1" << endl;
    cout << "..........................................................." << endl;
#endif

    //convolution layer 2
    conv_layer_2.convolution_layer_with_table(
            activation_type,
            pooling_1_out_data,
            has_connection_table[1],
            conv_2_weight2D,
            conv_2_bias2D,
            conv_2_out_data);

#if _C_DEBUG_MODE_
    cout << "Finished conv_layer 2" << endl;
    cout << "..........................................................." << endl;
#endif

    //pooling layer 2

    pooling_layer_2.pooling_layer(
            activation_type,
            conv_2_out_data,
            pooling_2_weight,
            pooling_2_bias2D,
            pooling_2_out_data);

#if _C_DEBUG_MODE_
    cout << "Finished pool_layer 2" << endl;
    cout << "..........................................................." << endl;
#endif


    //convolution layer 3
    conv_layer_3.convolution_layer_with_table(
            activation_type,
            pooling_2_out_data,
            has_connection_table[2],
            conv_3_weight2D,
            conv_3_bias2D,
            conv_3_out_data    );

#if _C_DEBUG_MODE_
    cout << "Finished conv_layer 3" << endl;
    cout << "..........................................................." << endl;
#endif

    //fully connected layer 1
    fc_layer_1.fully_connected_layer(
            activation_type,
            conv_3_out_data,
            fc_1_weight2D,
            fc_1_bias2D,
            fc_1_out_data);

#if _C_DEBUG_MODE_
    cout << "Finished fc_layer 1" << endl;
    cout << "..........................................................." << endl;
#endif

}

#endif //FFNET_CONSTRUCT_NET_H
