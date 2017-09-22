
#ifndef _CONFIG_H_
#define _CONFIG_H_


//define data type
typedef float data_type;
typedef float data_type_w;
typedef float data_type_o;

// C++ compilation debug mode
#define _HLS_MODE_  1

// C++ compilation debug mode
//#define _BATCH_MODE_ 1
//#define _BATCH_NORM_  1
//#define _SCALE_ 1
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


int nn_in_data_size_conv[13] = {224, 224, 112, 112, 56, 56, 56, 28, 28, 28, 14, 14, 14};
int nn_in_number_conv[13] = {3, 64, 64, 128, 128, 256, 256, 256, 512, 512, 512, 512, 512};
int nn_out_number_conv[13] = {64, 64, 128, 128, 256, 256, 256, 512, 512, 512, 512, 512, 512};
int nn_channel_size_conv[13] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
int nn_padding_conv[13] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int nn_group_conv[13] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int nn_in_data_size_pooling[5] = {224, 112, 56, 28, 14};
int nn_in_number_pooling[5] = {64, 128, 256, 512, 512};
int nn_channel_size_pooling[5] = {2, 2, 2, 2, 2};
int nn_in_data_size_fc[3] = {7, 1, 1};
int nn_in_number_fc[3] = {512, 4096, 4096};
int nn_out_number_fc[3] = {4096, 4096, 1000};
int nn_channel_size_fc[3] = {7, 1, 1};


#endif
