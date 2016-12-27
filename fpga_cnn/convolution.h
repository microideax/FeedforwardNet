//General convolution function in CNNs with the connection table as channel connection input.

#ifndef _CONVOLUTION_H_
#define _CONVOLUTION_H_

#include <iostream>
#include <fstream>
#include <algorithm>
#include "data_type.h"
#include "activation_functions.h"
#include "config.h"

using namespace std;

extern const bool tbl[6][16];
//convolution kernel function
void convolution_kernel(int input_size,
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
}

//tensor to tensor convolution layer with connection table
void convolution_layer_with_table(
	char activation_type,
	int input_size,
	int kernel_size,
	tensor_t_3d& in_data3D,
//    T& in_data3D,
	bool has_connection_table,
	tensor_t_3d& kernel_weights,
	vec_t& kernel_bias,
	tensor_t_3d& out_data3D,
	int in_channel,
	int out_channel) {

	cout << "starting convolution ...." << endl;
	out_data3D.clear();

	tensor_t out_data2D_plus;
	float out_data2D_final_f;
	vec_t out_data2D_final_v;
	tensor_t out_data2D_final;

	for (int b = 0; b < out_channel; b++) {//output kernel loop
		int connection_num = 0;
		for (int a = 0; a < in_channel; a++) {//input kernel loop
			if (has_connection_table) {
				if (tbl[a][b]) {
					tensor_t out_data2D;
					convolution_kernel(input_size,
						               kernel_size,
						               in_data3D[a],
                                       kernel_weights[b*in_channel + a],
						               out_data2D);
					for (int i = 0; i < out_data2D.size(); i++) {
//						vector<float> result_1;
                        vec_t result_1;
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
                        for (int r = 0; r < result_1.size(); r++){
                            result_1[r] = result_1[r] + out_data2D[i][r];
                        }
                        out_data2D_plus.push_back(result_1);
					}
					if (connection_num != 0) {
                        //static vectors
						for (int i = 0; i < out_data2D.size(); i++) {
							out_data2D_plus.erase(0);
						}
					}
					connection_num++;
				}
			}
			else if (!has_connection_table) {
				tensor_t out_data2D;
				convolution_kernel(input_size,
					kernel_size,
					in_data3D[a],
					kernel_weights[b*in_channel + a],
					out_data2D);
				for (int i = 0; i < out_data2D.size(); i++) {
//					vector<float> result_1;
                    vec_t result_1;
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
                    for (int r = 0; r < result_1.size(); r++){
                        result_1[r] = result_1[r] + out_data2D[i][r];
                    }
					out_data2D_plus.push_back(result_1);
				}
				if (connection_num != 0) {
					for (int i = 0; i < out_data2D.size(); i++) {
						out_data2D_plus.erase(0);
					}
				}
				connection_num++;
			}
		}

		for (int i = 0; i < out_data2D_plus.size(); i++) {
			for (int j = 0; j < out_data2D_plus[i].size(); j++) {
				out_data2D_final_f = out_data2D_plus[i][j] + kernel_bias[b];

				out_data2D_final_f = f(activation_type, out_data2D_final_f);//

				out_data2D_final_v.push_back(out_data2D_final_f);//
			}
			out_data2D_final.push_back(out_data2D_final_v);//
			out_data2D_final_v.clear();
		}
		out_data3D.push_back(out_data2D_final);
		out_data2D_final.clear();
		out_data2D_plus.clear();
	}


	//debugging output
#if _C_DEBUG_MODE_
	cout << "finished convolution ...." << endl;
	ofstream out_conv;
    out_conv.open("out_conv.txt", ios::app);
	for (int i = 0; i < out_data3D.size(); i++) {
		for (int j = 0; j < out_data3D[i].size(); j++) {
			for (int k = 0; k < out_data3D[i][j].size(); k++) {
				out_conv << out_data3D[i][j][k] << " ";
			}
			out_conv << endl;
		}
		out_conv << endl;
	}
	out_conv.close();
	cout << endl;
#endif

}

//general convolution layer without connection table
// 3D tensor to 3D tensor convertion
//TODO: Add convolution function without connection table.


#endif