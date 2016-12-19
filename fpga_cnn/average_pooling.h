#ifndef _AVERAGE_POOLING_H_
#define _AVERAGE_POOLING_H_


#pragma once
#include <iostream>
//#include "../tiny_dnn/util/util.h"
#include <fstream>
#include <algorithm>
#include "data_type.h"
#include "activation_functions.h"

using namespace std;

tensor_t pooling_kernel(int input_size,
						int kernel_size,
                        tensor_t& in_data,
                        float kernel_weights,
                        tensor_t& out_data) {
	out_data.clear();
	std::vector<tensor_t> out_1;
	std::vector<tensor_t> out_2;
	vec_t vec2;//输出的行向量
	for (int i = 0; i < input_size - kernel_size / 2; i = i + kernel_size) //遍历输入map
	{
		for (int j = 0; j < input_size - kernel_size / 2; j = j + kernel_size)
		{
			float sum = 0;
			for (int ii = 0; ii < kernel_size; ++ii) //遍历kernel
			{
				for (int jj = 0; jj < kernel_size; ++jj)
				{
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
	return out_data;
}

std::vector<tensor_t> pooling_layer(
	string activation_type,
	int& input_size,
	int& kernel_size,
	std::vector<tensor_t>& in_data3D,
	vec_t& kernel_weights,
	vec_t& kernel_bias,
	std::vector<tensor_t>& out_data3D,
	int& in_channel, int& out_channel)
{
	/*
	2d convolution function body
	in_data should be 1 32x32 data array
	out_data should be 6 28x28 data array
	please restruct the convolution function body here
	this function will be used in layer_1/layer_3/layer_5 in LeNet-5 model
	*/
	cout << "starting average_pooling ...." << endl;
	out_data3D.clear();
	tensor_t out_data2D;
	float out_data2D_final_f;
	vec_t out_data2D_final_v;
	tensor_t out_data2D_final;

	for (int a = 0; a < in_channel; a++) {//6个in
		out_data2D = pooling_kernel(input_size, kernel_size, in_data3D[a], kernel_weights[a], out_data2D);
		//循环遍历out_data2D矩阵加偏置和激活
		for (int i = 0; i < out_data2D.size(); i++) {
			for (int j = 0; j < out_data2D[i].size(); j++) {
				out_data2D_final_f = out_data2D[i][j] + kernel_bias[a];
				//const float ep = exp(out_data2D_final_f);
				//const float em = exp(-out_data2D_final_f);
				//out_data2D_final_f = (ep - em) / (ep + em);//tan_h????

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

	//debugging output
	cout << "finished average_pooling ...." << endl;
//	FILE *fp = NULL;
//	for (int i = 0; i < out_data3D.size(); i++) {
//		for (int j = 0; j < out_data3D[i].size(); j++) {
//			for (int k = 0; k < out_data3D[i][j].size(); k++) {
//				fp = freopen("out_pool.txt", "a+", stdout);
//				cout << out_data3D[i][j][k] << " ";
//			}
//			cout << endl;
//		}
//	}
//	fclose(fp);
//	cout << endl;
//	//getchar();
	return out_data3D;
}

#endif
