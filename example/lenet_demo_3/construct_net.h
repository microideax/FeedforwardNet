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
#include "../../fpga_cnn/conv_layer_buf.h"
#include "../../fpga_cnn/pool_layer_buf.h"
#include "../../fpga_cnn/fc_layer_buf.h"


void inference_net(

        char activation_type,

        // input pic data
        ap_fixed<24,16> in_data_3D[1][32][32],

        // layer weights and bias inputs ------- conv2pool2fc
        ap_fixed<10,2>        conv_1_weight_a[6][5][5],
        ap_fixed<10,2>        conv_1_bias_a[6],
        ap_fixed<10,2>        conv_2_weight_a[96][5][5],
        ap_fixed<10,2>        conv_2_bias_a[16],
        ap_fixed<10,2>        fc_1_weight_a[160][5][5],
        ap_fixed<10,2>        fc_1_bias_a[10],

        // output fc data
        ap_fixed<24,16> fc_1_out_a[10][1][1]
) {


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    cout << "starting forward network process..........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif



    /******************************************************************************************/
    //construct network --------------conv(1-6) + max Pooling + conv(6-16) + max pooling + fc
    conv_layer_buf<ap_fixed<24,16>, ap_fixed<10,2>, ap_fixed<24,16>, 32, 5, 0, 1, 1, 6, 1> C1;
    pool_layer_buf<ap_fixed<24,16>, ap_fixed<10,2>, ap_fixed<24,16>, 28, 2, 0, 2, 6> P2;
    conv_layer_buf<ap_fixed<24,16>, ap_fixed<10,2>, ap_fixed<24,16>, 14, 5, 0, 1, 6, 16, 1> C3;
    pool_layer_buf<ap_fixed<24,16>, ap_fixed<10,2>, ap_fixed<24,16>, 10, 2, 0, 2, 16> P4;
    fc_layer_buf<ap_fixed<24,16>, ap_fixed<10,2>, ap_fixed<24,16>, 16, 5, 10> F5;

    //temp storage space
    ap_fixed<24,16> buf_1[BUF_1_X_DIM][BUF_1_Y_DIM][BUF_1_Z_DIM];
    ap_fixed<24,16> buf_2[BUF_1_X_DIM][BUF_1_Y_DIM][BUF_1_Z_DIM];
    for(int i = 0; i < BUF_1_X_DIM; i++){
        for(int j = 0; j < BUF_1_Y_DIM; j++){
            for(int k = 0; k < BUF_1_Z_DIM; k++){
                buf_1[i][j][k] = ap_fixed<24,16>(0);
                buf_2[i][j][k] = ap_fixed<24,16>(0);
            }
        }
    }

    //Forward propagation by layer
    C1.conv_layer_input_buf(activation_type, in_data_3D, conv_1_weight_a, conv_1_bias_a, buf_1);
    P2.max_pooling_layer_buf(activation_type, buf_1, buf_2);
    C3.conv_layer_general_buf(activation_type, buf_2, conv_2_weight_a, conv_2_bias_a, buf_1);
    P4.max_pooling_layer_buf(activation_type, buf_1, buf_2);
    F5.fc_layer_output_buf(activation_type, buf_2, fc_1_weight_a, fc_1_bias_a, fc_1_out_a);
    /******************************************************************************************/


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    cout << "Finished forward network process .........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif

}

#endif //_CONSTRUCT_NET_H_
