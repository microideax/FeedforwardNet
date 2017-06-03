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
#include "conv_acc.h"

using namespace std;

// caffenet inference function
void inference_net(
    char activation_type,

    // input pic data
    data_type in_data_3D[3*227*227],

    // layer weights and bias inputs
    data_type_w *conv_weight_port,
    data_type_w *conv_bias_port,
    data_type_w *fc_weight_port,
    data_type_w *fc_bias_port,

    // output fc data
    data_type_o fc_8_out_a[10*1*1]
) {

#if _HLS_MODE_

#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
#pragma HLS INTERFACE s_axilite port=activation_type bundle=CRTL_BUS

#pragma HLS INTERFACE m_axi depth=3072 port=in_data_3D

#pragma HLS INTERFACE m_axi depth=2103840  port=conv_weight_port
#pragma HLS INTERFACE m_axi depth=1376  port=conv_bias_port
#pragma HLS INTERFACE m_axi depth=17866752  port=fc_weight_port
#pragma HLS INTERFACE m_axi depth=8202  port=fc_bias_port

#pragma HLS INTERFACE m_axi depth=10  port=fc_8_out_a

#pragma HLS INTERFACE m_axi depth=24576  port=output_temp_1
#pragma HLS INTERFACE m_axi depth=24576  port=output_temp_2

#endif


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
	cout << "starting forward network process..........................." << endl;
	cout << "..........................................................." << endl;
#endif
#endif


    /******************************************************************************************/

    //construct network ----- lenet
    //-------------------------------conv layer 1 ----------------------------------//
    //conv_layer<data_type, data_type_w, data_type_o, 28, 5, 2, 1, 1, 6, 1> C1;
    conv_acc<data_type, data_type_w, data_type_o, 6, 1, 28, 28> convAcc1;//{0<Tm<=M;0<Tn<=N;0<Tr<=R;0<Tc<=C;}
    //-------------------------------pooling layer 1 -------------------------------//
    pool_layer<data_type, data_type_w, data_type_o, 28, 2, 0, 2, 6> P1;
    //-------------------------------conv layer 2 ----------------------------------//
    //conv_layer<data_type, data_type_w, data_type_o, 14, 5, 0, 1, 6, 16, 1> C2;
    conv_acc<data_type, data_type_w, data_type_o, 16, 6, 10, 10> convAcc2;//{0<Tm<=M;0<Tn<=N;0<Tr<=R;0<Tc<=C;}
    //-------------------------------pooling layer 2 -------------------------------//
    pool_layer<data_type, data_type_w, data_type_o, 10, 2, 0, 2, 16> P2;
    //-------------------------------conv layer 3 ----------------------------------//
    fc_layer<data_type,data_type_w,data_type_o, 16, 5, 10> F5;

    //temp storage space
    data_type_o in_data_buf[28*28];
    data_type_o fc_8_out_buf[10];
    data_type_o acc_fc_out[10];

    TRANS_DATA: for(int i = 0; i < 28*28; i++) {
        in_data_buf[i] = *(in_data_3D + i);
    }

    //internal memory initiallization:must do it!
    RESET_fc_8_out_buf: for(int i = 0; i < 10; i++){
        fc_8_out_buf[i] = data_type_o(0);
    }
    data_type_o  conv_acc_temp_0[96*32*32];
    data_type_o  conv_acc_temp_1[96*16*16];
    RESET_temp_0: for(int addr = 0; addr < 96*32*32; addr++){
        conv_acc_temp_0[addr] = data_type_o(0);
    }
    RESET_temp_1: for(int addr = 0; addr < 96*16*16; addr++){
        conv_acc_temp_1[addr] = 0;
    }

    char act;

    //Forward propagation by layer
//    C1.conv_layer_a(activation_type, in_data_buf, conv_weight_port, conv_bias_port, output_temp_1);
    convAcc1.conv_layer_acc(1, 5, 6, 28, 28, 1, 2, in_data_buf,conv_weight_port, conv_bias_port, conv_acc_temp_0);

//    P1.max_pooling_layer_a(activation_type, output_temp_1, output_temp_2);
    P1.max_pooling_layer_a(activation_type, conv_acc_temp_0, conv_acc_temp_1);
    RESET_1: for(int addr = 0; addr < 96*32*32; addr++){
        //output_temp_1[addr] = data_type_o(0);
        conv_acc_temp_0[addr] = data_type_o(0);
    }
//    C2.conv_layer_a(activation_type, output_temp_2, conv_weight_port+150, conv_bias_port+6, output_temp_1);
    convAcc2.conv_layer_acc(6, 5, 16, 10, 10, 1, 0, conv_acc_temp_1, conv_weight_port+150, conv_bias_port+6, conv_acc_temp_0);
    RESET_2: for(int addr = 0; addr < 96*16*16; addr++){
        //output_temp_2[addr] = data_type_o(0);
        conv_acc_temp_1[addr] = 0;
    }
//    P2.max_pooling_layer_a(activation_type, output_temp_1, output_temp_2);
    P2.max_pooling_layer_a(activation_type, conv_acc_temp_0, conv_acc_temp_1);
    RESET_3: for(int addr = 0; addr < 96*32*32; addr++){
        //output_temp_1[addr] = data_type_o(0);
        conv_acc_temp_0[addr] = data_type_o(0);
    }
    F5.fc_layer_a(activation_type, conv_acc_temp_1, fc_weight_port, fc_bias_port, fc_8_out_buf);
//    F5.fc_layer_a(activation_type, output_temp_2, fc_weight_port, fc_bias_port, fc_8_out_buf);

    RESET_10: for(int i = 0; i < 10; i++){
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
