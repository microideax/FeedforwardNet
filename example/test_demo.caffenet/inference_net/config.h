
#ifndef _CONFIG_H_
#define _CONFIG_H_

// Constant value, do not change
typedef ap_fixed<32,16,AP_TRN_ZERO,AP_SAT> data_type_itf;

//define data type
typedef ap_fixed<16,8> data_type;
typedef ap_fixed<16,8> data_type_w;
typedef ap_fixed<16,8> data_type_o;


#define FOLD_FACTOR_i = int(sizeof(data_type_itf)/sizeof(data_type));
#define FOLD_FACTOR_w = int(sizeof(data_type_itf)/sizeof(data_type_w));
#define FOLD_FACTOR_o = int(sizeof(data_type_itf)/sizeof(data_type_o));

// C++ compilation debug mode
#define _LAYER_MODE_ 1

// C++ compilation debug mode
#define _HLS_MODE_  1

// C++ compilation debug mode
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


int nn_in_data_size_conv[5] = {227, 27, 13, 13, 13};
int nn_in_number_conv[5] = {3, 48, 256, 192, 192};
int nn_out_number_conv[5] = {96, 256, 384, 384, 256};
int nn_channel_size_conv[5] = {11, 5, 3, 3, 3};
int nn_padding_conv[5] = {0, 2, 1, 1, 1};
int nn_group_conv[5] = {1, 2, 1, 2, 2};
int nn_in_data_size_pooling[3] = {55, 27, 13};
int nn_in_number_pooling[3] = {96, 256, 256};
int nn_channel_size_pooling[3] = {3, 3, 3};
int nn_in_data_size_fc[3] = {6, 1, 1};
int nn_in_number_fc[3] = {256, 4096, 4096};
int nn_out_number_fc[3] = {4096, 4096, 1000};
int nn_channel_size_fc[3] = {6, 1, 1};


#endif
