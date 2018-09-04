
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "ap_fixed.h"

//define data type
typedef float data_type;
typedef float data_type_w;
typedef float data_type_o;

// counter datatype used in the entire design
typedef ap_uint<16> data_type_c;
//typedef ap_fixed<16,0> param_type;

// C++ compilation debug mode
#define _LAYER_MODE_ 1

#ifdef _LAYER_MODE_
#define IBUF  (Tr-1)*S_max + K_max
//#define
#endif

// C++ compilation debug mode
//#ifdef _LAYER_MODE_
//#define _ACC_MODE_ 0
//#else
//#define _ACC_MODE_ 1
//#endif

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


//*****************************************************************//
// System buffer configuration
//const int IBUF_t = 32;
//const int WBUF_t = 32;
//const int OBUF_t = 32;
// conv accelerator configuration
//const int Tm = 16;
//const int Tn = 16;
//const int Tr = 8;
//const int Tc = 8;
//const int Pool_Tr = Tr/2 + Tr%2;
//const int Pool_Tc = Tc/2 + Tc%2;
//const int S_max = 5;
//const int K_max = 5;
//pooling accelerator configuration
//const int pTm = 4;
//const int pTn = 4;
//const int pTr = 8;
//const int pTc = 8;
//const int pS_max = 2;
//const int pK_max = 2;
//inner productoin configuration
//const int iTm = 4;
//const int iTn = 4;
//const int iS_max = 5;
//const int iK_max = 5;
//****************************************************************//

//network configuration PARAMETERS
int weight_bias_record = 0;
int weight_bias_count_1 = 0;
int weight_bias_count_2 = 0;
int nn_in_data_size_conv[2] = {28, 14};
int nn_in_number_conv[2] = {1, 6};
int nn_out_number_conv[2] = {6, 16};
int nn_channel_size_conv[2] = {5, 5};
int nn_padding_conv[2] = {2, 0};
int nn_group_conv[2] = {1, 1};
int nn_in_data_size_pooling[2] = {28, 10};
int nn_in_number_pooling[2] = {6, 16};
int nn_channel_size_pooling[2] = {2, 2};
int nn_in_data_size_fc[1] = {5};
int nn_in_number_fc[1] = {16};
int nn_out_number_fc[1] = {10};
int nn_channel_size_fc[1] = {5};

struct pool_param_reg {
    int n;
    int r;
    int c;
    int S;
    int K;
    int P;
    int R;
    int C;
    int N;
    int R_IN;
    int C_IN;
    int TR;
    int TC;
    bool i_flag;
    bool o_flag;
} pool_regs;


#endif
