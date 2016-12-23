//This is the main function of a LeNet-5 model based application.
//Application description: LeNet-5 image recognition with std image.
//Using stb_image instead of OpenCV to eliminate library dependency.

//#include "stdafx.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include "../fpga_cnn/convolution.h"
#include "../fpga_cnn/average_pooling.h"
#include "../fpga_cnn/fully_connect.h"
#include "../fpga_cnn/image_converter.h"
#include "../fpga_cnn/construct_net.h"

using namespace std;

int nn_in_data_size_conv[3] = { 32,14,5 };
int nn_in_number_conv[3] = { 1,6,16 };
int nn_channel_size_conv = 5;
int nn_channel_number_conv[3] = { 6,16,120 };
bool has_connection_table[3] = { false, true, false };
int in_number_conv = 0;  // number of convolutional layer

int nn_in_data_size_pooling[2] = { 28,10 };
int nn_in_number_pooling[2] = { 6,16 };
int nn_channel_number_pooling[2] = { 6,16 };
int nn_channel_size_pooling = 2;
int in_number_pooling = 0;// number of average_pooling layer

int nn_in_data_size_fc[1] = { 1 };
int nn_in_number_fc[1] = { 120 };
int nn_channel_size_fc = 1;
int nn_channel_number_fc[1] = { 10 };
int in_number_fc = 0;// number of fully_connected layer

int weight_bias_count_1 = 0;
int weight_bias_count_2 = 0;

int main(int argc, char** argv) {

	std_vec_t data_in;  //image data storage
	std_tensor_t in_data_temp;
	tensor_t in_data;

	//convert image to data matrix
	const std::string filename = "4.bmp";
	convert_image(filename, -1.0, 1.0, 32, 32, data_in);
	in_data_temp = in_2_3D(data_in);

	tensor_t_3d in_data2D;
	std_tensor_t_3d in_data2D_temp;
	in_data2D_temp = in_2_2D_conv(nn_in_data_size_conv[0], in_data_temp);//
	tensor_t_3d conv_1_out_data;

	cout << "Finished network weights and data space preparation" << endl;
	cout << endl;
	cout << "starting convolution layer 1" << endl;

	char tan_h = 't';

	std_2_static(in_data2D, in_data2D_temp);

	ofstream indata;
	indata.open("in_data.txt", ios::app);
	for (int i = 0; i < in_data2D.size(); i++) {
		for (int j = 0; j < in_data2D[i].size(); j++) {
			for (int k = 0; k < in_data2D[i][j].size(); k++) {
				indata << in_data2D[i][j][k] << " ";
			}
			indata << endl;
		}
		indata << endl;
	}
	indata.close();

	tensor_t_3d  conv_1_weight2D;//
	vec_t 		           conv_1_bias2D;//
	conv_1_weight2D = load_weight_conv(weight_bias_count_1,
		weight_bias_count_2,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_channel_number_conv,
		in_number_conv);
	conv_1_bias2D = load_bias_conv(weight_bias_count_1,
		weight_bias_count_2,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_channel_number_conv,
		in_number_conv);

	//convolution_1
	convolution_layer_with_table(
		tan_h,
		nn_in_data_size_conv[0],
		nn_channel_size_conv,
		in_data2D,
		has_connection_table[0],
		conv_1_weight2D,
		conv_1_bias2D,
		conv_1_out_data,
		nn_in_number_conv[0],
		nn_channel_number_conv[0]);

	in_number_conv++;

	cout << "Finished convolution layer 1" << endl;
	cout << "Starting pooling layer 1" << endl;

	vec_t pooling_1_weight;
	vec_t pooling_1_bias2D;
	pooling_1_weight = load_weight_pooling(weight_bias_count_1,
		weight_bias_count_2,
		nn_channel_size_pooling,
		nn_in_number_pooling,
		in_number_pooling);
	pooling_1_bias2D = load_bias_pooling(weight_bias_count_1,
		weight_bias_count_2,
		nn_channel_size_pooling,
		nn_in_number_pooling,
		nn_channel_number_pooling,
		in_number_pooling);
	tensor_t_3d pooling_1_out_data;

	//pooling_1
	pooling_layer(
		tan_h,
		nn_in_data_size_pooling[0],
		nn_channel_size_pooling,
		conv_1_out_data,
		pooling_1_weight,
		pooling_1_bias2D,
		pooling_1_out_data,
		nn_in_number_pooling[0]);

	in_number_pooling++;

	cout << "Finished pooling layer 1" << endl;
	cout << "Starting convolution layer 2" << endl;

	tensor_t_3d conv_2_weight2D;
	vec_t                 conv_2_bias2D;
	conv_2_weight2D = load_weight_conv(weight_bias_count_1,
		weight_bias_count_2,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_channel_number_conv,
		in_number_conv);
	conv_2_bias2D = load_bias_conv(weight_bias_count_1,
		weight_bias_count_2,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_channel_number_conv,
		in_number_conv);
	tensor_t_3d conv_2_out_data;

	//convolution_2
	convolution_layer_with_table(
		tan_h,
		nn_in_data_size_conv[1],
		nn_channel_size_conv,
		pooling_1_out_data,
		has_connection_table[1],
		conv_2_weight2D,
		conv_2_bias2D,
		conv_2_out_data,
		nn_in_number_conv[1],
		nn_channel_number_conv[1]);

	in_number_conv++;

	cout << "Finished convolution layer 2" << endl;
	cout << "Starting pooling layer 2" << endl;

	vec_t pooling_2_weight;
	vec_t pooling_2_bias2D;
	pooling_2_weight = load_weight_pooling(weight_bias_count_1,
		weight_bias_count_2,
		nn_channel_size_pooling,
		nn_in_number_pooling,
		in_number_pooling);
	pooling_2_bias2D = load_bias_pooling(weight_bias_count_1,
		weight_bias_count_2,
		nn_channel_size_pooling,
		nn_in_number_pooling,
		nn_channel_number_pooling,
		in_number_pooling);
	tensor_t_3d pooling_2_out_data;

	//pooling_2
	pooling_layer(
		tan_h,
		nn_in_data_size_pooling[1],
		nn_channel_size_pooling,
		conv_2_out_data,
		pooling_2_weight,
		pooling_2_bias2D,
		pooling_2_out_data,
		nn_in_number_pooling[1]);

	in_number_pooling++;

	cout << "Finished pooling layer 2" << endl;
	cout << "Starting convolution layer 3" << endl;


	tensor_t_3d conv_3_weight2D;
	vec_t  conv_3_bias2D;
	conv_3_weight2D = load_weight_conv(weight_bias_count_1,
		weight_bias_count_2,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_channel_number_conv,
		in_number_conv);
	conv_3_bias2D = load_bias_conv(weight_bias_count_1,
		weight_bias_count_2,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_channel_number_conv,
		in_number_conv);
	tensor_t_3d conv_3_out_data;


	//convolution_3
	convolution_layer_with_table(
		tan_h,
		nn_in_data_size_conv[2],
		nn_channel_size_conv,
		pooling_2_out_data,
		has_connection_table[2],
		conv_3_weight2D,
		conv_3_bias2D,
		conv_3_out_data,
		nn_in_number_conv[2],
		nn_channel_number_conv[2]);

	in_number_conv++;

	cout << "Finished convolution layer 3" << endl;
	cout << "Starting fully connected layer 1" << endl;

	tensor_t_3d fc_1_weight2D;
	vec_t                 fc_1_bias2D;
	fc_1_weight2D = load_weight_fc(weight_bias_count_1,
		weight_bias_count_2,
		nn_channel_size_fc,
		nn_in_number_fc,
		nn_channel_number_fc,
		in_number_fc);
	fc_1_bias2D = load_bias_fc(weight_bias_count_1,
		weight_bias_count_2,
		nn_channel_size_fc,
		nn_in_number_fc,
		nn_channel_number_fc,
		in_number_fc);
	tensor_t_3d fc_1_out_data;

	//fully_connect
	fully_connected_layer(
		tan_h,
		nn_in_data_size_fc[0],
		conv_3_out_data,
		fc_1_weight2D,
		fc_1_bias2D,
		fc_1_out_data,
		nn_in_number_fc[0],
		nn_channel_number_fc[0]);

	in_number_fc++;

	cout << "Finished fully connected layer 1" << endl;

	cout << "End of network" << endl;

	return 0;
}

