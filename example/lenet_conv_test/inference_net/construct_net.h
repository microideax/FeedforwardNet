//
// Created by yaochen on 24/12/16.
//

#ifndef _CONSTRUCT_NET_H_
#define _CONSTRUCT_NET_H_


#include <iostream>
#include <ap_fixed.h>

#include "config.h"
#include "conv_layer_one_dim.h"
#include "pool_layer_one_dim.h"
#include "fc_layer_one_dim.h"
#include "conv_acc_break.h"
#include "max_pool_acc.h"

using namespace std;

// caffenet inference function
void inference_net(
    char activation_type,

    // input pic data
    data_type in_data_3D[28*28],

    // layer weights and bias inputs
    data_type_w *conv_weight_port,
    data_type_w *conv_bias_port,
    data_type_w *fc_weight_port,
    data_type_w *fc_bias_port,

    // output fc data
    data_type_o fc_8_out_a[10],

    //temp output ports
    data_type_o output_temp_0[96*32*32],
    data_type_o output_temp_1[96*32*32]
) {

#if _HLS_MODE_

#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
#pragma HLS INTERFACE s_axilite port=activation_type bundle=CRTL_BUS

#pragma HLS INTERFACE m_axi depth=784 port=in_data_3D

#pragma HLS INTERFACE m_axi depth=2550  port=conv_weight_port
#pragma HLS INTERFACE m_axi depth=156  port=conv_bias_port
#pragma HLS INTERFACE m_axi depth=4000  port=fc_weight_port
#pragma HLS INTERFACE m_axi depth=10  port=fc_bias_port

#pragma HLS INTERFACE m_axi depth=10  port=fc_8_out_a

#pragma HLS INTERFACE m_axi depth=24576  port=output_temp_0
#pragma HLS INTERFACE m_axi depth=24576  port=output_temp_1

#endif


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
	cout << "starting forward network process..........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif


    /******************************************************************************************/
    //construct network ----- lenet
    //-------------------------------conv acc 1 ----------------------------------//
    conv_acc<data_type, data_type_w, data_type_o, 16, 4, 16, 16> convAcc1;//{0<Tm<=M;0<Tn<=N;0<Tr<=R;0<Tc<=C;}
    //conv_acc<data_type, data_type_w, data_type_o, 5, 4, 1, 1> fcAcc1;
    fc_layer<float, float, float, 16, 5, 10> F5;
    //-------------------------------pooling acc 1 -------------------------------//
    max_pool_acc<data_type, data_type_w, data_type_o, 2, 4, 4> poolAcc1;

#pragma HLS allocation instances=conv_layer_acc limit=1 function

    //temp storage space
    data_type_o in_data_buf[28*28];
    data_type_o fc_8_out_buf[10];

    //internal memory initiallization:must do it!
    RESET_fc_8_out_buf: for(int i = 0; i < 10; i++){
        fc_8_out_buf[i] = data_type_o(0);
    }

    //Forward propagation by layer
    //--------------------------conv layer 1---------------------------//
    convAcc1.conv_layer_acc(1, 5, 6, 28, 28, 1, 2, output_temp_1, conv_weight_port, conv_bias_port, output_temp_0);
    //--------------------------pool layer 1---------------------------//
    RESET_0: for(int addr = 0; addr < 96*32*32; addr++){
        output_temp_1[addr] = 0;
    }
    poolAcc1.max_pool_layer_acc(6, 2, 14, 14, 2, 0, output_temp_0, output_temp_1);
    RESET_1: for(int addr = 0; addr < 96*32*32; addr++){
        output_temp_0[addr] = data_type_o(0);
    }
    //--------------------------conv layer 2---------------------------//
    convAcc1.conv_layer_acc(6, 5, 16, 10, 10, 1, 0, output_temp_1, conv_weight_port+150, conv_bias_port+6, output_temp_0);
    RESET_2: for(int addr = 0; addr < 96*32*32; addr++){
        output_temp_1[addr] = 0;
    }
    //--------------------------pool layer 2---------------------------//
    poolAcc1.max_pool_layer_acc(16, 2, 5, 5, 2, 0, output_temp_0, output_temp_1);
    RESET_3: for(int addr = 0; addr < 96*32*32; addr++){
        output_temp_0[addr] = data_type_o(0);
    }
    //--------------------------fc layer 1-----------------------------//
    convAcc1.conv_layer_acc(16, 5, 10, 1, 1, 5, 0, output_temp_1, fc_weight_port, fc_bias_port, output_temp_0);
//    F5.fc_layer_a('r', output_temp_1, fc_weight_port, fc_bias_port, fc_8_out_buf);

    RESET_10: for(int i = 0; i < 10; i++){
	    fc_8_out_a[i] = output_temp_0[i];
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
