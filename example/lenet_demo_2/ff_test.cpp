//This is the main function of a LeNet-5 model based application.
//Application description: LeNet-5 image recognition with std image.
//Using stb_image instead of OpenCV to eliminate library dependency.

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <time.h>

#include <ap_fixed.h>

#include "config.h"
#include "construct_net.h"
#include "../../fpga_cnn/data_type.h"
#include "../../fpga_cnn/image_converter.h"
#include "../../fpga_cnn/weight_bias_one_dim.h"
#include "../../fpga_cnn/conv_layer_one_dim.h"
#include "../../fpga_cnn/data_quantize.h"
#include "../../fpga_cnn/read_mnist_one_dim.h"
#include "../../fpga_cnn/softmax_one_dim.h"
#include "../../fpga_cnn/predict_one_dim.h"
#include "../../fpga_cnn/accuracy_one_dim.h"
#include "get_config_params.h"

using namespace std;

int main() {
	//net weight src *****************************
	const char* weight_src = "net_weights.txt";

	char tan_h = 't';
	char relu = 'r';
	char none = 'i';
	int in_number_conv = 0;  // number of convolutional layer
	int in_number_fc = 0;// number of fully_connected layer
	int in_number_pooling = 0;

#if _KERNEL_DEBUG_
	//input data array
	data_type in_data_3D[1024] = { 0 };
	ifstream ifs("./input_3.txt");
	string str;
	int count = 0;
	while (ifs >> str)
	{
		float f = atof(str.c_str());
		in_data_3D[count] = (data_type)f;
		count++;
	}

	ofstream indata;
	indata.open("in_data.txt", ios::app);
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 32; j++) {
			for (int k = 0; k < 32; k++) {
				indata << in_data_3D[i * 1024 + 32 * j + k] << " ";
			}
			indata << endl;
		}
		indata << endl;
	}
	indata.close();
#endif

#if _BATCH_MODE_
	float mnist_train_data[60000 * 1 * 32 * 32];
	data_type mnist_train_data_fixed[60000 * 1 * 32 * 32] = {0};
	float mnist_train_label[60000 * 10] = { 0 };
	data_type mnist_train_label_fixed[60000 * 10] = { 0 };
	float mnist_test_data[10000 * 1 * 32 * 32];
	data_type mnist_test_data_fixed[10000 * 1 * 32 * 32];
	float mnist_test_label[10000 * 10] = { 0 };
	data_type mnist_test_label_fixed[10000 * 10];
	for(int i = 0; i < 10000 * 1 * 32 * 32; i++){
        mnist_test_data_fixed[i] = (data_type)(0);
    }
    for(int i = 0; i < 10000 * 10; i++){
        mnist_test_label_fixed[i] = (data_type)(0);
    }
	//getSrcData(mnist_train_data, mnist_train_label, mnist_test_data, mnist_test_label);
    getSrcData(mnist_train_data_fixed, mnist_train_label_fixed, mnist_test_data_fixed, mnist_test_label_fixed);
	cout << "getSrcData end!!!!!!!!!!!!!!" << endl;
#endif

	// Prepare weights and bias for convolution layer 1
	float        conv_1_weight2D[150] = { 0 };
	data_type_w          conv_1_weight2D_int[150] = {0};
	float 		 conv_1_bias2D[6] = { 0 };
    data_type_w          conv_1_bias2D_int[6] = {0};
	load_weight_conv(
		weight_src,
		conv_1_weight2D,
		weight_bias_record,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_out_number_conv,
		in_number_conv);
	load_bias_conv(
		weight_src,
		conv_1_bias2D,
		weight_bias_record,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_out_number_conv,
		in_number_conv);
	in_number_conv++;

	for(int i = 0; i < 150; i++){
        conv_1_weight2D_int[i] = (data_type_w)(conv_1_weight2D[i]);
    }
    for(int i = 0; i < 6; i++){
        conv_1_bias2D_int[i] = (data_type_w)(conv_1_bias2D[i]);
    }

	// Prepare weights and bias for convolution layer 2
	float        conv_2_weight2D[2400] = { 0 };//
	data_type_w          conv_2_weight2D_int[2400] = {0};
	float 		 conv_2_bias2D[16] = { 0 };//
	data_type_w          conv_2_bias2D_int[16] = {0};

	load_weight_conv(
		weight_src,
		conv_2_weight2D,
		weight_bias_record,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_out_number_conv,
		in_number_conv);
	load_bias_conv(
		weight_src,
		conv_2_bias2D,
		weight_bias_record,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_out_number_conv,
		in_number_conv);
	in_number_conv++;

	for(int i = 0; i < 2400; i++){
        conv_2_weight2D_int[i] = (data_type_w)(conv_2_weight2D[i]);
    }
    for(int i = 0; i < 16; i++){
        conv_2_bias2D_int[i] = (data_type_w)(conv_2_bias2D[i]);
    }

	// Prepare weights and bias for fully connected layer 1
	float fc_1_weight2D[4000] = { 0 };
	data_type_w          fc_1_weight2D_int[4000] = {0};
	float fc_1_bias2D[10] = { 0 };
	data_type_w          fc_1_bias2D_int[10] = {0};
	load_weight_fc(
		weight_src,
		fc_1_weight2D,
		weight_bias_record,
		nn_channel_size_fc,
		nn_in_number_fc,
		nn_out_number_fc,
		in_number_fc);
	load_bias_fc(
		weight_src,
		fc_1_bias2D,
		weight_bias_record,
		nn_channel_size_fc,
		nn_in_number_fc,
		nn_out_number_fc,
		in_number_fc);
	in_number_fc++;

	for(int i = 0; i < 4000; i++){
        fc_1_weight2D_int[i] = (data_type_w)(fc_1_weight2D[i]);
    }
    for(int i = 0; i < 10; i++){
        fc_1_bias2D_int[i] = (data_type_w)(fc_1_bias2D[i]);
    }

	float fc_1_out_a[10000 * 10] = { 0 };
	data_type_o   fc_1_out_a_int[10000*10] = {0};
	float fc_1_out_temp[10] = { 0 };
	data_type_o   fc_1_out_temp_int[10];
    for(int i = 0; i < 10; i++){
        fc_1_out_temp_int[i] = (data_type_o)(0);
    }

#if _BATCH_MODE_
	cout << "starting forward network batch process..........................." << endl;
	cout << "................................................................." << endl;

	clock_t start, finish;
	double totaltime;
	start = clock();

	for (int i = 0; i < 10000; i++) {
		if (i % 200 == 0) {
			cout << ">>";
		}
#endif

#if _KERNEL_DEBUG_
		cout << "starting forward network single picture processing ..............." << endl;
		cout << ".................................................................." << endl;
#endif

		//Inference network process
		inference_net(
			relu, //activation function
#if _KERNEL_DEBUG_
			in_data_3D, //input pic data
#endif
#if _BATCH_MODE_
			mnist_test_data_fixed + i * 32 * 32, //input test dataset
#endif
										   //layer weights and bias inputs
			conv_1_weight2D_int,
			conv_1_bias2D_int,
			conv_2_weight2D_int,
			conv_2_bias2D_int,
			fc_1_weight2D_int,
			fc_1_bias2D_int,

			//output fc data
			fc_1_out_temp_int);

#if _BATCH_MODE_
		for (int j = 0; j < 10; j++) {
			fc_1_out_a[i * 10 + j] = float(fc_1_out_temp_int[j]);
			fc_1_out_temp_int[j] = (data_type_o)(0);
		}
	}
	cout << endl;
	softmax(fc_1_out_a, 10000, 10);

	predict(fc_1_out_a, label_list, 10000, 10);

	for(int j=0;j<10000*10;j++){
        mnist_test_label[j]=(float)(mnist_test_label_fixed[j]);
    }

	accuracy(fc_1_out_a, mnist_test_label, 10000, 10);

	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "predicted time is: " << totaltime << " s" << endl;
	//getchar();
#endif
#if _HLS_MODE_
	cout << "finished inference processing ............" << endl;
	ofstream predict_output;
	predict_output.open("predict_output.txt", ios::app);
	for (int i = 0; i < 10; i++) {
		predict_output << fc_1_out_temp[i] << " " << endl;
	}
	predict_output.close();
	cout << endl;
#endif
#if _KERNEL_DEBUG_
	for (int j = 0; j < 10; j++) {
            fc_1_out_temp[j] = (float)(fc_1_out_temp_int[j]);
        }
	softmax(fc_1_out_temp, 10);

	predict(fc_1_out_temp, 10);
#endif
	return 0;

}