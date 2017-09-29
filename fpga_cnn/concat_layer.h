
#ifndef _CONCAT_LAYER_H_
#define _CONCAT_LAYER_H_

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
class concat_layer {

private:
	int concat_layer_num;

public:
	concat_layer() :concat_layer_num(0) {};

	/************************************************************************************************/
	void concat_layer_a(
		T *in_data3D_1,
		T *in_data3D_2) {
#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        cout << "Starting concat layer ...." << endl;
#endif
#endif
			for(int n = 0; n < _IN_CHANNEL_NUM_; n++){
				for(int i=0;i<_INPUT_SIZE_;i++){
					for(int j=0;j<_INPUT_SIZE_;j++){
					    (*(in_data3D_2 + (_IN_CHANNEL_NUM_ + n)*_INPUT_SIZE_*_INPUT_SIZE_ + i*_INPUT_SIZE_ +j)) 
					    = (*(in_data3D_1 + n*_INPUT_SIZE_*_INPUT_SIZE_ + i*_INPUT_SIZE_ +j));
				    }
				}
			}
#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        cout << "Finished concat layer ...." << endl;
		ofstream out_concat_a;
		out_concat_a.open("concat_layer_a.txt", ios::app);
		out_concat_a << "output from concat layer .........................." << endl;
		for(int n = 0; n < 2 * _IN_CHANNEL_NUM_; n++){
			for(int i=0;i<_INPUT_SIZE_;i++){
				for(int j=0;j<_INPUT_SIZE_;j++){
				    out_concat_a << *(in_data3D_2 + n*_INPUT_SIZE_*_INPUT_SIZE_ + i*_INPUT_SIZE_ +j) << " ";
				}
				out_concat_a << endl;
			}
			out_concat_a << endl;
		}
		out_concat_a.close();
		cout << endl;
#endif
#endif
	}
};
#endif