#pragma once
#include <iostream>
//#include "../tiny_dnn/util/util.h"
#include <fstream>
#include <algorithm>
using namespace tiny_dnn;
using namespace std;

tensor_t pooling_kernel(int input_size, int kernel_size, tensor_t in_data, float kernel_weights, float kernel_bias, tensor_t out_data, int& in_channel, int& out_channel) {
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
			sum = (float)(sum/ (kernel_size*kernel_size));//求出每个pooling窗口内的均值
			sum = sum*kernel_weights;//每个输入乘同一个weight
			sum += kernel_bias;
			const float ep = exp(sum);
			const float em = exp(-sum);
			sum = (ep - em) / (ep + em);//tan_h激活
			vec2.push_back(sum);//放入sum构成输出行向量
		}
		out_data.push_back(vec2);//放入输出行向量构成输出map
		vec2.clear();
	}
	return out_data;
}

std::vector<tensor_t> pooling_layer(
	int& input_size,
	int& kernel_size,
	std::vector<tensor_t> in_data,
	vec_t kernel_weights,
	vec_t kernel_bias,
	std::vector<tensor_t> out_data,
	int& in_channel, int& out_channel)
{
	/*
	2d convolution function body
	in_data should be 1 32x32 data array
	out_data should be 6 28x28 data array
	please restruct the convolution function body here
	this function will be used in layer_1/layer_3/layer_5 in LeNet-5 model
	*/
	out_data.clear();
	tensor_t out_data3D;
		for (int a = 0; a < in_channel; a++) {//6个in
			out_data3D = pooling_kernel(input_size, kernel_size, in_data[a], kernel_weights[a], kernel_bias[a], in_data[a], in_channel, out_channel);
			out_data.push_back(out_data3D);//
			out_data3D.clear();
		}
	for (int i = 0; i < out_data.size(); i++) {
		for (int j = 0; j < out_data[i].size(); j++) {//查看第一个输出map
			for (int k = 0; k < out_data[i][j].size(); k++) {
				freopen("out_pool.txt", "a+", stdout);
				cout << out_data[i][j][k] << " ";
			}
			cout << endl;
		}
	}
	cout << endl;
	//getchar();
	return out_data;
}
