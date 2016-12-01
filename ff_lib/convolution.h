#ifndef _CONVOLUTION_H_
#define _CONVOLUTION_H_

#include <iostream.h>
#include "../tiny_dnn/util.h"

void convolution(vec_t& in_data, 
				 std::vector<vec_t>& out_data,
				 vec_t& channel_weight,
				 cnn_size_t in_data_size,
				 cnn_size_t out_data_size,
				 cnn_size_t channel_size,
				 cnn_size_t channel_number);





#endif

