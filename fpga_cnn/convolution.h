#ifndef _CONVOLUTION_H_
#define _CONVOLUTION_H_

#include <iostream>
#include <fstream>
#include <algorithm>
#include "data_type.h"
#include "activation_functions.h"

using namespace std;

#define O true
#define X false
static const bool tbl[6][16] = {
	O, X, X, X, O, O, O, X, X, O, O, O, O, X, O, O,
	O, O, X, X, X, O, O, O, X, X, O, O, O, O, X, O,
	O, O, O, X, X, X, O, O, O, X, X, O, X, O, O, O,
	X, O, O, O, X, X, O, O, O, O, X, X, O, X, O, O,
	X, X, O, O, O, X, X, O, O, O, O, X, O, O, X, O,
	X, X, X, O, O, O, X, X, O, O, O, O, X, O, O, O
};
#undef O
#undef X

//convolution kernel function
tensor_t convolution_kernel(int input_size,
	int kernel_size,
	tensor_t& in_data,
	tensor_t& kernel_weights,
	tensor_t& out_data) {
	out_data.clear();
	vec_t vec2;//output row vector
	for (int i = kernel_size / 2; i < input_size - kernel_size / 2; ++i)
	{
		for (int j = kernel_size / 2; j < input_size - kernel_size / 2; ++j)
		{
			float sum = 0;
			for (int ii = -kernel_size / 2; ii <= kernel_size / 2; ++ii)
			{
				for (int jj = -kernel_size / 2; jj <= kernel_size / 2; ++jj)
				{
					float data = in_data[i + ii][j + jj];
					float weight = kernel_weights[ii + kernel_size / 2][jj + kernel_size / 2];
					sum += data * weight;
				}
			}
			vec2.push_back(sum);
		}
		out_data.push_back(vec2);
		vec2.clear();
	}
	return out_data;
}

//tensor to tensor convolutional layer with connection table
void convolution_layer_with_table(
	string activation_type,
	int input_size,
	int kernel_size,
	std::vector<tensor_t>& in_data3D,
	bool has_connection_table,
	std::vector<tensor_t>& kernel_weights,
	vec_t& kernel_bias,
	std::vector<tensor_t>& out_data3D,
	int in_channel,
	int out_channel
/*const bool* tbl*/)
{
	/*
	2d convolution function body
	in_data should be 1 32x32 data array
	out_data should be 6 28x28 data array
	please restruct the convolution function body here
	this function will be used in layer_1/layer_3/layer_5 in LeNet-5 model
	*/
	cout << "starting convolution ...." << endl;
	out_data3D.clear();

	tensor_t out_data2D_plus;//每一个滤波器filter中所有卷积核卷积计算结果的累加结果
	float out_data2D_final_f;//每个像素加偏置、激活后的值
	vec_t out_data2D_final_v;//每个像素加偏置、激活后的值组成的行向量
	tensor_t out_data2D_final;//最终的输出矩阵

	for (int b = 0; b < out_channel; b++) {//output kernel loop
		int connection_num = 0;//每个in对于每个kernel是第几个连接
		for (int a = 0; a < in_channel; a++) {//input kernel loop
			if (has_connection_table) {//如果有连接表
				if (tbl[a][b]) {//如果是连接的
					tensor_t out_data2D;//每一个卷积计算的结果
					out_data2D = convolution_kernel(input_size,
						kernel_size,
						in_data3D[a],
						kernel_weights[b*in_channel + a],
						out_data2D);
					for (int i = 0; i < out_data2D.size(); i++) {
						vector<float> result_1;
						if (connection_num == 0) {//第一个连接
							for (int j = 0; j < out_data2D[i].size(); j++) {
								result_1.push_back(0);//行向量的累加和初始置0
							}
						}
						else if (connection_num != 0) {
							for (int j = 0; j < out_data2D[i].size(); j++) {
								result_1.push_back(out_data2D_plus[i][j]);//行向量的累加和
							}
						}
						transform(result_1.begin(),
							result_1.end(),
							out_data2D[i].begin(),
							result_1.begin(),
							plus<float>());//行向量累加
						out_data2D_plus.push_back(result_1);//把每个行向量累加结果放入out_data2D_plus，每次放入会增加10行
					}
					//将行向量累加放入out_data2D_plus中后，删除前10个行向量
					if (connection_num != 0) {
						tensor_t::iterator it;
						//vector<string>::iterator subIt = (*it).begin();
						for (int i = 0; i < out_data2D.size(); i++)//把累加和tensor的前10行删除，剩下的10行则为每次累加的中间结果
						{
							it = out_data2D_plus.begin();
							out_data2D_plus.erase(it);
							//it++;//这里迭代器更新
						}
					}
					connection_num++;
				}
			}
			else if (!has_connection_table) {//如果没有连接表
				tensor_t out_data2D;
				out_data2D = convolution_kernel(input_size,
					kernel_size,
					in_data3D[a],
					kernel_weights[b*in_channel + a],
					out_data2D);
				for (int i = 0; i < out_data2D.size(); i++) {
					vector<float> result_1;
					if (connection_num == 0) {
						for (int j = 0; j < out_data2D[i].size(); j++) {
							result_1.push_back(0);
						}
					}
					else if (connection_num != 0) {
						for (int j = 0; j < out_data2D[i].size(); j++) {
							result_1.push_back(out_data2D_plus[i][j]);
						}
					}
					transform(result_1.begin(),
						result_1.end(),
						out_data2D[i].begin(),
						result_1.begin(),
						plus<float>());
					out_data2D_plus.push_back(result_1);
				}
				if (connection_num != 0) {
					tensor_t::iterator it;
					//vector<string>::iterator subIt = (*it).begin();
					for (int i = 0; i < out_data2D.size(); i++)
					{
						it = out_data2D_plus.begin();
						out_data2D_plus.erase(it);
						//it++;
					}
				}
				connection_num++;
			}
		}
		//循环遍历out_data2D_plus矩阵加偏置和激活
		for (int i = 0; i < out_data2D_plus.size(); i++) {
			for (int j = 0; j < out_data2D_plus[i].size(); j++) {
				out_data2D_final_f = out_data2D_plus[i][j] + kernel_bias[b];
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
		//out_data2D.clear();
		out_data2D_final.clear();
		out_data2D_plus.clear();
	}

	//debugging output
	cout << "finished convolution ...." << endl;
	FILE *fp = NULL;
	for (int i = 0; i < out_data3D.size(); i++) {
		for (int j = 0; j < out_data3D[i].size(); j++) {
			for (int k = 0; k < out_data3D[i][j].size(); k++) {
				fp = freopen("out_conv.txt", "a+", stdout);
				cout << out_data3D[i][j][k] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
	fclose(fp);
	cout << endl;
}

//general convolution layer without connection table
// 3D tensor to 3D tensor convertion

#endif
