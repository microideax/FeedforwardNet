//This is the main function of a LeNet-5 model based application.
//Application description: LeNet-5 image recognition with std image.
//Using stb_image instead of OpenCV to eliminate library dependency.

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <cstdlib>
#include <time.h>

#include "../fpga_cnn/config.h"
#include "../fpga_cnn/data_type.h"
#include "../fpga_cnn/image_converter.h"
#include "../fpga_cnn/weight_bias.h"
#include "../fpga_cnn/construct_net.h"
#include "../fpga_cnn/conv_layer.h"
#include "../fpga_cnn/data_quantize.h"
#include "../fpga_cnn/read_mnist.h"
#include "../fpga_cnn/softmax.h"
#include "../fpga_cnn/predict.h"
#include "../fpga_cnn/accuracy.h"

using namespace std;

int main() {

	char tan_h = 't';
	char relu = 'r';
	char none = 'i';
	int in_number_conv = 0;  // number of convolutional layer
	int in_number_fc = 0;// number of fully_connected layer
	int in_number_pooling = 0;

	//std_vec_t data_in;
	////std_tensor_t in_data_temp;
	//float in_data_1[1][32][32] = { 0 };
	////	//convert image to data matrix
	//	const std::string filename = ".\\example\\1.bmp";
	//	convert_image(filename, 0, 1.0, 32, 32, data_in);
	//	for (int i = 0; i < data_in.size(); i++) {
	//		in_data_1[0][i / 32][i % 32] = data_in[i];
	//	}
	//	//in_data_temp = in_2_3D(data_in);
	//	/*for (int i = 0; i < 32; i++) {
	//		for (int j = 0; j < 32; j++) {
	//				in_data_1[0][i][j] = in_data_temp[i][j];
	//			}
	//		}*/
	//	ofstream indata1;
	//	indata1.open("in_data.txt", ios::app);
	//	for (int i = 0; i < 1; i++) {
	//		for (int j = 0; j < 32; j++) {
	//			for (int k = 0; k < 32; k++) {
	//				indata1 << in_data_1[i][j][k] << " ";
	//			}
	//			indata1 << endl;
	//		}
	//		indata1 << endl;
	//	}
	//	indata1.close();
	//	in_data_temp = in_2_3D(data_in);
	//	tensor_t_3d in_data2D;
	//	std_tensor_t_3d in_data2D_temp;
	//	in_data2D_temp = in_2_2D_conv(nn_in_data_size_conv[0], in_data_temp);

	//input data array
	float in_data_3D[1][28][28] = { 0 };

	ifstream ifs("./input_3.txt");
	string str;
	int count = 0;
	while (ifs >> str)
	{
	float f = atof(str.c_str());
	in_data_3D[0][count / 28][count % 28] = f;
	count++;
	}

	ofstream indata;
	indata.open("in_data.txt", ios::app);
	for (int i = 0; i < 1; i++) {
	    for (int j = 0; j < 28; j++) {
	        for (int k = 0; k < 28; k++) {
	            indata << in_data_3D[i][j][k] << " ";
	        }
	        indata << endl;
	    }
	    indata << endl;
	}
	indata.close();

    /*
	float mnist_train_data[60000][1][32][32];
	float mnist_train_label[60000][10] = { 0 };
	float mnist_test_data[10000][1][32][32];
	float mnist_test_label[10000][10] = { 0 };
	getSrcData(mnist_train_data, mnist_train_label, mnist_test_data, mnist_test_label);
	cout << "getSrcData end!!!!!!!!!!!!!!" << endl;
*/
	// Prepare weights and bias for convolution layer 1
	float        conv_1_weight2D[6][nn_channel_size_conv][nn_channel_size_conv] = { 0 };
	float 		 conv_1_bias2D[6] = { 0 };
	load_weight_conv(
		conv_1_weight2D,
		conv_1_bias2D,
		weight_bias_record,
		//		weight_bias_count_2,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_channel_number_conv,
		in_number_conv);
	load_bias_conv(
		conv_1_bias2D,
		weight_bias_record,
		//weight_bias_count_2,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_channel_number_conv,
		in_number_conv);
	in_number_conv++;


	for (int i = 0; i < 6; i++) {
	for (int j = 0; j < 5; j++) {
	for (int k = 0; k < 5; k++) {
	conv_1_weight2D[i][j][k] = roundf(conv_1_weight2D[i][j][k] * 10) / 10.0;
//	conv_1_weight2D[i][j][k] = quantize(conv_1_weight2D[i][j][k]);
	}
	}
	}
	for (int i = 0; i<6; i++) {
	conv_1_bias2D[i] = roundf(conv_1_bias2D[i] * 10) / 10;
//	conv_1_bias2D[i] = quantize(conv_1_bias2D[i]);
	}


	//cout << "conv 1 weight size = " << sizeof(conv_1_weight2D) << "  " << sizeof(conv_1_weight2D[0])<< "  " << sizeof(conv_1_weight2D[0][0]) << endl;
	//cout << "conv 1 bias size = " << sizeof(conv_1_bias2D) << endl;

	//Prepare weights and bias for pooling layer 1
	//float pool_1_weight2D[24];
	//float pool_1_bias2D[6];
	//load_weight_pooling(
	//	pool_1_weight2D,
	//	weight_bias_record,
	//	//weight_bias_count_2,
	//	nn_channel_size_pooling,
	//	nn_in_number_pooling,
	//	in_number_pooling);
	//load_bias_pooling(
	//	pool_1_bias2D,
	//	weight_bias_record,
	//	//weight_bias_count_2,
	//	nn_channel_size_pooling,
	//	nn_in_number_pooling,
	//	nn_channel_number_pooling,
	//	in_number_pooling);
	//in_number_pooling++;

	//cout << "pool 1 weight size = " << pooling_1_weight.size() << endl;
	//cout << "pool 1 bias size = " << pooling_1_bias2D.size() << endl;

	// Prepare weights and bias for convolution layer 2
	float        conv_2_weight2D[96][nn_channel_size_conv][nn_channel_size_conv] = { 0 };//
	float 		 conv_2_bias2D[16] = { 0 };//

	load_weight_conv(
		conv_2_weight2D,
		conv_2_bias2D,
		weight_bias_record,
		//		weight_bias_count_2,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_channel_number_conv,
		in_number_conv);
	load_bias_conv(
		conv_2_bias2D,
		weight_bias_record,
		//weight_bias_count_2,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_channel_number_conv,
		in_number_conv);
	in_number_conv++;


	for (int i = 0; i < 96; i++) {
	for (int j = 0; j < 5; j++) {
	for (int k = 0; k < 5; k++) {
	conv_2_weight2D[i][j][k] = roundf(conv_2_weight2D[i][j][k] * 10) / 10.0;
//	conv_2_weight2D[i][j][k] = quantize(-0.2, 0.2, conv_2_weight2D[i][j][k]);
	}
	}
	}
	for (int i = 0; i<16; i++) {
	conv_2_bias2D[i] = roundf(conv_2_bias2D[i] * 10) / 10;
//	conv_2_bias2D[i] = quantize(conv_2_bias2D[i]);
	}


	//cout << "conv 2 weight size = " << conv_2_weight2D.size() << "  " << conv_2_weight2D[0].size() << "  " << conv_2_weight2D[0][0].size() << endl;
	//cout << "conv 2 bias size = " << conv_2_bias2D.size() << endl;

	// Prepare weights and bias for pooling layer 2
	//  float pool_2_weight2D[64];
	//  float pool_2_bias2D[16];
	//  load_weight_pooling(
	//  	pool_2_weight2D,
	//weight_bias_record,
	//  	//weight_bias_count_2,
	//  	nn_channel_size_pooling,
	//  	nn_in_number_pooling,
	//  	in_number_pooling);
	//  load_bias_pooling(
	//  	pool_2_bias2D,
	//weight_bias_record,
	//  	//weight_bias_count_2,
	//  	nn_channel_size_pooling,
	//  	nn_in_number_pooling,
	//  	nn_channel_number_pooling,
	//  	in_number_pooling);
	//  in_number_pooling++;

	//cout << "pool 2 weight size = " << pooling_2_weight.size() << endl;
	//cout << "pool 2 bias size = " << pooling_2_bias2D.size() << endl;

	// Prepare weights and bias for convolution layer 3
	//tensor_t_3d fc_1_weight2D;
	float fc_1_weight2D[160][5][5] = { 0 };
	//vec_t fc_1_bias2D;
	float fc_1_bias2D[10] = { 0 };
	load_weight_fc(
		fc_1_weight2D,
		weight_bias_record,
		//weight_bias_count_2,
		nn_channel_size_fc,
		nn_in_number_fc,
		nn_channel_number_fc,
		in_number_fc);
	load_bias_fc(
		fc_1_bias2D,
		weight_bias_record,
		//weight_bias_count_2,
		nn_channel_size_fc,
		nn_in_number_fc,
		nn_channel_number_fc,
		in_number_fc);
	in_number_fc++;


	for (int i = 0; i < 160; i++) {
	for (int j = 0; j < 5; j++) {
	for (int k = 0; k < 5; k++) {
	fc_1_weight2D[i][j][k] = roundf(fc_1_weight2D[i][j][k] * 10) / 10.0;
//	fc_1_weight2D[i][j][k] = quantize(fc_1_weight2D[i][j][k]);
	}
	}
	}
	for (int i = 0; i<10; i++) {
	fc_1_bias2D[i] = roundf(fc_1_bias2D[i] * 10) / 10;
//	fc_1_bias2D[i] = quantize(fc_1_bias2D[i]);
	}

	/*float fc_1_weight[160];
	ofstream fc_weights;
	fc_weights.open("fc_weights.txt", ios::app);
	for (int i = 0; i < 160; i++) {
	fc_weights << fc_1_weight2D[i][0][0] << " ";
	fc_1_weight[i] = fc_1_weight2D[i][0][0];
	}
	fc_weights << endl;
	fc_weights.close();*/

	/*
	cout << "fc weight size = " << fc_1_weight2D.size() << "  " << fc_1_weight2D[0].size() << "  " << fc_1_weight2D[0][0].size() << endl;
	cout << "fc bias size = " << fc_1_bias2D.size() << endl;*/

	float fc_1_out_a[10000][10] = { 0 };


	/*ofstream indata_compare;
	indata_compare.open("in_data_compare.txt", ios::app);
	for (int i = 0; i < 1; i++) {
	for (int j = 0; j < 32; j++) {
	for (int k = 0; k < 32; k++) {
	indata_compare << in_data_3D[i][j][k] << " ";
	}
	indata_compare << endl;
	}
	indata_compare << endl;
	}
	indata_compare.close();*/

	clock_t start, finish;
	double totaltime;
	start = clock();

	//Inference network process
	inference_net(

		//tan_h,
		relu,

		//input pic data
        in_data_3D,
//		mnist_test_data,

		//layer weights and bias inputs
		conv_1_weight2D,
		conv_1_bias2D,
		//pool_1_weight2D,
		//pool_1_bias2D,
		conv_2_weight2D,
		conv_2_bias2D,
		//pool_2_weight2D,
		//pool_2_bias2D,
		//conv_3_weight2D,
		//conv_3_bias2D,
		fc_1_weight2D,
		fc_1_bias2D,

		//output fc data
		fc_1_out_a
	);

    /*
	softmax(fc_1_out_a);

	predict(fc_1_out_a, label_list);

	accuracy(fc_1_out_a, mnist_test_label);

	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "predicted time is: " << totaltime << " s" << endl;
	getchar();
	return 0;
    */
}