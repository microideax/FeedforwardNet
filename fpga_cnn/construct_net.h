//Network construction function, will be modified after all functional blocks are tested.
// Created by yaochen on 19/12/16.
//

#ifndef _CONSTRUCT_NET_H
#define _CONSTRUCT_NET_H

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "data_type.h"

/*
//void construct_net(network<sequential>& nn) {
void construct_net(network<sequential>& nn) {
// connection table [Y.Lecun, 1998 Table.1]
#define O true
#define X false
static const bool tbl[] = {
O, X, X, X, O, O, O, X, X, O, O, O, O, X, O, O,
O, O, X, X, X, O, O, O, X, X, O, O, O, O, X, O,
O, O, O, X, X, X, O, O, O, X, X, O, X, O, O, O,
X, O, O, O, X, X, O, O, O, O, X, X, O, X, O, O,
X, X, O, O, O, X, X, O, O, O, O, X, O, O, X, O,
X, X, X, O, O, O, X, X, O, O, O, O, X, O, O, O
};
#undef O
#undef X

// construct nets
nn << convolutional_layer<tan_h>(32, 32, 5, 1, 6)  // C1, 1@32x32-in, 6@28x28-out
<< average_pooling_layer<tan_h>(28, 28, 6, 2)   // S2, 6@28x28-in, 6@14x14-out
<< convolutional_layer<tan_h>(14, 14, 5, 6, 16,
connection_table(tbl, 6, 16))              // C3, 6@14x14-in, 16@10x10-out
<< average_pooling_layer<tan_h>(10, 10, 16, 2)  // S4, 16@10x10-in, 16@5x5-out
<< convolutional_layer<tan_h>(5, 5, 5, 16, 120) // C5, 16@5x5-in, 120@1x1-out
<< fully_connected_layer<tan_h>(120, 10);       // F6, 120-in, 10-out
}

*/


tensor_t_3d load_weight_conv(int& weight_bias_count_1,
	int& weight_bias_count_2,
	int nn_channel_size_conv,
	int nn_in_number_conv[],
	int nn_channel_number_conv[],
	int in_number_conv) {
	ifstream ifs("LeNet-weights");
	string str;
	tensor_t weight_v;
	tensor_t_3d weight2D;
	vec_t vec3;
	int weight_count = weight_bias_count_1 + nn_channel_size_conv*nn_channel_size_conv*nn_in_number_conv[in_number_conv]
		* nn_channel_number_conv[in_number_conv];
	int weight_bias_count_3_conv = 0;
	while (ifs >> str && weight_bias_count_3_conv < weight_count)
	{
		if (weight_bias_count_3_conv >= weight_bias_count_2) {
			if (weight_bias_count_1 == 0 || (weight_bias_count_1 - weight_bias_count_2 - 1) / nn_channel_size_conv
				== (weight_bias_count_1 - weight_bias_count_2) / nn_channel_size_conv) {//��ԭ����һάȨ�ذ�kernel��Сת���ɶ�ά
				float f = atof(str.c_str());
				vec3.push_back(f);
				if (weight_bias_count_1 == weight_count - 1) {
					weight_v.push_back(vec3);
					vec3.clear();
				}
				if (weight_v.size() == int(nn_channel_size_conv)) {
					weight2D.push_back(weight_v);
					weight_v.clear();
				}
			}
			else {
				weight_v.push_back(vec3);
				vec3.clear();
				float f = atof(str.c_str());
				vec3.push_back(f);
			}
			weight_bias_count_1++;
		}
		weight_bias_count_3_conv++;
	}
	cout << "conv weights number in total is = " << weight_bias_count_1 << endl;
	ifs.close();
	return weight2D;
}

vec_t load_weight_pooling(int& weight_bias_count_1,
	int& weight_bias_count_2,
	int nn_channel_size_pooling,
	int nn_in_number_pooling[],
	int in_number_pooling) {
	ifstream ifs("LeNet-weights");
	string str;
	vec_t weight_v;//
	int weight_count = weight_bias_count_1 +
		nn_channel_size_pooling*nn_channel_size_pooling*nn_in_number_pooling[in_number_pooling];
	int weight_bias_count_3_pooling = 0;
	while (ifs >> str&&weight_bias_count_3_pooling<weight_count)
	{
		if (weight_bias_count_3_pooling >= weight_bias_count_2) {
			//
			float f = atof(str.c_str());
			weight_v.push_back(f);
			weight_bias_count_1++;
		}
		weight_bias_count_3_pooling++;
	}
	cout << "weights number in total is = " << weight_bias_count_1 << endl;
	ifs.close();
	return weight_v;
}

tensor_t_3d load_weight_fc(int& weight_bias_count_1,
	int& weight_bias_count_2,
	int nn_channel_size_fc,
	int nn_in_number_fc[],
	int nn_channel_number_fc[],
	int in_number_fc) {
	ifstream ifs("LeNet-weights");
	string str;
	tensor_t weight_v;
	tensor_t_3d weight2D;
	vec_t vec3;
	int weight_count = weight_bias_count_1 + nn_channel_size_fc*nn_channel_size_fc*nn_in_number_fc[in_number_fc]
		* nn_channel_number_fc[in_number_fc];
	int weight_bias_count_3_fc = 0;
	while (ifs >> str && weight_bias_count_3_fc < weight_count)
	{
		if (weight_bias_count_3_fc >= weight_bias_count_2) {
			//
			if (nn_channel_size_fc == 1) {
				float f = atof(str.c_str());
				vec3.push_back(f);
				weight_v.push_back(vec3);
				weight2D.push_back(weight_v);
				vec3.clear();
				weight_v.clear();
			}
			else {
				if (weight_bias_count_1 == 0 || (weight_bias_count_1 - weight_bias_count_2 - 1) / nn_channel_size_fc
					== (weight_bias_count_1 - weight_bias_count_2) / nn_channel_size_fc) {
					float f = atof(str.c_str());
					vec3.push_back(f);
					if (weight_bias_count_1 == weight_count - 1) {
						weight_v.push_back(vec3);
						vec3.clear();
					}
					if (weight_v.size() == int(nn_channel_size_fc)) {
						weight2D.push_back(weight_v);
						weight_v.clear();
					}
				}
				else {
					weight_v.push_back(vec3);
					vec3.clear();
					float f = atof(str.c_str());
					vec3.push_back(f);
				}
			}
			weight_bias_count_1++;
		}
		weight_bias_count_3_fc++;
	}
	cout << "conv weights number in total is = " << weight_bias_count_1 << endl;
	ifs.close();
	return weight2D;
}

vec_t load_bias_conv(int& weight_bias_count_1,
	int& weight_bias_count_2,
	int nn_channel_size_conv,
	int nn_in_number_conv[],
	int nn_channel_number_conv[],
	int in_number_conv) {
	ifstream ifs("LeNet-weights");
	string str;
	vec_t bias2D;
	int weight_count = weight_bias_count_2
		+ nn_channel_size_conv
		* nn_channel_size_conv*nn_in_number_conv[in_number_conv]
		* nn_channel_number_conv[in_number_conv];
	int weight_bias_count = weight_count + nn_channel_number_conv[in_number_conv];
	int weight_bias_count_3_conv = 0;
	while (ifs >> str && weight_bias_count_3_conv < weight_bias_count)
	{
		if (weight_bias_count_3_conv >= weight_bias_count_1 && weight_bias_count_1 >= weight_count
			&& weight_bias_count_1 <= weight_bias_count) {
			//����n��������bias
			float f = atof(str.c_str());
			bias2D.push_back(f);//����ƫ��
			weight_bias_count_1++;
		}
		weight_bias_count_3_conv++;
	}
	weight_bias_count_2 = weight_bias_count_1;
	cout << "bias weights number in total is = " << weight_bias_count_1 << endl;
	ifs.close();
	return bias2D;
}

vec_t load_bias_pooling(int& weight_bias_count_1,
	int& weight_bias_count_2,
	int nn_channel_size_pooling,
	int nn_in_number_pooling[],
	int nn_channel_number_pooling[],
	int in_number_pooling) {
	ifstream ifs("LeNet-weights");
	string str;
	vec_t bias2D;
	int weight_count = weight_bias_count_2
		+ nn_channel_size_pooling*nn_channel_size_pooling * nn_in_number_pooling[in_number_pooling];
	int weight_bias_count = weight_count + nn_channel_number_pooling[in_number_pooling];
	int weight_bias_count_3_pooling = 0;
	while (ifs >> str&&weight_bias_count_3_pooling<weight_bias_count)
	{
		if (weight_bias_count_3_pooling >= weight_bias_count_1 &&
			weight_bias_count_1 >= weight_count &&
			weight_bias_count_1 <= weight_bias_count) {
			//
			float f = atof(str.c_str());
			bias2D.push_back(f);//
			weight_bias_count_1++;
		}
		weight_bias_count_3_pooling++;
	}
	weight_bias_count_2 = weight_bias_count_1;
	cout << "weights number in total is = " << weight_bias_count_1 << endl;
	ifs.close();
	return bias2D;
}

vec_t load_bias_fc(int& weight_bias_count_1,
	int& weight_bias_count_2,
	int nn_channel_size_fc,
	int nn_in_number_fc[],
	int nn_channel_number_fc[],
	int in_number_fc) {
	ifstream ifs("LeNet-weights");
	string str;
	vec_t bias2D;
	int weight_count = weight_bias_count_2
		+ nn_channel_size_fc*nn_channel_size_fc*nn_in_number_fc[in_number_fc]
		* nn_channel_number_fc[in_number_fc];
	int weight_bias_count = weight_count + nn_channel_number_fc[in_number_fc];
	int weight_bias_count_3_fc = 0;
	while (ifs >> str&&weight_bias_count_3_fc<weight_bias_count)
	{
		if (weight_bias_count_3_fc >= weight_bias_count_1 &&
			weight_bias_count_1 >= weight_count &&
			weight_bias_count_1 <= weight_bias_count) {
			float f = atof(str.c_str());
			bias2D.push_back(f);
			weight_bias_count_1++;
		}
		weight_bias_count_3_fc++;
	}
	weight_bias_count_2 = weight_bias_count_1;
	cout << "bias weights number in total is = " << weight_bias_count_1 << endl;
	ifs.close();
	return bias2D;
}

#endif //FFNET_CONSTRUCT_NET_H
