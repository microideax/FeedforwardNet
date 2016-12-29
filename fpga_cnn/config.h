// This file contains the global configurations of the feedforward net.
// Created by yaochen on 20/12/16.
//TODO: add all the global configurations in.

#ifndef _CONFIG_H_
#define _CONFIG_H_

// C++ compilation debug mode
#define _HLS_MODE_  1

#ifdef _HLS_MODE_
#define _C_DEBUG_MODE_ 0
#else
#define _C_DEBUG_MODE_ 1
#endif




//network configuration PARAMETERS
#define KERNEL_SIZE 5
#define INPUT_SIZE 32
#define IN_CHANNEL_NUM 1
#define OUT_CHANNEL_NUM 6


//const int nn_in_data_size_conv[3] = { 32,14,5 };
//const int nn_channel_size_conv = 5;

#define O true
#define X false
const bool tbl[6][16] = {
        O, X, X, X, O, O, O, X, X, O, O, O, O, X, O, O,
        O, O, X, X, X, O, O, O, X, X, O, O, O, O, X, O,
        O, O, O, X, X, X, O, O, O, X, X, O, X, O, O, O,
        X, O, O, O, X, X, O, O, O, O, X, X, O, X, O, O,
        X, X, O, O, O, X, X, O, O, O, O, X, O, O, X, O,
        X, X, X, O, O, O, X, X, O, O, O, O, X, O, O, O};
#undef O
#undef X

#endif //FFNET_CONFIG_H
