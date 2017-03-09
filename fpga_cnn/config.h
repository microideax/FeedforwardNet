// This file contains the global configurations of the feedforward net.
// Created by yaochen on 20/12/16.
//TODO: add all the global configurations in.
//TODO: modify global configuration representations.

#ifndef _CONFIG_H_
#define _CONFIG_H_

// C++ compilation debug mode
//#define _HLS_MODE_  1

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

int nn_in_data_size_conv[5] = { 227,27,13,13,13 };
int nn_in_number_conv[5] = { 3,48,256,192,192 };
int nn_out_number_conv[5] = {96, 256, 384,384,256};
int nn_channel_size_conv[5] = {11,5,3,3,3};
int nn_channel_number_conv[5] = { 96, 256, 384,384,256 };

int nn_in_data_size_pooling[3] = { 55,27,13 };
int nn_in_number_pooling[3] = { 96,256,256 };
int nn_out_number_pooling[3] = { 96,256,256 };
int nn_channel_number_pooling[3] = { 96,256,256 };
int nn_channel_size_pooling = 2;

//int nn_in_data_size_fc[1] = { 1 };
//int nn_in_number_fc[1] = { 120 };
//int nn_channel_size_fc = 1;
//int nn_channel_number_fc[1] = { 10 };

int nn_in_data_size_fc[3] = { 6,1,1 };
int nn_in_number_fc[3] = { 256,4096,4096 };
int nn_out_number_fc[3] = { 4096,4096,1000 };
int nn_channel_size_fc[3] = { 6,1,1 };
int nn_channel_number_fc[3] = { 4096,4096,1000 };

float alpha = 0.0001;
float beta = 0.75;
float dropout_ratio = 0.5;

float label_list[10] = { 0,1,2,3,4,5,6,7,8,9 };

#define O true
#define X false
const bool tbl[6][16] = {
	O, X, X, X, O, O, O, X, X, O, O, O, O, X, O, O,
	O, O, X, X, X, O, O, O, X, X, O, O, O, O, X, O,
	O, O, O, X, X, X, O, O, O, X, X, O, X, O, O, O,
	X, O, O, O, X, X, O, O, O, O, X, X, O, X, O, O,
	X, X, O, O, O, X, X, O, O, O, O, X, O, O, X, O,
	X, X, X, O, O, O, X, X, O, O, O, O, X, O, O, O };
#undef O
#undef X

#endif //FFNET_CONFIG_H
