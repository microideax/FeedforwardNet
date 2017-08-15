
#ifndef _CONSTRUCT_NET_H_
#define _CONSTRUCT_NET_H_
#include <iostream>
#include <fstream>
#include <algorithm>
#include <ap_fixed.h>
#include "config.h"
#include "data_type.h"
//#include "weight_bias_one_dim.h"
//#include "activation_functions.h"
//#include "conv_layer_one_dim.h"
//#include "pool_layer_one_dim.h"
//#include "fc_layer_one_dim.h"
//#include "conv_acc_break.h"
//#include "conv_acc_break_noact.h"
//#include "max_pool_acc.h"
//#include "ave_pool_acc_noact.h"
#include "acc_instance.h"


void   inference_net(
   char   activation_type,
   data_type   in_data_3D[3*32*32],
   data_type_w *conv_weight_port,
   data_type_w *conv_bias_port,
   data_type_w *fc_weight_port,
   data_type_w *fc_bias_port,
   data_type_o   fc_out_a[10*1*1],
   data_type_o   output_temp_1[32768],
   data_type_o   output_temp_2[32768])   {

#if _HLS_MODE_
#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
#pragma HLS INTERFACE s_axilite port=activation_type bundle=CRTL_BUS

#pragma HLS INTERFACE m_axi depth=3072  port=in_data_3D
#pragma HLS INTERFACE m_axi depth=79200 port=conv_weight_port
#pragma HLS INTERFACE m_axi depth=128   port=conv_bias_port
#pragma HLS INTERFACE m_axi depth=10240 port=fc_weight_port
#pragma HLS INTERFACE m_axi depth=10    port=fc_bias_port
#pragma HLS INTERFACE m_axi depth=10    port=fc_out_a
#pragma HLS INTERFACE m_axi depth=32768 port=output_temp_1
#pragma HLS INTERFACE m_axi depth=32768 port=output_temp_2
#endif


   cout << "starting forward network process..........................." << endl;
   cout << "..........................................................." << endl;

   //conv_layer<data_type, data_type_w, data_type_o, 32, 5, 2, 1, 3, 32, 1> C1;
//   conv_acc_noact<data_type, data_type_w, data_type_o, 32, 3, 32, 32> convAcc1;//{0<Tm<=M;0<Tn<=N;0<Tr<=R;0<Tc<=C;}
   //pool_layer<data_type, data_type_w, data_type_o, 32, 3, 0, 2, 32> P1;
//   max_pool_acc<data_type, data_type_w, data_type_o, 32, 16, 16> maxPoolAcc1;//{0<Tm<=M;0<Tn<=N;0<Tr<=R;0<Tc<=C;}
   //conv_layer<data_type, data_type_w, data_type_o, 16, 5, 2, 1, 32, 32, 1> C2;
//   conv_acc<data_type, data_type_w, data_type_o, 32, 32, 16, 16> convAcc2;//{0<Tm<=M;0<Tn<=N;0<Tr<=R;0<Tc<=C;}
   //pool_layer<data_type, data_type_w, data_type_o, 16, 3, 0, 2, 32> P2;
//   ave_pool_acc_noact<data_type, data_type_w, data_type_o, 32, 4, 4> avePoolAcc1;//{0<Tm<=M;0<Tn<=N;0<Tr<=R;0<Tc<=C;}
   //conv_layer<data_type, data_type_w, data_type_o, 8, 5, 2, 1, 32, 64, 1> C3;
   //pool_layer<data_type, data_type_w, data_type_o, 8, 3, 0, 2, 64> P3;
   //fc_layer<data_type, data_type_w, data_type_o, 64, 4, 10> F1;


//   data_type_o in_data_buf[3*32*32];
//   data_type_o fc_4_out_buf[10];


//   for (int addr = 0; addr < 32768; addr++) {
//      output_temp_1[addr] = data_type_o(0);
//      output_temp_2[addr] = data_type_o(0);
//   }
//   for (int i = 0; i < 3; i++) {
//      for (int j = 0; j < 32; j++) {
//         for (int k = 0; k < 32; k++) {
//            in_data_buf[i*32*32 + j*32+ k] = in_data_3D[i*32*32 + j*32 + k];
//         }
//
//      }
//
//   }

    //offset list
    int shift_weight_conv2 = 2400;
    int shift_bias_conv2 = 32;
    int shift_weight_conv3 = 28000;
    int shift_bias_conv3 = 64;

    // conv-1
    conv_layer_new(3, 5, 32, 32, 32, 1, 2, 1, output_temp_2, conv_weight_port, conv_bias_port, output_temp_1, 0, 0, 0, 0);
//    maxPoolAcc1.max_pool_layer_acc(32,32,32, 3, 16, 16, 2, 0, output_temp_1, output_temp_2);

    ave_pool_layer_new(32, 32, 32, 3, 16, 16, 2, 0, 0, output_temp_1, output_temp_2);

    clean_1: for (int addr = 0; addr < 32768; addr++) { output_temp_1[addr] = data_type_o(0); }

    // conv-2
//    convAcc2.conv_layer_acc(32, 5, 32, 16, 16, 1, 2, output_temp_2, conv_weight_port + shift_weight_conv2, conv_bias_port + shift_bias_conv2, output_temp_1, 0, 0);
    conv_layer_new(32, 5, 32, 16, 16, 1, 2, 1, output_temp_2, conv_weight_port, conv_bias_port, output_temp_1, shift_weight_conv2, shift_bias_conv2, 0, 0);
    clean_2: for (int addr = 0; addr < 32768; addr++) { output_temp_2[addr] = data_type_o(0);  }
//    avePoolAcc1.ave_pool_layer_acc_noact(16,16,32, 3, 8, 8, 2, 0, output_temp_1, output_temp_2);

    ave_pool_layer_new(16, 16, 32, 3, 8, 8, 2, 0, 0, output_temp_1, output_temp_2);

    clean_3: for (int addr = 0; addr < 32768; addr++) { output_temp_1[addr] = data_type_o(0);  }

    //conv-3
//    convAcc2.conv_layer_acc(32, 5, 64, 8, 8, 1, 2, output_temp_2, conv_weight_port + shift_weight_conv3, conv_bias_port + shift_bias_conv3, output_temp_1, 0, 0);
    conv_layer_new(32, 5, 64, 8, 8, 1, 2, 1, output_temp_2, conv_weight_port, conv_bias_port,  output_temp_1, shift_weight_conv3, shift_bias_conv3, 0, 0);
    clean_4: for (int addr = 0; addr < 32768; addr++) { output_temp_2[addr] = data_type_o(0);  }
//    avePoolAcc1.ave_pool_layer_acc_noact(8,8,64, 3, 4, 4, 2, 0, output_temp_1, output_temp_2);
    ave_pool_layer_new(8, 8, 64, 3, 4, 4, 2, 0, 0, output_temp_1, output_temp_2);
    clean_5: for (int addr = 0; addr < 32768; addr++) { output_temp_1[addr] = data_type_o(0);  }

    // fc_1
//    convAcc1.conv_layer_acc_noact(64, 4, 10, 1, 1, 4, 0, output_temp_2, fc_weight_port, fc_bias_port, output_temp_1, 0, 0);
    conv_layer_new(64, 4, 10, 1, 1, 4, 0, 0, output_temp_2, fc_weight_port, fc_bias_port, output_temp_1, 0, 0, 0, 0);

    for (int i = 0; i < 10; i++) { fc_out_a[i] = output_temp_1[i];  }

    cout << "Finished forward network process .........................." << endl;
    cout << "..........................................................." << endl;

}
#endif //_CONSTRUCT_NET_H_
