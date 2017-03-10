
#ifndef _LRN_LAYER_H_
#define _LRN_LAYER_H_

#include <iostream>
#include <fstream>
#include <algorithm>
#include <math.h> 
#include "config.h"
#include "data_type.h"
#include "activation_functions.h"

using namespace std;

template <typename T, int _IN_CHANNEL_NUM_, int _LOCAL_SIZE_, int _INPUT_SIZE_>
class lrn_layer {

private:
	int lrn_layer_num;

public:
	lrn_layer() :lrn_layer_num(0) {};

	/************************************************************************************************/
	void lrn_local_a_within_channel(
		T alpha,
		T beta,
		T in_data[_INPUT_SIZE_][_INPUT_SIZE_],
		T out_data[_INPUT_SIZE_][_INPUT_SIZE_]) {
		for (int i = 0; i < _INPUT_SIZE_ ; ++i) {
			for (int j = 0; j < _INPUT_SIZE_ ; ++j) {
				T data = 0;
				for (int ii = - _LOCAL_SIZE_ / 2; ii <= _LOCAL_SIZE_ / 2; ++ii) {
					for (int jj = - _LOCAL_SIZE_ / 2; jj <= _LOCAL_SIZE_ / 2; ++jj) {
						if (i + ii >= 0 && i + ii<_INPUT_SIZE_&&j + jj >= 0 && j + jj<_INPUT_SIZE_) {//if overlapped
							data += pow(in_data[i + ii][j + jj], 2);
						}
					}
				}
				out_data[i][j] = in_data[i][j]*pow(1+( alpha / _LOCAL_SIZE_ ) * data, - beta) ;
			}
		}
	}

	void lrn_local_a_across_channels(
		T alpha,
		T beta,
		int a,
		T in_data[_IN_CHANNEL_NUM_][_INPUT_SIZE_][_INPUT_SIZE_],
		T out_data[_IN_CHANNEL_NUM_][_INPUT_SIZE_][_INPUT_SIZE_]) {
		//for (int i = a; i < _IN_CHANNEL_NUM_; ++i) {
			for (int j = 0; j < _INPUT_SIZE_; ++j) {
				for (int k = 0; k < _INPUT_SIZE_; ++k) {
					T data = 0;
					for (int ii = -_LOCAL_SIZE_ / 2; ii <= _LOCAL_SIZE_ / 2; ++ii) {
						if (a + ii >= 0 && a + ii<_IN_CHANNEL_NUM_) {//if in all channels
							data += pow(in_data[a+ii][j][k], 2);
						}
					}
					out_data[a][j][k] = in_data[a][j][k] * pow(1 + (alpha / _LOCAL_SIZE_) * data, -beta);
				}
			}
		//}
	}

	/************************************************************************************************/
	//lrn layer function with array input
	void lrn_layer_a(
		T alpha,
		T beta,
		T in_data3D[_IN_CHANNEL_NUM_][_INPUT_SIZE_][_INPUT_SIZE_],
		T out_data3D[_IN_CHANNEL_NUM_][_INPUT_SIZE_][_INPUT_SIZE_]) {
		assert(_LOCAL_SIZE_ % 2 == 1);//LRN only supports odd values for local_size
		for (int a = 0; a < _IN_CHANNEL_NUM_; a++) {//input kernel loop
			lrn_local_a_across_channels(alpha, beta,a,in_data3D,out_data3D);
			//lrn_local_a_within_channel(alpha, beta, in_data3D[a], out_data3D[a]);
		}
#if _C_DEBUG_MODE_
		ofstream out_lrn_a;
		out_lrn_a.open("lrn_layer_a.txt", ios::app);
		out_lrn_a << "output from lrn layer .........................." << endl;
		for (int i = 0; i < _IN_CHANNEL_NUM_; i++) {
			for (int j = 0; j < _INPUT_SIZE_; j++) {
				for (int k = 0; k < _INPUT_SIZE_; k++) {
					out_lrn_a << out_data3D[i][j][k] << " ";
				}
				out_lrn_a << endl;
			}
			out_lrn_a << endl;
		}
		out_lrn_a.close();
		cout << endl;
	}
#endif
};

#endif