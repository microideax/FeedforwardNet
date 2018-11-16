
#ifndef _CONSTRUCT_NET_H_
#define _CONSTRUCT_NET_H_

#include <iostream>
#include <fstream>
#include <algorithm>
#include "config.h"
#include "data_type.h"
#include "acc_instance.h"
#include "fc_layer_one_dim.h"

using namespace std;

void inf_net_0(
//   char   activation_type,
        data_type_itf conv_weight_port_0[4536],
        data_type_itf conv_weight_port_1[4536],
        data_type_itf conv_weight_port_2[4536],
        data_type_itf conv_weight_port_3[4536],
        data_type_itf conv_weight_port_4[4536],
        data_type_itf conv_weight_port_5[4536],
        data_type_itf conv_weight_port_6[4536],
        data_type_itf conv_weight_port_7[4536],

        data_type_itf conv_bias_port[96],

        data_type_itf data_in_0[227 * 227],
        data_type_itf data_in_1[227 * 227],
        data_type_itf data_in_2[227 * 227],
        data_type_itf data_in_3[227 * 227],
        data_type_itf data_in_4[227 * 227],
        data_type_itf data_in_5[227 * 227],
        data_type_itf data_in_6[227 * 227],
        data_type_itf data_in_7[227 * 227],

//   data_type_o   fc_8_out_a[1000*1*1],
        data_type_itf data_out_0[13*13*32],
        data_type_itf data_out_1[13*13*32],
        data_type_itf data_out_2[13*13*32],
        data_type_itf data_out_3[13*13*32],
        data_type_itf data_out_4[13*13*32],
        data_type_itf data_out_5[13*13*32],
        data_type_itf data_out_6[13*13*32],
        data_type_itf data_out_7[13*13*32]) {

#if _HLS_MODE_

#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS

#pragma HLS INTERFACE m_axi port=conv_weight_port_0 depth=4536
#pragma HLS INTERFACE m_axi port=conv_weight_port_1 depth=4536
#pragma HLS INTERFACE m_axi port=conv_weight_port_2 depth=4536
#pragma HLS INTERFACE m_axi port=conv_weight_port_3 depth=4536
#pragma HLS INTERFACE m_axi port=conv_weight_port_4 depth=4536
#pragma HLS INTERFACE m_axi port=conv_weight_port_5 depth=4536
#pragma HLS INTERFACE m_axi port=conv_weight_port_6 depth=4536
#pragma HLS INTERFACE m_axi port=conv_weight_port_7 depth=4536

#pragma HLS INTERFACE m_axi port=conv_bias_port depth=96

#pragma HLS INTERFACE m_axi port=data_in_0 depth=227*227
#pragma HLS INTERFACE m_axi port=data_in_1 depth=227*227
#pragma HLS INTERFACE m_axi port=data_in_2 depth=227*227
#pragma HLS INTERFACE m_axi port=data_in_3 depth=227*227
#pragma HLS INTERFACE m_axi port=data_in_4 depth=227*227
#pragma HLS INTERFACE m_axi port=data_in_5 depth=227*227
#pragma HLS INTERFACE m_axi port=data_in_6 depth=227*227
#pragma HLS INTERFACE m_axi port=data_in_7 depth=227*227

#pragma HLS INTERFACE bram port=data_out_0
#pragma HLS RESOURCE variable=data_out_0 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_1
#pragma HLS RESOURCE variable=data_out_1 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_2
#pragma HLS RESOURCE variable=data_out_2 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_3
#pragma HLS RESOURCE variable=data_out_3 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_4
#pragma HLS RESOURCE variable=data_out_4 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_5
#pragma HLS RESOURCE variable=data_out_5 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_6
#pragma HLS RESOURCE variable=data_out_6 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_7
#pragma HLS RESOURCE variable=data_out_7 core=RAM_1P_BRAM
    // tk=2, 64240; no tk, 24976
    /*
    #pragma HLS INTERFACE m_axi port=data_out_0 depth=256
    #pragma HLS INTERFACE m_axi port=data_out_1 depth=256
    #pragma HLS INTERFACE m_axi port=data_out_2 depth=256
    #pragma HLS INTERFACE m_axi port=data_out_3 depth=256
    #pragma HLS INTERFACE m_axi port=data_out_4 depth=256
    #pragma HLS INTERFACE m_axi port=data_out_5 depth=256
    #pragma HLS INTERFACE m_axi port=data_out_6 depth=256
    #pragma HLS INTERFACE m_axi port=data_out_7 depth=256
    latency = 64546
     */
#endif


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    cout << "Starting inf_net_0 process..........................." << endl;
    cout << "..........................................................." << endl;
#endif
#endif


    int shift_weight_conv1_1 = 0;
    int shift_bias_conv1_1 = 0;


    data_type_o tmp_0_0_0[55 * 55 * 12];
    data_type_o tmp_0_0_1[55 * 55 * 12];
    data_type_o tmp_0_0_2[55 * 55 * 12];
    data_type_o tmp_0_0_3[55 * 55 * 12];
    data_type_o tmp_0_0_4[55 * 55 * 12];
    data_type_o tmp_0_0_5[55 * 55 * 12];
    data_type_o tmp_0_0_6[55 * 55 * 12];
    data_type_o tmp_0_0_7[55 * 55 * 12];

/*
   conv_layer_new(3, 11, 96, 55, 55, 4, 0, 1, output_temp_2, conv_weight_port, conv_bias_port, output_temp_1, shift_weight_conv1_1, shift_bias_conv1_1, 0, 0);
   clean_1:   for (int addr = 0; addr < 290400; addr++) {output_temp_2[addr] = data_type_o(0);}
*/
    conv_layer_acc_0(3, 11, 96, 227, 227, 55, 55, 4, 0, 1,
                     conv_weight_port_0, conv_weight_port_1, conv_weight_port_2, conv_weight_port_3,
                     conv_weight_port_4, conv_weight_port_5, conv_weight_port_6, conv_weight_port_7,
                     conv_bias_port,
                     shift_weight_conv1_1, shift_bias_conv1_1, 0, 0,
                     data_in_0, data_in_1, data_in_2, data_in_3, data_in_4, data_in_5, data_in_6, data_in_7,
                     tmp_0_0_0, tmp_0_0_1, tmp_0_0_2, tmp_0_0_3, tmp_0_0_4, tmp_0_0_5, tmp_0_0_6, tmp_0_0_7);


    data_type_o tmp_0_1_0[27 * 27 * 12];
    data_type_o tmp_0_1_1[27 * 27 * 12];
    data_type_o tmp_0_1_2[27 * 27 * 12];
    data_type_o tmp_0_1_3[27 * 27 * 12];
    data_type_o tmp_0_1_4[27 * 27 * 12];
    data_type_o tmp_0_1_5[27 * 27 * 12];
    data_type_o tmp_0_1_6[27 * 27 * 12];
    data_type_o tmp_0_1_7[27 * 27 * 12];

    /*
    max_pool_layer_new_noact(55, 55, 96, 3, 27, 27, 2, 0, output_temp_1, output_temp_2);
    clean_2:   for (int addr = 0; addr < 290400; addr++) {   output_temp_1[addr] = data_type_o(0);   }*/

    max_pool_layer_acc_1(55, 55, 96, 3, 27, 27, 2, 0, 1,
                         tmp_0_0_0, tmp_0_0_1, tmp_0_0_2, tmp_0_0_3, tmp_0_0_4, tmp_0_0_5, tmp_0_0_6, tmp_0_0_7,
                         tmp_0_1_0, tmp_0_1_1, tmp_0_1_2, tmp_0_1_3, tmp_0_1_4, tmp_0_1_5, tmp_0_1_6, tmp_0_1_7);


    int shift_weight_conv2_1 = 34848 / 8;
    int shift_bias_conv2_1 = 96;
    int shift_weight_conv2_2 = 188448 / 8;
    int shift_bias_conv2_2 = 224;
    int in_shift_2 = 48 * 27 * 27 / 8;
    int out_shift_2 = 128 * 27 * 27 / 8;

    data_type_o tmp_0_2_0[27 * 27 * 32];
    data_type_o tmp_0_2_1[27 * 27 * 32];
    data_type_o tmp_0_2_2[27 * 27 * 32];
    data_type_o tmp_0_2_3[27 * 27 * 32];
    data_type_o tmp_0_2_4[27 * 27 * 32];
    data_type_o tmp_0_2_5[27 * 27 * 32];
    data_type_o tmp_0_2_6[27 * 27 * 32];
    data_type_o tmp_0_2_7[27 * 27 * 32];

/*
    conv_layer_new(48, 5, 128, 27, 27, 1, 2, 1, output_temp_2, conv_weight_port, conv_bias_port, output_temp_1, shift_weight_conv2_1, shift_bias_conv2_1, 0, 0);
    conv_layer_new(48, 5, 128, 27, 27, 1, 2, 1, output_temp_2, conv_weight_port, conv_bias_port, output_temp_1, shift_weight_conv2_2, shift_bias_conv2_2, in_shift_2, out_shift_2);
    clean_3:   for (int addr = 0; addr < 290400; addr++) {   output_temp_2[addr] = data_type_o(0);   }
*/

    conv_layer_acc_1(48, 5, 128, 27, 27, 27, 27, 1, 2, 1,
                     conv_weight_port_0, conv_weight_port_1, conv_weight_port_2, conv_weight_port_3,
                     conv_weight_port_4, conv_weight_port_5, conv_weight_port_6, conv_weight_port_7,
                     conv_bias_port,
                     shift_weight_conv2_1, shift_bias_conv2_1, 0, 0,
                     tmp_0_1_0, tmp_0_1_1, tmp_0_1_2, tmp_0_1_3, tmp_0_1_4, tmp_0_1_5, tmp_0_1_6, tmp_0_1_7,
                     tmp_0_2_0, tmp_0_2_1, tmp_0_2_2, tmp_0_2_3, tmp_0_2_4, tmp_0_2_5, tmp_0_2_6, tmp_0_2_7);
    conv_layer_acc_1(48, 5, 128, 27, 27, 27, 27, 1, 2, 1,
                     conv_weight_port_0, conv_weight_port_1, conv_weight_port_2, conv_weight_port_3,
                     conv_weight_port_4, conv_weight_port_5, conv_weight_port_6, conv_weight_port_7,
                     conv_bias_port,
                     shift_weight_conv2_2, shift_bias_conv2_2, in_shift_2, out_shift_2,
                     tmp_0_1_0, tmp_0_1_1, tmp_0_1_2, tmp_0_1_3, tmp_0_1_4, tmp_0_1_5, tmp_0_1_6, tmp_0_1_7,
                     tmp_0_2_0, tmp_0_2_1, tmp_0_2_2, tmp_0_2_3, tmp_0_2_4, tmp_0_2_5, tmp_0_2_6, tmp_0_2_7);

/*
   max_pool_layer_new_noact(27, 27, 256, 3, 13, 13, 2, 0, output_temp_1, output_temp_2);
   clean_4:   for (int addr = 0; addr < 290400; addr++) {   output_temp_1[addr] = data_type_o(0);   }
*/

    max_pool_layer_acc_2(27, 27, 256, 3, 13, 13, 2, 0, 1,
                         tmp_0_2_0, tmp_0_2_1, tmp_0_2_2, tmp_0_2_3, tmp_0_2_4, tmp_0_2_5, tmp_0_2_6, tmp_0_2_7,
                         data_out_0, data_out_1, data_out_2, data_out_3, data_out_4, data_out_5, data_out_6,
                         data_out_7);

};


void inf_net_1(
        data_type_itf conv_weight_port_0[384*(48+32)*9],
        data_type_itf conv_weight_port_1[384*(48+32)*9],
        data_type_itf conv_weight_port_2[384*(48+32)*9],
        data_type_itf conv_weight_port_3[384*(48+32)*9],
        data_type_itf conv_weight_port_4[384*(48+32)*9],
        data_type_itf conv_weight_port_5[384*(48+32)*9],
        data_type_itf conv_weight_port_6[384*(48+32)*9],
        data_type_itf conv_weight_port_7[384*(48+32)*9],

        data_type_itf conv_bias_port[256+384],

        data_type_itf data_in_0[13*13*32],
        data_type_itf data_in_1[13*13*32],
        data_type_itf data_in_2[13*13*32],
        data_type_itf data_in_3[13*13*32],
        data_type_itf data_in_4[13*13*32],
        data_type_itf data_in_5[13*13*32],
        data_type_itf data_in_6[13*13*32],
        data_type_itf data_in_7[13*13*32],

        data_type_itf data_out_0[13*13*48],
        data_type_itf data_out_1[13*13*48],
        data_type_itf data_out_2[13*13*48],
        data_type_itf data_out_3[13*13*48],
        data_type_itf data_out_4[13*13*48],
        data_type_itf data_out_5[13*13*48],
        data_type_itf data_out_6[13*13*48],
        data_type_itf data_out_7[13*13*48]) {

#if _HLS_MODE_

#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS

#pragma HLS INTERFACE m_axi port=conv_weight_port_0 depth=165888
#pragma HLS INTERFACE m_axi port=conv_weight_port_1 depth=165888
#pragma HLS INTERFACE m_axi port=conv_weight_port_2 depth=165888
#pragma HLS INTERFACE m_axi port=conv_weight_port_3 depth=165888
#pragma HLS INTERFACE m_axi port=conv_weight_port_4 depth=165888
#pragma HLS INTERFACE m_axi port=conv_weight_port_5 depth=165888
#pragma HLS INTERFACE m_axi port=conv_weight_port_6 depth=165888
#pragma HLS INTERFACE m_axi port=conv_weight_port_7 depth=165888

#pragma HLS INTERFACE m_axi port=conv_bias_port depth=768

#pragma HLS INTERFACE m_axi port=data_in_0 depth=13*13*32
#pragma HLS INTERFACE m_axi port=data_in_1 depth=13*13*32
#pragma HLS INTERFACE m_axi port=data_in_2 depth=13*13*32
#pragma HLS INTERFACE m_axi port=data_in_3 depth=13*13*32
#pragma HLS INTERFACE m_axi port=data_in_4 depth=13*13*32
#pragma HLS INTERFACE m_axi port=data_in_5 depth=13*13*32
#pragma HLS INTERFACE m_axi port=data_in_6 depth=13*13*32
#pragma HLS INTERFACE m_axi port=data_in_7 depth=13*13*32

#pragma HLS INTERFACE bram port=data_out_0
#pragma HLS RESOURCE variable=data_out_0 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_1
#pragma HLS RESOURCE variable=data_out_1 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_2
#pragma HLS RESOURCE variable=data_out_2 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_3
#pragma HLS RESOURCE variable=data_out_3 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_4
#pragma HLS RESOURCE variable=data_out_4 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_5
#pragma HLS RESOURCE variable=data_out_5 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_6
#pragma HLS RESOURCE variable=data_out_6 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_7
#pragma HLS RESOURCE variable=data_out_7 core=RAM_1P_BRAM

#endif


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    cout << "Starting inf_net_1 process..........................." << endl;
    cout << "..........................................................." << endl;
#endif
#endif

    int shift_weight_conv3_1 = 0;
      int shift_bias_conv3_1 = 0;

    data_type_o tmp_1_0_0[13*13*48];
    data_type_o tmp_1_0_1[13*13*48];
    data_type_o tmp_1_0_2[13*13*48];
    data_type_o tmp_1_0_3[13*13*48];
    data_type_o tmp_1_0_4[13*13*48];
    data_type_o tmp_1_0_5[13*13*48];
    data_type_o tmp_1_0_6[13*13*48];
    data_type_o tmp_1_0_7[13*13*48];

    conv_layer_acc_2(256, 3, 384, 13, 13, 13, 13, 1, 1, 1,
                     conv_weight_port_0, conv_weight_port_1, conv_weight_port_2, conv_weight_port_3,
                     conv_weight_port_4, conv_weight_port_5, conv_weight_port_6, conv_weight_port_7,
                     conv_bias_port,
                     shift_weight_conv3_1, shift_bias_conv3_1, 0, 0,
                     data_in_0, data_in_1, data_in_2, data_in_3, data_in_4, data_in_5, data_in_6, data_in_7,
                     tmp_1_0_0, tmp_1_0_1, tmp_1_0_2, tmp_1_0_3, tmp_1_0_4, tmp_1_0_5, tmp_1_0_6, tmp_1_0_7);

    int shift_weight_conv4_1 = 110592;
    int shift_bias_conv4_1 = 384;
//    int shift_weight_conv4_2 = 1558560;
//    int shift_bias_conv4_2 = 928;

    conv_layer_acc_3(384, 3, 384, 13, 13, 13, 13, 1, 1, 1,
                     conv_weight_port_0, conv_weight_port_1, conv_weight_port_2, conv_weight_port_3,
                     conv_weight_port_4, conv_weight_port_5, conv_weight_port_6, conv_weight_port_7,
                     conv_bias_port,
                     shift_weight_conv4_1, shift_bias_conv4_1, 0, 0,
                     tmp_1_0_0, tmp_1_0_1, tmp_1_0_2, tmp_1_0_3, tmp_1_0_4, tmp_1_0_5, tmp_1_0_6, tmp_1_0_7,
                     data_out_0, data_out_1, data_out_2, data_out_3, data_out_4, data_out_5, data_out_6,
                     data_out_7);


    /*
      int in_shift_4 = 192*13*13;
      int out_shift_4 = 192*13*13;

      int shift_weight_conv5_1 = 1890336;
      int shift_bias_conv5_1 = 1120;
      int shift_weight_conv5_2 = 2111520;
      int shift_bias_conv5_2 = 1248;
      int in_shift_5 = 192*13*13;
      int out_shift_5 = 128*13*13;


   //   data_type_w *fc_weight_port,
   //   data_type_w *fc_bias_port,

      int shift_weight_fc1 = 0;
      int shift_bias_fc1 = 0;

      int shift_weight_fc2 = 37748736;
      int shift_bias_fc2 = 4096;

      int shift_weight_fc3 = 54525952;
      int shift_bias_fc3 = 8192;
   */



 /*
   conv_layer_new(256, 3, 384, 13, 13, 1, 1, 1, output_temp_2, conv_weight_port, conv_bias_port, output_temp_1, shift_weight_conv3_1, shift_bias_conv3_1, 0, 0);
   clean_5:   for (int addr = 0; addr < 290400; addr++) {   output_temp_2[addr] = data_type_o(0);   }

   conv_layer_new(192, 3, 192, 13, 13, 1, 1, 1, output_temp_1, conv_weight_port, conv_bias_port, output_temp_2, shift_weight_conv4_1, shift_bias_conv4_1, 0, 0);
   conv_layer_new(192, 3, 192, 13, 13, 1, 1, 1, output_temp_1, conv_weight_port, conv_bias_port, output_temp_2, shift_weight_conv4_2, shift_bias_conv4_2, in_shift_4, out_shift_4);
   clean_6:   for (int addr = 0; addr < 290400; addr++) {   output_temp_1[addr] = data_type_o(0);   }

   conv_layer_new(192, 3, 128, 13, 13, 1, 1, 1, output_temp_2, conv_weight_port, conv_bias_port, output_temp_1, shift_weight_conv5_1, shift_bias_conv5_1, 0, 0);
   conv_layer_new(192, 3, 128, 13, 13, 1, 1, 1, output_temp_2, conv_weight_port, conv_bias_port, output_temp_1, shift_weight_conv5_2, shift_bias_conv5_2, in_shift_5, out_shift_5);
   clean_7:   for (int addr = 0; addr < 290400; addr++) {   output_temp_2[addr] = data_type_o(0);   }

   max_pool_layer_new_noact(13, 13, 256, 3, 6, 6, 2, 0, output_temp_1, output_temp_2);
   clean_8:   for (int addr = 0; addr < 290400; addr++) {   output_temp_1[addr] = data_type_o(0);   }

   conv_layer_new(256, 6, 4096, 1, 1, 6, 0, 1, output_temp_2, fc_weight_port, fc_bias_port, output_temp_1, shift_weight_fc1, shift_bias_fc1, 0, 0);
   clean_9:   for (int addr = 0; addr < 290400; addr++) {   output_temp_2[addr] = data_type_o(0);   }

   conv_layer_new(4096, 1, 4096, 1, 1, 1, 0, 1, output_temp_1, fc_weight_port, fc_bias_port, output_temp_2, shift_weight_fc2, shift_bias_fc2, 0, 0);
   clean_10:   for (int addr = 0; addr < 290400; addr++) {   output_temp_1[addr] = data_type_o(0);   }

   conv_layer_new(4096, 1, 1000, 1, 1, 1, 0, 1, output_temp_2, fc_weight_port, fc_bias_port, output_temp_1, shift_weight_fc3, shift_bias_fc3, 0, 0);
   */
//   for (int i = 0; i < 1000; i++) {
//      fc_8_out_a[i] = output_temp_1[i];
//   }


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    cout << "Finished forward network process .........................." << endl;
    cout << "..........................................................." << endl;
#endif
#endif
};


void inf_net_2(
        data_type_itf conv_weight_port_0[384*(32)*9],
        data_type_itf conv_weight_port_1[384*(32)*9],
        data_type_itf conv_weight_port_2[384*(32)*9],
        data_type_itf conv_weight_port_3[384*(32)*9],
        data_type_itf conv_weight_port_4[384*(32)*9],
        data_type_itf conv_weight_port_5[384*(32)*9],
        data_type_itf conv_weight_port_6[384*(32)*9],
        data_type_itf conv_weight_port_7[384*(32)*9],

        data_type_itf conv_bias_port[256+384],

        data_type_itf fc_layer_weights_0[7327744],
        data_type_itf fc_layer_weights_1[7327744],
        data_type_itf fc_layer_weights_2[7327744],
        data_type_itf fc_layer_weights_3[7327744],
        data_type_itf fc_layer_weights_4[7327744],
        data_type_itf fc_layer_weights_5[7327744],
        data_type_itf fc_layer_weights_6[7327744],
        data_type_itf fc_layer_weights_7[7327744],

        data_type_itf fc_layer_bias[9192],

        data_type_itf data_in_0[13*13*32],
        data_type_itf data_in_1[13*13*32],
        data_type_itf data_in_2[13*13*32],
        data_type_itf data_in_3[13*13*32],
        data_type_itf data_in_4[13*13*32],
        data_type_itf data_in_5[13*13*32],
        data_type_itf data_in_6[13*13*32],
        data_type_itf data_in_7[13*13*32],

        data_type_itf data_out_0[128],
        data_type_itf data_out_1[128],
        data_type_itf data_out_2[128],
        data_type_itf data_out_3[128],
        data_type_itf data_out_4[128],
        data_type_itf data_out_5[128],
        data_type_itf data_out_6[128],
        data_type_itf data_out_7[128]) {

#if _HLS_MODE_

#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS

#pragma HLS INTERFACE m_axi port=conv_weight_port_0 depth=165888
#pragma HLS INTERFACE m_axi port=conv_weight_port_1 depth=165888
#pragma HLS INTERFACE m_axi port=conv_weight_port_2 depth=165888
#pragma HLS INTERFACE m_axi port=conv_weight_port_3 depth=165888
#pragma HLS INTERFACE m_axi port=conv_weight_port_4 depth=165888
#pragma HLS INTERFACE m_axi port=conv_weight_port_5 depth=165888
#pragma HLS INTERFACE m_axi port=conv_weight_port_6 depth=165888
#pragma HLS INTERFACE m_axi port=conv_weight_port_7 depth=165888

#pragma HLS INTERFACE m_axi port=conv_bias_port depth=768

#pragma HLS INTERFACE m_axi port=fc_layer_weights_0 depth=7327744
#pragma HLS INTERFACE m_axi port=fc_layer_weights_1 depth=7327744
#pragma HLS INTERFACE m_axi port=fc_layer_weights_2 depth=7327744
#pragma HLS INTERFACE m_axi port=fc_layer_weights_3 depth=7327744
#pragma HLS INTERFACE m_axi port=fc_layer_weights_4 depth=7327744
#pragma HLS INTERFACE m_axi port=fc_layer_weights_5 depth=7327744
#pragma HLS INTERFACE m_axi port=fc_layer_weights_6 depth=7327744
#pragma HLS INTERFACE m_axi port=fc_layer_weights_7 depth=7327744

#pragma HLS INTERFACE m_axi port=fc_layer_bias depth=9192

#pragma HLS INTERFACE m_axi port=data_in_0 depth=13*13*32
#pragma HLS INTERFACE m_axi port=data_in_1 depth=13*13*32
#pragma HLS INTERFACE m_axi port=data_in_2 depth=13*13*32
#pragma HLS INTERFACE m_axi port=data_in_3 depth=13*13*32
#pragma HLS INTERFACE m_axi port=data_in_4 depth=13*13*32
#pragma HLS INTERFACE m_axi port=data_in_5 depth=13*13*32
#pragma HLS INTERFACE m_axi port=data_in_6 depth=13*13*32
#pragma HLS INTERFACE m_axi port=data_in_7 depth=13*13*32

#pragma HLS INTERFACE bram port=data_out_0
#pragma HLS RESOURCE variable=data_out_0 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_1
#pragma HLS RESOURCE variable=data_out_1 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_2
#pragma HLS RESOURCE variable=data_out_2 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_3
#pragma HLS RESOURCE variable=data_out_3 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_4
#pragma HLS RESOURCE variable=data_out_4 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_5
#pragma HLS RESOURCE variable=data_out_5 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_6
#pragma HLS RESOURCE variable=data_out_6 core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=data_out_7
#pragma HLS RESOURCE variable=data_out_7 core=RAM_1P_BRAM

#endif


#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
    cout << "Starting inf_net_2 process..........................." << endl;
    cout << "..........................................................." << endl;
#endif
#endif

    data_type_o tmp_2_0_0[13*13*32];
    data_type_o tmp_2_0_1[13*13*32];
    data_type_o tmp_2_0_2[13*13*32];
    data_type_o tmp_2_0_3[13*13*32];
    data_type_o tmp_2_0_4[13*13*32];
    data_type_o tmp_2_0_5[13*13*32];
    data_type_o tmp_2_0_6[13*13*32];
    data_type_o tmp_2_0_7[13*13*32];

int shift_weight_conv1_1 = 1890336/8;
int shift_bias_conv1_1 = 1120;

conv_layer_acc_4(384, 3, 256, 13, 13, 13, 13, 1, 1, 1,
                     conv_weight_port_0, conv_weight_port_1, conv_weight_port_2, conv_weight_port_3,
                     conv_weight_port_4, conv_weight_port_5, conv_weight_port_6, conv_weight_port_7,
                     conv_bias_port,
                     shift_weight_conv1_1, shift_bias_conv1_1, 0, 0,
                     data_in_0, data_in_1, data_in_2, data_in_3, data_in_4, data_in_5, data_in_6, data_in_7,
                     tmp_2_0_0, tmp_2_0_1, tmp_2_0_2, tmp_2_0_3, tmp_2_0_4, tmp_2_0_5, tmp_2_0_6, tmp_2_0_7);

    data_type_o tmp_2_1_0[6*6*32];
    data_type_o tmp_2_1_1[6*6*32];
    data_type_o tmp_2_1_2[6*6*32];
    data_type_o tmp_2_1_3[6*6*32];
    data_type_o tmp_2_1_4[6*6*32];
    data_type_o tmp_2_1_5[6*6*32];
    data_type_o tmp_2_1_6[6*6*32];
    data_type_o tmp_2_1_7[6*6*32];

max_pool_layer_acc_3(13, 13, 256, 3, 6, 6, 2, 0, 1,
                         tmp_2_0_0, tmp_2_0_1, tmp_2_0_2, tmp_2_0_3, tmp_2_0_4, tmp_2_0_5, tmp_2_0_6, tmp_2_0_7,
                         tmp_2_1_0, tmp_2_1_1, tmp_2_1_2, tmp_2_1_3, tmp_2_1_4, tmp_2_1_5, tmp_2_1_6, tmp_2_1_7);

    data_type_o tmp_2_2_0[5120];
    data_type_o tmp_2_2_1[5120];
    data_type_o tmp_2_2_2[5120];
    data_type_o tmp_2_2_3[5120];
    data_type_o tmp_2_2_4[5120];
    data_type_o tmp_2_2_5[5120];
    data_type_o tmp_2_2_6[5120];
    data_type_o tmp_2_2_7[5120];

      int shift_weight_fc1 = 0;
      int shift_bias_fc1 = 0;

      int shift_weight_fc2 = 37748736/8;
      int shift_bias_fc2 = 4096;

      int shift_weight_fc3 = 54525952/8;
      int shift_bias_fc3 = 8192;

#pragma HLS allocation instances=fc_layer_acc_0 limit=1 function

 fc_layer_acc_0(256, 6, 4096, 6, 6, 1, 1, 6, 0, 1,
                     fc_layer_weights_0, fc_layer_weights_1, fc_layer_weights_2, fc_layer_weights_3,
                     fc_layer_weights_4, fc_layer_weights_5, fc_layer_weights_6, fc_layer_weights_7,
                     fc_layer_bias,
                     shift_weight_fc1, shift_bias_fc1, 0, 0,
                     tmp_2_1_0, tmp_2_1_1, tmp_2_1_2, tmp_2_1_3, tmp_2_1_4, tmp_2_1_5, tmp_2_1_6, tmp_2_1_7,
                     tmp_2_2_0, tmp_2_2_1, tmp_2_2_2, tmp_2_2_3, tmp_2_2_4, tmp_2_2_5, tmp_2_2_6, tmp_2_2_7);
fc_layer_acc_0(4096, 1, 4096, 1, 1, 1, 1, 1, 0, 1,
                     fc_layer_weights_0, fc_layer_weights_1, fc_layer_weights_2, fc_layer_weights_3,
                     fc_layer_weights_4, fc_layer_weights_5, fc_layer_weights_6, fc_layer_weights_7,
                     fc_layer_bias,
                     shift_weight_fc2, shift_bias_fc2, 0, 0,
                     tmp_2_2_0, tmp_2_2_1, tmp_2_2_2, tmp_2_2_3, tmp_2_2_4, tmp_2_2_5, tmp_2_2_6, tmp_2_2_7,
                     tmp_2_1_0, tmp_2_1_1, tmp_2_1_2, tmp_2_1_3, tmp_2_1_4, tmp_2_1_5, tmp_2_1_6, tmp_2_1_7);
fc_layer_acc_0(4096, 1, 1000, 1, 1, 1, 1, 1, 0, 1,
                     fc_layer_weights_0, fc_layer_weights_1, fc_layer_weights_2, fc_layer_weights_3,
                     fc_layer_weights_4, fc_layer_weights_5, fc_layer_weights_6, fc_layer_weights_7,
                     fc_layer_bias,
                     shift_weight_fc3, shift_bias_fc3, 0, 0,
                     tmp_2_1_0, tmp_2_1_1, tmp_2_1_2, tmp_2_1_3, tmp_2_1_4, tmp_2_1_5, tmp_2_1_6, tmp_2_1_7,
                     tmp_2_2_0, tmp_2_2_1, tmp_2_2_2, tmp_2_2_3, tmp_2_2_4, tmp_2_2_5, tmp_2_2_6, tmp_2_2_7);


                    for (int i = 0; i < 128; i+=8)
                    {
                        data_out_0[i] = tmp_2_2_0[i];
                        data_out_1[i] = tmp_2_2_1[i];
                        data_out_2[i] = tmp_2_2_2[i];
                        data_out_3[i] = tmp_2_2_3[i];
                        data_out_4[i] = tmp_2_2_4[i];
                        data_out_5[i] = tmp_2_2_5[i];
                        data_out_6[i] = tmp_2_2_6[i];
                        data_out_7[i] = tmp_2_2_7[i];
                    }

 };

#endif //_CONSTRUCT_NET_H_
