//Network construction function, will be modified after all functional blocks are tested.
// Created by yaochen on 19/12/16.
//TODO: change all functions to store data in array, array dimensions are defined by network model
//

#ifndef _WEIGHT_BIAS_H_
#define _WEIGHT_BIAS_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "data_type.h"


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

vec_t load_weight_pooling(
        int& weight_bias_count_1,
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

#endif //
