//
// Created by yaochen on 29/12/16.
//

#ifndef _FC_LAYER_H_
#define _FC_LAYER_H_

#include "config.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include "data_type.h"
#include "activation_functions.h"


using namespace std;

template <typename T, int _IN_CHANNEL_NUM_, int _INPUT_SIZE_, int _OUT_CHANNEL_NUM_>
class fc_layer {

private:
	int fc_layer_num;

public:
	fc_layer() :fc_layer_num(0) {};

	/************************************************************************************************/
	//fc kernel function with array input
	void fc_kernel_a(
		T in_data[_INPUT_SIZE_][_INPUT_SIZE_],
		T kernel_weights[_INPUT_SIZE_][_INPUT_SIZE_],
//		T kernel_weight,
		T& out_data) {
		T sum = 0;
		for (uint i = 0; i < _INPUT_SIZE_; i++)
		{
			for (uint j = 0; j < _INPUT_SIZE_; j++)
			{
				T data = in_data[i][j];
				T weight = kernel_weights[i][j];
//                T weight = kernel_weight;
				sum += data * weight;
			}
		}
		out_data = sum;

#if _C_DEBUG_MODE_
//		cout << "array fc kernel output ...." << endl;
		//ofstream fc_kernel_a;
		//fc_kernel_a.open("fc_kernel_a.txt", ios::app);
		//fc_kernel_a << "fc kernel in data" << endl;
		//for (int i = 0; i < _INPUT_SIZE_; i++) {
		//	for (int j = 0; j < _INPUT_SIZE_; j++) {
		//		fc_kernel_a << in_data[i][j] << " ";
		//	}
		//	fc_kernel_a << endl;
		//}
		//fc_kernel_a << endl;
		//fc_kernel_a << "fc kernel in weight" << endl;

		//for (int i = 0; i < _INPUT_SIZE_; i++) {
		//	for (int j = 0; j < _INPUT_SIZE_; j++) {
		//		fc_kernel_a << kernel_weights[i][j] << " ";
		//	}
		//	fc_kernel_a << endl;
		//}
  //      /*fc_kernel_a << kernel_weight << endl;
		//fc_kernel_a << endl;*/

		//fc_kernel_a << "fc kernel out data" << endl;
		////for (int i = 0; i < 1; i++) {
		////    for (int j = 0; j < 1; j++) {
		//fc_kernel_a << out_data << " ";
		////    }
		////    fc_kernel_a << endl;
		////}
		//fc_kernel_a << endl;
		//fc_kernel_a.close();
//		cout << endl;
#endif
	}

	/************************************************************************************************/
	//fc layer function with with array input
	void fc_layer_a(
		char activation_type,
		T in_data3D[_IN_CHANNEL_NUM_][_INPUT_SIZE_][_INPUT_SIZE_],
		T kernel_weights[_IN_CHANNEL_NUM_ * _OUT_CHANNEL_NUM_][_INPUT_SIZE_][_INPUT_SIZE_],
//        T kernel_weights[_IN_CHANNEL_NUM_ * _OUT_CHANNEL_NUM_],
		T kernel_bias[_OUT_CHANNEL_NUM_],
		T out_data3D[_OUT_CHANNEL_NUM_]) {

		//cout << "starting fully_connect layer ...." << endl;

		for (uint b = 0; b < _OUT_CHANNEL_NUM_; b++) {//output kernel loop
			for (uint a = 0; a < _IN_CHANNEL_NUM_; a++) {//input kernel loop
														 //float out_data_temp[1][1] = {0};//每一个卷积计算的结果
				T out_data_temp = 0;
				fc_kernel_a(in_data3D[a],
					kernel_weights[b * _IN_CHANNEL_NUM_ + a],//weight的存放顺序跟convolution层的不同
					out_data_temp);
				out_data3D[b] += out_data_temp;
			}
			out_data3D[b] = f(activation_type, (out_data3D[b] + kernel_bias[b]));
		}

        //cout << "Finished fully_connect layer ...." << endl;

		//debugging output
#if _C_DEBUG_MODE_
		/*cout << "finished fully_connect ...." << endl;
		ofstream fc_layer_a;
		fc_layer_a.open("fc_layer_a.txt", ios::app);
		for (int i = 0; i < _OUT_CHANNEL_NUM_; i++) {
			fc_layer_a << out_data3D[i] << " ";
		}
		fc_layer_a << endl;
		fc_layer_a << endl;
		fc_layer_a.close();
		cout << endl;*/
#endif
	}

};


#endif //FFNET_FC_LAYER_H
