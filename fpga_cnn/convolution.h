#ifndef _CONVOLUTION_H_
#define _CONVOLUTION_H_

#include <iostream>
#include "../tiny_dnn/util/util.h"
#include <fstream>
#include <algorithm>
using namespace tiny_dnn;
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
			float kernel_bias, 
			tensor_t& out_data, 
			int in_channel, 
			int out_channel) {
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
	int input_size,
	int kernel_size,
	std::vector<tensor_t>& in_data,
	bool has_connection_table,
	std::vector<tensor_t>& kernel_weights,
	vec_t& kernel_bias,
	std::vector<tensor_t>& out_data,
	int in_channel, 
	int out_channel,
	const bool& tbl)
{
	/*
	2d convolution function body
	in_data should be 1 32x32 data array
	out_data should be 6 28x28 data array
	please restruct the convolution function body here
	this function will be used in layer_1/layer_3/layer_5 in LeNet-5 model
	*/
	cout << "starting convolution ...." << endl;
	out_data.clear();
	tensor_t out_data2D;//ÿһ???????????Ľ???
	tensor_t out_data2D_plus;//ÿһ???˲???filter?????о????˾??????????????ۼӽ???
	float out_data2D_final_f;//ÿ?????ؼ?ƫ?á?????????ֵ
	vec_t out_data2D_final_v;//ÿ?????ؼ?ƫ?á?????????ֵ???ɵ?????��
	tensor_t out_data2D_final;//???յ?????????

	for (int b = 0; b < out_channel; b++) {//16??kernel
		int connection_num = 0;//ÿ??in????ÿ??kernel?ǵڼ???��??
		for (int a = 0; a < in_channel; a++) {//6??in
			if (has_connection_table) {//??????��?ӱ?
				if (tbl[a][b]) {//??????��?ӵ?
					out_data2D = convolution_kernel(input_size, 
									kernel_size, 
									in_data[a], 
									kernel_weights[b*in_channel + a], 
									kernel_bias[b], 
									in_data[a], 
									in_channel, 
									out_channel);
					for (int i = 0; i < out_data2D.size(); i++) {
						vector<float> result_1;
						if (connection_num == 0) {//??һ??��??
							for (int j = 0; j < out_data2D[i].size(); j++) {
								result_1.push_back(0);//????��???ۼӺͳ?ʼ??0
							}
						}
						else if (connection_num != 0) {
							for (int j = 0; j < out_data2D[i].size(); j++) {
								result_1.push_back(out_data2D_plus[i][j]);//????��???ۼӺ?
							}
						}
						transform(result_1.begin(), 
								  result_1.end(), 
								  out_data2D[i].begin(), 
								  result_1.begin(), 
								  plus<float>());//????��?ۼ?
						out_data2D_plus.push_back(result_1);//??ÿ??????��?ۼӽ???????out_data2D_plus??ÿ?η?????????10??
					}
					if (connection_num != 0) {
						tensor_t::iterator it;
						//vector<string>::iterator subIt = (*it).begin();
						for (int i = 0; i < out_data2D.size(); i++)//???ۼӺ?tensor??ǰ10??ɾ????ʣ?µ?10????Ϊÿ???ۼӵ??м?????
						{
							it = out_data2D_plus.begin();
							out_data2D_plus.erase(it);
							//it++;//??????????????
						}
					}
					connection_num++;
				}
			}
			else if (!has_connection_table) {//????û??��?ӱ?
				out_data2D = convolution_kernel(input_size, 
								kernel_size, 
								in_data[a], 
								kernel_weights[b*in_channel + a], 
								kernel_bias[b], 
								in_data[a], 
								in_channel, 
								out_channel);
				for (int i = 0; i < out_data2D.size(); i++) {
					vector<float> result_1;
					if (connection_num == 0) {//??һ??��??
						for (int j = 0; j < out_data2D[i].size(); j++) {
							result_1.push_back(0);//????��???ۼӺͳ?ʼ??0
						}
					}
					else if (connection_num != 0) {
						for (int j = 0; j < out_data2D[i].size(); j++) {
							result_1.push_back(out_data2D_plus[i][j]);//????��???ۼӺ?
						}
					}
					transform(result_1.begin(), 
							  result_1.end(), 
							  out_data2D[i].begin(), 
							  result_1.begin(), 
							  plus<float>());//????��?ۼ?
					out_data2D_plus.push_back(result_1);//??ÿ??????��?ۼӽ???????out_data2D_plus??ÿ?η?????????10??,??20??
				}
				//??????��?ۼӷ???out_data2D_plus?к???ɾ??ǰ10??????��
				if (connection_num != 0) {
					tensor_t::iterator it;
					//vector<string>::iterator subIt = (*it).begin();
					for (int i = 0; i < out_data2D.size(); i++)//???ۼӺ?out_data2D_plus??ǰ10??ɾ????ʣ?µ?10????Ϊÿ???ۼӵ??м?????
					{
						it = out_data2D_plus.begin();
						out_data2D_plus.erase(it);
						//it++;//??????????????
					}
				}
				connection_num++;
			}
		}
		//ѭ??????out_data2D_plus??????ƫ?úͼ???
		for (int i = 0; i < out_data2D.size(); i++) {
			for (int j = 0; j < out_data2D[i].size(); j++) {
				out_data2D_final_f = out_data2D_plus[i][j] + kernel_bias[b];
				const float ep = exp(out_data2D_final_f);
				const float em = exp(-out_data2D_final_f);
				out_data2D_final_f = (ep - em) / (ep + em);//tan_h????
				out_data2D_final_v.push_back(out_data2D_final_f);//????????ÿ??????????ֵ
			}
			out_data2D_final.push_back(out_data2D_final_v);//??ÿ??????��?ۼӽ???????out_data2D_final
			out_data2D_final_v.clear();
		}
		out_data.push_back(out_data2D_final);
		out_data2D.clear();
		out_data2D_final.clear();
		out_data2D_plus.clear();
	}
	cout << "finished convolution ...." << endl;
	for (int i = 0; i < out_data.size(); i++) {
		for (int j = 0; j < out_data[i].size(); j++) {//?鿴??һ??????map
			for (int k = 0; k < out_data[i][j].size(); k++) {
				freopen("out_conv.txt", "a+", stdout);
				cout << out_data[i][j][k] << " ";
			}
			cout << endl;
		}
	}
	cout << endl;
	// return out_data;
}

//general convolution layer without connection table
// void convolution_layer(
// 	int& input_size,
// 	int& kernel_size,
// 	std::vector<tensor_t>& in_data2D,
// 	std::vector<tensor_t>& kernel_weights,
// 	vec_t& kernel_bias,
// 	std::vector<tensor_t>& out_data3D,
// 	int& in_channel, 
// 	int& out_channel) {
	
// 	cout << "starting convolution ...." << endl;

// 	tensor_t out_data;

// 	for (int b = 0; b < out_channel; b++) {//6??kernel
// 		for (int a = 0; a < in_channel; a++) {//1??in
// 			//tensor_t out_data;
// 			convolution_kernel(input_size, 
// 			   		      kernel_size, 
// 					      in_data2D[a], 
// 					      kernel_weights[b], 
// 					      kernel_bias[b], 
// 					      out_data, 
// 					      in_channel, 
// 					      out_channel);
// 			out_data3D.push_back(out_data); ////6*16??
// 		}
// 	}
// }

#endif
