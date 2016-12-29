//average pooling layer with kernel weights and without kernel weights
//TODO: Add average/max pooling without kernel weights functions.

#ifndef _AVERAGE_POOLING_H_
#define _AVERAGE_POOLING_H_

//#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include "data_type.h"
#include "activation_functions.h"
#include "config.h"

using namespace std;

void pooling_kernel(int input_size,
                    int kernel_size,
                    tensor_t& in_data,
                    float kernel_weights,
                    tensor_t& out_data) {
    out_data.clear();
	vec_t vec2;//output row vector
	for (int i = 0; i < input_size - kernel_size / 2; i = i + kernel_size) //遍历输入map
	{
		for (int j = 0; j < input_size - kernel_size / 2; j = j + kernel_size)
		{
#pragma HLS PIPELINE
			float sum = 0;
			for (int ii = 0; ii < kernel_size; ++ii) //遍历kernel
			{
#pragma HLS UNROLL
				for (int jj = 0; jj < kernel_size; ++jj)
				{
#pragma HLS UNROLL
					float data = in_data[i + ii][j + jj];
					sum += data;
				}
			}
			sum = (float)(sum / (kernel_size*kernel_size));//求出每个pooling窗口内的均值
			sum = sum*kernel_weights;//每个输入乘同一个weight
									 //sum += kernel_bias;
			vec2.push_back(sum);//放入sum构成输出行向量
		}
		out_data.push_back(vec2);//放入输出行向量构成输出map
		vec2.clear();
	}
}

void pooling_layer(
	char activation_type,
	int input_size,
	int kernel_size,
	tensor_t_3d& in_data3D,
	vec_t& kernel_weights,
	vec_t& kernel_bias,
	tensor_t_3d& out_data3D,
	int& in_channel) {

	cout << "Starting average_pooling ...." << endl;
	out_data3D.clear();

	tensor_t out_data2D;
	float out_data2D_final_f;
	vec_t out_data2D_final_v;
	tensor_t out_data2D_final;

	for (int a = 0; a < in_channel; a++) {//input kernel loop
		pooling_kernel(input_size, kernel_size, in_data3D[a], kernel_weights[a], out_data2D);
		//循环遍历out_data2D矩阵加偏置和激活
		for (int i = 0; i < out_data2D.size(); i++) {
			for (int j = 0; j < out_data2D[i].size(); j++) {
				out_data2D_final_f = out_data2D[i][j] + kernel_bias[a];

				out_data2D_final_f = f(activation_type, out_data2D_final_f);//激活函数激活

				out_data2D_final_v.push_back(out_data2D_final_f);//放入每个像素最终输出值
			}
			out_data2D_final.push_back(out_data2D_final_v);//把每个行向量累加结果放入out_data2D_final
			out_data2D_final_v.clear();
		}
		out_data3D.push_back(out_data2D_final);
		out_data2D.clear();
		out_data2D_final.clear();
	}
	cout << "Finished average_pooling ...." << endl;

    //debugging output
#if _C_DEBUG_MODE_
    ofstream out_pool;
    out_pool.open("out_pool.txt", ios::app);
	for (int i = 0; i < out_data3D.size(); i++) {
		for (int j = 0; j < out_data3D[i].size(); j++) {
			for (int k = 0; k < out_data3D[i][j].size(); k++) {
				out_pool << out_data3D[i][j][k] << " ";
			}
			out_pool << endl;
		}
        out_pool << endl;
	}
	out_pool.close();
	cout << endl;
#endif

}
#endif