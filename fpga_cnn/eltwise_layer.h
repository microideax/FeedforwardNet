
#ifndef _ELTWISE_LAYER_H_
#define _ELTWISE_LAYER_H_

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <fstream>
#include <algorithm>
#include <math.h>
//#include "config.h"
#include "pow_function.h"
#include "activation_functions.h"

using namespace std;

template <typename T, int _IN_CHANNEL_NUM_, int _INPUT_SIZE_>
class eltwise_layer {

private:
	int eltwise_layer_num;

public:
	eltwise_layer() :eltwise_layer_num(0) {};

	/************************************************************************************************/
	void eltwise_layer_a(
		T *in_data3D_1,
		T *in_data3D_2) {
#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        cout << "Starting eltwise layer ...." << endl;
#endif
#endif
			for(int n = 0; n < _IN_CHANNEL_NUM_; n++){
				for(int i=0;i<_INPUT_SIZE_;i++){
					for(int j=0;j<_INPUT_SIZE_;j++){
					    (*(in_data3D_2 + n*_INPUT_SIZE_*_INPUT_SIZE_ + i*_INPUT_SIZE_ +j)) 
					    += (*(in_data3D_1 + n*_INPUT_SIZE_*_INPUT_SIZE_ + i*_INPUT_SIZE_ +j));
				    }
				}
			}
#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        cout << "Finished eltwise layer ...." << endl;
		ofstream out_eltwise_a;
		out_eltwise_a.open("eltwise_layer_a.txt", ios::app);
		out_eltwise_a << "output from eltwise layer .........................." << endl;
		for(int n = 0; n < _IN_CHANNEL_NUM_; n++){
			for(int i=0;i<_INPUT_SIZE_;i++){
				for(int j=0;j<_INPUT_SIZE_;j++){
				    out_eltwise_a << *(in_data3D_2 + n*_INPUT_SIZE_*_INPUT_SIZE_ + i*_INPUT_SIZE_ +j) << " ";
				}
				out_eltwise_a << endl;
			}
			out_eltwise_a << endl;
		}
		out_eltwise_a.close();
#endif
#endif
	}
};
#endif