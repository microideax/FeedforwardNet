
#ifndef _SCALE_LAYER_H_
#define _SCALE_LAYER_H_

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
class scale_layer {

private:
	int scale_layer_num;

public:
	scale_layer() :scale_layer_num(0) {};

	/************************************************************************************************/
	void scale_layer_a(
		T *gamma,
		T *beta,
		T *in_data3D,
		T *out_data3D) {
#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        cout << "Starting scale layer ...." << endl;
#endif
#endif
		    T gamma_=0;
		    T beta_=0;
			for(int n = 0; n < _IN_CHANNEL_NUM_; n++){
				gamma_ = (*(gamma + n));
			    beta_ = (*(beta + n));
				for(int i=0;i<_INPUT_SIZE_;i++){
					for(int j=0;j<_INPUT_SIZE_;j++){
					    (*(out_data3D + n*_INPUT_SIZE_*_INPUT_SIZE_ + i*_INPUT_SIZE_ +j)) = relu(gamma_* (*(in_data3D + n*_INPUT_SIZE_*_INPUT_SIZE_ + i*_INPUT_SIZE_ +j)) + beta_);
				    }
				}
			}
#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        cout << "Finished scale layer ...." << endl;
		ofstream out_scale_a;
		out_scale_a.open("scale_layer_a.txt", ios::app);
		out_scale_a << "output from scale layer .........................." << endl;
		for(int n = 0; n < _IN_CHANNEL_NUM_; n++){
			for(int i=0;i<_INPUT_SIZE_;i++){
				for(int j=0;j<_INPUT_SIZE_;j++){
				    out_scale_a << *(out_data3D + n*_INPUT_SIZE_*_INPUT_SIZE_ + i*_INPUT_SIZE_ +j) << " ";
				}
				out_scale_a << endl;
			}
			out_scale_a << endl;
		}
		out_scale_a.close();
		cout << endl;
#endif
#endif
	}
};
#endif