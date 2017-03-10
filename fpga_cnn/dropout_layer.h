
#ifndef _DROPOUT_LAYER_H_
#define _DROPOUT_LAYER_H_

#include <iostream>
#include <fstream>
//#include <algorithm>
//#include <math.h>
#include "config.h"
//#include "data_type.h"
//#include "activation_functions.h"

using namespace std;

template <typename T, int _IN_CHANNEL_NUM_, int _INPUT_SIZE_>
class dropout_layer {

private:
	int dropout_layer_num;

public:
	dropout_layer() :dropout_layer_num(0) {};
	/************************************************************************************************/
	//dropout layer function with array input
	void dropout_layer_a(
		T dropout_ratio,
		T in_data3D[_IN_CHANNEL_NUM_][_INPUT_SIZE_][_INPUT_SIZE_],
		T out_data3D[_IN_CHANNEL_NUM_][_INPUT_SIZE_][_INPUT_SIZE_]) {
		for (int a = 0; a < _IN_CHANNEL_NUM_; a++) {
			for (int i = 0; i < _INPUT_SIZE_; ++i) {
				for (int j = 0; j < _INPUT_SIZE_; ++j) {
					out_data3D[a][i][j] = in_data3D[a][i][j] * (1 - dropout_ratio);
				}
			}
		}

//#if _C_DEBUG_MODE_
		ofstream out_dropout_a;
		out_dropout_a.open("dropout_layer_a.txt", ios::app);
		out_dropout_a << "output from dropout layer .........................." << endl;
		for (int i = 0; i < _IN_CHANNEL_NUM_; i++) {
			for (int j = 0; j < _INPUT_SIZE_; j++) {
				for (int k = 0; k < _INPUT_SIZE_; k++) {
					out_dropout_a << out_data3D[i][j][k] << " ";
				}
				out_dropout_a << endl;
			}
			out_dropout_a << endl;
		}
		out_dropout_a.close();
		cout << endl;
//#endif
	}
};

#endif