//This is the main function of a LeNet-5 model based application.
//Application description: LeNet-5 image recognition with std image.
//Using stb_image instead of OpenCV to eliminate library dependency.

//#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>

//#include "../fpga_cnn/hls_lib/static_vector.h"
#include "../fpga_cnn/convolution.h"
#include "../fpga_cnn/average_pooling.h"
#include "../fpga_cnn/fully_connect.h"
#include "../fpga_cnn/image_converter.h"


using namespace std;

int nn_in_data_size_conv[3] = { 32,14,5 };
int nn_out_data_size_conv[3] = { 28,10,1 };
int nn_in_number_conv[4] = { 1,6,16,120 };
int nn_channel_size_conv = 5;
int nn_channel_number_conv[4] = { 1, 6,16,120 };
bool has_connection_table[3] = { false,true,false };
int in_number_conv = 0;  // number of convolutional layer
int weight_bias_count_1 = 0;//
int weight_bias_count_2 = 0;//
int weight_count = 0;
int convolution_layer_count = 0;

int nn_in_data_size_pooling[2] = { 28,10 };
int nn_out_data_size_pooling[2] = { 14,5 };
int nn_in_number_pooling[2] = { 6,16 };
int nn_channel_number_pooling[2] = { 6,16 };
int nn_channel_size_pooling = 2;
int in_number_pooling = 0;
int nn_in_data_size_fc[1] = { 1 };
int nn_out_data_size_fc[1] = { 1 };
int nn_in_number_fc[2] = { 120,10 };
int nn_channel_size_fc = 1;
int nn_channel_number_fc[2] = { 120,10 };
int in_number_fc = 0;


std::vector<tensor_t> load_weight_conv() {
	ifstream ifs("LeNet-weights");
	string str;
	tensor_t weight_v;
	std::vector<tensor_t> weight2D;
	vec_t vec3;
	int weight_count = weight_bias_count_1
                       + nn_channel_size_conv*nn_channel_size_conv*nn_in_number_conv[in_number_conv]
                       * nn_in_number_conv[in_number_conv + 1];
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
				if (weight_v.size() == nn_channel_size_conv) {
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

vec_t load_weight_pooling() {
	ifstream ifs("LeNet-weights");
	string str;
	vec_t weight_v;//Ȩ������
				   //ǰn-1��Ȩ��&ƫ��+��n��Ȩ����
	int weight_count = weight_bias_count_1 +
            nn_channel_size_pooling*nn_channel_size_pooling*nn_in_number_pooling[in_number_pooling];
	int weight_bias_count_3_pooling = 0;
	while (ifs >> str&&weight_bias_count_3_pooling<weight_count)
	{
		if (weight_bias_count_3_pooling >= weight_bias_count_2) {
			//����n��������weight
			float f = atof(str.c_str());
			weight_v.push_back(f);//����kernelÿ�����ص�Ȩ��ֵ��������������
			weight_bias_count_1++;
		}
		weight_bias_count_3_pooling++;
	}
	cout << "weights number in total is = " << weight_bias_count_1 << endl;
	ifs.close();
	return weight_v;
}

std::vector<tensor_t> load_weight_fc() {
	ifstream ifs("LeNet-weights");
	string str;
	tensor_t weight_v;
	std::vector<tensor_t> weight2D;
	vec_t vec3;
	int weight_count = weight_bias_count_1 + nn_channel_size_fc*nn_channel_size_fc*nn_in_number_fc[in_number_fc]
                                             * nn_in_number_fc[in_number_fc + 1];
	int weight_bias_count_3_fc = 0;
	while (ifs >> str&&weight_bias_count_3_fc<weight_count)
	{
		if (weight_bias_count_3_fc >= weight_bias_count_2) {
			//����n��������weight
			if (nn_channel_size_fc == 1) {//���ȫ����kernel��СΪ1
				float f = atof(str.c_str());
				vec3.push_back(f);//����kernelÿ�����ص�Ȩ��ֵ��������������
				weight_v.push_back(vec3);//�����������������ɶ�ά����tensor
				weight2D.push_back(weight_v);
				vec3.clear();
				weight_v.clear();
			}
			else {
				if (weight_bias_count_1 == 0 || (weight_bias_count_1 - weight_bias_count_2 - 1) / nn_channel_size_fc
                                                == (weight_bias_count_1 - weight_bias_count_2) / nn_channel_size_fc) {//��ԭ����һάȨ�ذ�kernel��Сת���ɶ�ά
					float f = atof(str.c_str());
					vec3.push_back(f);//����kernelÿ�����ص�Ȩ��ֵ��������������
					if (weight_bias_count_1 == weight_count - 1) {//���һ������������
						weight_v.push_back(vec3);//�����������������ɶ�ά����tensor
						vec3.clear();
					}
					if (weight_v.size() == nn_channel_size_fc) {
						weight2D.push_back(weight_v);
						weight_v.clear();
					}
				}
				else {
					weight_v.push_back(vec3);
					vec3.clear();
					float f = atof(str.c_str());
					vec3.push_back(f);//����������������һ��Ԫ��
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

vec_t load_bias_conv() {
	ifstream ifs("LeNet-weights");
	string str;
	vec_t bias2D;
	int weight_count = weight_bias_count_2
                       + nn_channel_size_conv
                       * nn_channel_size_conv*nn_in_number_conv[in_number_conv]
                       * nn_in_number_conv[in_number_conv + 1];
	int weight_bias_count = weight_bias_count_2
                            + nn_channel_size_conv*nn_channel_size_conv*nn_in_number_conv[in_number_conv]
                            * nn_in_number_conv[in_number_conv + 1]
                            + nn_channel_number_conv[in_number_conv + 1];
	int weight_bias_count_3_conv = 0;
	while (ifs >> str&&weight_bias_count_3_conv<weight_bias_count)
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

vec_t load_bias_pooling() {
	ifstream ifs("LeNet-weights");
	string str;
	vec_t bias2D;
	int weight_count = weight_bias_count_2
                       + nn_channel_size_pooling*nn_channel_size_pooling * nn_in_number_pooling[in_number_pooling];
	int weight_bias_count = weight_bias_count_2
                            + nn_channel_size_pooling*nn_channel_size_pooling*nn_in_number_pooling[in_number_pooling]
                            + nn_channel_number_pooling[in_number_pooling];
	int weight_bias_count_3_pooling = 0;
	while (ifs >> str&&weight_bias_count_3_pooling<weight_bias_count)
	{
		if (weight_bias_count_3_pooling >= weight_bias_count_1 &&
                weight_bias_count_1 >= weight_count &&
                weight_bias_count_1 <= weight_bias_count) {
			//����n��������bias
			float f = atof(str.c_str());
			bias2D.push_back(f);//����ƫ��
			weight_bias_count_1++;
		}
		weight_bias_count_3_pooling++;
	}
	weight_bias_count_2 = weight_bias_count_1;
	cout << "weights number in total is = " << weight_bias_count_1 << endl;
	ifs.close();
	return bias2D;
}

vec_t load_bias_fc() {
	ifstream ifs("LeNet-weights");
	string str;
	vec_t bias2D;
	int weight_count = weight_bias_count_2
                       + nn_channel_size_fc*nn_channel_size_fc*nn_in_number_fc[in_number_fc]
                         * nn_in_number_fc[in_number_fc + 1];
	int weight_bias_count = weight_bias_count_2
                            + nn_channel_size_fc*nn_channel_size_fc*nn_in_number_fc[in_number_fc]
                            * nn_in_number_fc[in_number_fc + 1]
                            + nn_channel_number_fc[in_number_fc + 1];
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


int main(int argc, char** argv) {

	float c = 3;
	//network<sequential> nn;
	vec_t res;
	vec_t data_in;  //image data storage
	vec_t data_out;
	vec_t data;
	tensor_t in_data;
	tensor_t out_data;

	//convert image to data matrix
	const std::string filename = "4.bmp";
	convert_image(filename, -1.0, 1.0, 32, 32, data_in);

	//construct net and load weights to net
	//construct_net(nn);	
	//nn_load_weight("LeNet-weights", nn);

	std::vector<tensor_t> conv_1_weight2D;//�����Ȩ�ؾ���
	vec_t 		      conv_1_bias2D;//ƫ�þ���
	conv_1_weight2D = load_weight_conv();
	conv_1_bias2D = load_bias_conv();
	in_data = in_2_3D(data_in);//vector������ͼƬת��tensor

	std::vector<tensor_t> in_data2D;
	in_data2D = in_2_2D_conv(nn_in_data_size_conv[0], in_data);//inת���ɶ�ά��ʾ
	std::vector<tensor_t> conv_1_out_data;

	cout << "Finished network weights and data space preparation" << endl;
	cout << "starting convolution layer 1" << endl;

	//convolution_1
	convolution_layer_with_table(
		"tan_h",
		nn_in_data_size_conv[0],
		nn_channel_size_conv,
		in_data2D,
		has_connection_table[0],
		conv_1_weight2D,
		conv_1_bias2D,
		conv_1_out_data,
		nn_channel_number_conv[0],
		nn_channel_number_conv[1]);

	in_number_conv++;

	cout << "Finished convolution layer 1" << endl;
	cout << "Starting pooling layer 1" << endl;

	vec_t pooling_1_weight;//Ȩ�ؾ���
	vec_t pooling_1_bias2D;//ƫ�þ���
	pooling_1_weight = load_weight_pooling();
	pooling_1_bias2D = load_bias_pooling();
	std::vector<tensor_t> pooling_1_out_data;

	//pooling_1
	pooling_layer(
		"tan_h",
		nn_in_data_size_pooling[0],
		nn_channel_size_pooling,
		conv_1_out_data,
		pooling_1_weight,
		pooling_1_bias2D,
		pooling_1_out_data,
		nn_in_number_pooling[0],
		nn_channel_number_pooling[0]);

	in_number_pooling++;

	cout << "Finished pooling layer 1" << endl;
	cout << "Starting convolution layer 2" << endl;

	std::vector<tensor_t> conv_2_weight2D;
	vec_t                 conv_2_bias2D;
	conv_2_weight2D = load_weight_conv();
	conv_2_bias2D = load_bias_conv();
	std::vector<tensor_t> conv_2_out_data;

	//convolution_2
	convolution_layer_with_table(
		"tan_h",
		nn_in_data_size_conv[1],
		nn_channel_size_conv,
		pooling_1_out_data,
		has_connection_table[1],
		conv_2_weight2D,
		conv_2_bias2D,
		conv_2_out_data,
		nn_channel_number_conv[1],
		nn_channel_number_conv[2]);

	in_number_conv++;

	cout << "Finished convolution layer 2" << endl;
	cout << "Starting pooling layer 2" << endl;

	vec_t pooling_2_weight;
    vec_t pooling_2_bias2D;
	pooling_2_weight = load_weight_pooling();
	pooling_2_bias2D = load_bias_pooling();
	std::vector<tensor_t> pooling_2_out_data;

	//pooling_2
	pooling_layer(
		"tan_h",
		nn_in_data_size_pooling[1],
		nn_channel_size_pooling,
		conv_2_out_data,
		pooling_2_weight,
		pooling_2_bias2D,
		pooling_2_out_data,
		nn_in_number_pooling[1],
		nn_channel_number_pooling[1]);

	in_number_pooling++;

	cout << "Finished pooling layer 2" << endl;
	cout << "Starting convolution layer 3" << endl;

	std::vector<tensor_t> conv_3_weight2D;
	vec_t                 conv_3_bias2D;
	conv_3_weight2D = load_weight_conv();
	conv_3_bias2D = load_bias_conv();
	std::vector<tensor_t> conv_3_out_data;

	//convolution_3
	convolution_layer_with_table(
		"tan_h",
		nn_in_data_size_conv[2],
		nn_channel_size_conv,
		pooling_2_out_data,
		has_connection_table[2],
		conv_3_weight2D,
		conv_3_bias2D,
		conv_3_out_data,
		nn_channel_number_conv[2],
		nn_channel_number_conv[3]);

	in_number_conv++;

	cout << "Finished convolution layer 3" << endl;
	cout << "Starting fully connected layer 1" << endl;

	std::vector<tensor_t> fc_1_weight2D;
	vec_t                 fc_1_bias2D;
	fc_1_weight2D = load_weight_fc();
	fc_1_bias2D = load_bias_fc();
	std::vector<tensor_t> fc_1_out_data;

	//fully_connect
	fully_connected_layer(
		"tan_h",
		nn_in_data_size_fc[0],
		conv_3_out_data,
		fc_1_weight2D,
		fc_1_bias2D,
		fc_1_out_data,
		nn_channel_number_fc[0],
		nn_channel_number_fc[1]);

	in_number_fc++;

	cout << "End of network" << endl;

	return 0;
}
