// This file contains the global configurations of the feedforward net.
// Created by yaochen on 20/12/16.
//TODO: add all the global configurations in.
//TODO: modify global configuration representations.

#ifndef _CONFIG_H_
#define _CONFIG_H_

//activation function selection
#define _ACT_RELU_ 1

//define data type
//typedef ap_fixed<64,32> data_type;
//typedef ap_fixed<32,16> data_type;
typedef ap_fixed<32,10> data_type;
typedef ap_fixed<16,2> data_type_w;
typedef ap_fixed<32,10> data_type_o;
//typedef ap_fixed<32,8> data_type;
//typedef float data_type;

// C++ compilation debug mode
#define _HLS_MODE_  1

//#define _BATCH_MODE_ 1

#ifdef _BATCH_MODE_
#define _KERNEL_DEBUG_ 0
#else
#define _KERNEL_DEBUG_ 1
#endif

#ifdef _HLS_MODE_
#define _C_DEBUG_MODE_ 0
#else
#define _C_DEBUG_MODE_ 1
#endif

//network configuration PARAMETERS
int weight_bias_record = 0;
int weight_bias_count_1 = 0;
int weight_bias_count_2 = 0;

int nn_in_data_size_conv[5] = { 32,16,8,6,4 };
int nn_in_number_conv[5] = { 3,48,256,192,192 };
int nn_out_number_conv[5] = { 96,256,384,384,256 };
int nn_channel_size_conv[5] = { 3,3,3,3,3 };

int nn_in_data_size_pooling[3] = { 32,16,4 };
int nn_in_number_pooling[3] = { 96,256,256 };
int nn_channel_size_pooling[3] = { 2,2,2 };

int nn_in_data_size_fc[3] = { 1,1,1 };
int nn_in_number_fc[3] = { 256,4096,4096 };
int nn_out_number_fc[3] = { 4096,4096,10 };
int nn_channel_size_fc[3] = { 1,1,1 };

int nn_local_size_lrn[2] = { 5,5 };
float nn_alpha_lrn[2] = { 0.0001,0.0001 };
float nn_beta_lrn[2] = { 0.75,0.75 };
//float dropout_ratio = 0.5;

//#define O true
//#define X false
//const bool tbl[6][16] = {
//	O, X, X, X, O, O, O, X, X, O, O, O, O, X, O, O,
//	O, O, X, X, X, O, O, O, X, X, O, O, O, O, X, O,
//	O, O, O, X, X, X, O, O, O, X, X, O, X, O, O, O,
//	X, O, O, O, X, X, O, O, O, O, X, X, O, X, O, O,
//	X, X, O, O, O, X, X, O, O, O, O, X, O, O, X, O,
//	X, X, X, O, O, O, X, X, O, O, O, O, X, O, O, O };
//#undef O
//#undef X

#endif //FFNET_CONFIG_H
