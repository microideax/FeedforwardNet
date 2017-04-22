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

#include <ap_fixed.h>

#include "config.h"
#include "construct_net.h"
#include "../../fpga_cnn/data_type.h"
#include "../../fpga_cnn/image_converter.h"
#include "../../fpga_cnn/weight_bias.h"
#include "../../fpga_cnn/conv_layer.h"
#include "../../fpga_cnn/data_quantize.h"
#include "../../fpga_cnn/read_mnist.h"
#include "../../fpga_cnn/softmax.h"
#include "../../fpga_cnn/predict.h"
#include "../../fpga_cnn/accuracy.h"

using namespace std;

int main() {

    const char* weight_src = "weights_lenet.txt";

	char tan_h = 't';
	char relu = 'r';
	char none = 'i';
	int in_number_conv = 0;  // number of convolutional layer
	int in_number_fc = 0;// number of fully_connected layer
	int in_number_pooling = 0;

#if _KERNEL_DEBUG_
	//input data array
	ap_fixed<24,16> in_data_3D[1][32][32] = { 0 };
	ifstream ifs("./input_3.txt");
	string str;
	int count = 0;
	while (ifs >> str)
	{
		float f = atof(str.c_str());
		in_data_3D[0][count / 32][count % 32] = (ap_fixed<24,16>)f;
		count++;
	}

	ofstream indata;
	indata.open("in_data.txt", ios::app);
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 32; j++) {
			for (int k = 0; k < 32; k++) {
				indata << in_data_3D[i][j][k] << " ";
			}
			indata << endl;
		}
		indata << endl;
	}
	indata.close();
#endif

#if _BATCH_MODE_
	float mnist_train_data[60000][1][32][32] = {0};
    ap_fixed<24,16> mnist_train_data_fixed[60000][1][32][32] = {0};
	float mnist_train_label[60000][10] = { 0 };
    ap_fixed<24,16> mnist_train_label_fixed[60000][10] = { 0 };
	float mnist_test_data[10000][1][32][32] = { 0 };
    ap_fixed<24,16> mnist_test_data_fixed[10000][1][32][32] = { 0 };
	float mnist_test_label[10000][10] = { 0 };
    ap_fixed<24,16> mnist_test_label_fixed[10000][10] = { 0 };
	getSrcData(mnist_train_data, mnist_train_label, mnist_test_data, mnist_test_label);
	cout << "getSrcData end!!!!!!!!!!!!!!" << endl;
    for(int i = 0; i < 10000; i++){
        for(int j = 0; j < 1; j++){
            for(int k = 0; k < 32; k++){
                for(int l = 0; l < 32; l++){
                    mnist_test_data_fixed[i][j][k][l] = ap_fixed<24,16>(mnist_test_data[i][j][k][l]);
                }
            }
        }
    }
    for(int i = 0; i < 10000; i++){
        for(int j = 0; j < 10; j++){
            mnist_test_label_fixed[i][j] = ap_fixed<24,16>(mnist_test_label[i][j]);
        }
    }
#endif

	// Prepare weights and bias for convolution layer 1
	float        conv_1_weight2D[6][5][5] = { 0 };
    ap_fixed<10,2>          conv_1_weight2D_int[6][5][5] = {0};
	float 		 conv_1_bias2D[6] = { 0 };
    ap_fixed<10,2>          conv_1_bias2D_int[6] = {0};
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

    float conv_1_min = 0;
    float conv_1_max = 0;
	for(int i = 0; i < 6; i++){
        for(int j = 0; j < 5; j++){
            for(int k = 0; k < 5; k++){
                if(conv_1_min >= conv_1_weight2D[i][j][k]) conv_1_min = conv_1_weight2D[i][j][k];
                if(conv_1_max < conv_1_weight2D[i][j][k])  conv_1_max = conv_1_weight2D[i][j][k];
                conv_1_weight2D_int[i][j][k] = ap_fixed<10,2>(conv_1_weight2D[i][j][k]);
            }
        }
    }
    cout << "conv 1 weight min = " << conv_1_min << endl;
    cout << "conv 1 weight max = " << conv_1_max << endl;
    cout << endl;
    for(int i = 0; i < 6; i++){
        conv_1_bias2D_int[i] = ap_fixed<10,2>(conv_1_bias2D[i]);
    }
    cout << endl;
	// Prepare weights and bias for convolution layer 2
	float        conv_2_weight2D[96][5][5] = { 0 };//
    ap_fixed<10,2>          conv_2_weight2D_int[96][5][5] = {0};
	float 		 conv_2_bias2D[16] = { 0 };//
    ap_fixed<10,2>          conv_2_bias2D_int[16] = {0};

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

    float conv_2_min = 0;
    float conv_2_max = 0;
    for(int i = 0; i < 96; i++){
        for(int j = 0; j < 5; j++){
            for(int k = 0; k < 5; k++){
                if(conv_2_min >= conv_2_weight2D[i][j][k]) conv_2_min = conv_2_weight2D[i][j][k];
                if(conv_2_max < conv_2_weight2D[i][j][k])  conv_2_max = conv_2_weight2D[i][j][k];
                conv_2_weight2D_int[i][j][k] = ap_fixed<10,2>(conv_2_weight2D[i][j][k]);
            }
        }
    }
    cout << "conv 2 weight min = " << conv_2_min << endl;
    cout << "conv 2 weight max = " << conv_2_max << endl;
    cout << endl;
    for(int i = 0; i < 16; i++){
        conv_2_bias2D_int[i] = ap_fixed<10,2>(conv_2_bias2D[i]);
    }

	// Prepare weights and bias for fully connected layer 1
	float fc_1_weight2D[160][5][5] = { 0 };
    ap_fixed<10,2>   fc_1_weight2D_int[160][5][5] = {0};
	float fc_1_bias2D[10] = { 0 };
    ap_fixed<10,2>   fc_1_bias2D_int[10] = {0};
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

    float fc_1_min = 0;
    float fc_1_max = 0;
    for(int i = 0; i < 160; i++){
        for(int j = 0; j < 5; j++){
            for(int k = 0; k < 5; k++){
                if(fc_1_min >= fc_1_weight2D[i][j][k]) fc_1_min = fc_1_weight2D[i][j][k];
                if(fc_1_max <  fc_1_weight2D[i][j][k]) fc_1_max = fc_1_weight2D[i][j][k];
                fc_1_weight2D_int[i][j][k] = ap_fixed<10,2>(fc_1_weight2D[i][j][k]);
            }
        }
    }
    cout << "fc 1 weight min = " << fc_1_min << endl;
    cout << "fc 1 weight max = " << fc_1_max << endl;
    cout << endl;
    for(int i = 0; i < 10; i++){
//        fc_1_bias2D[i] = round(fc_1_bias2D[i] * 100)/100;
        fc_1_bias2D_int[i] = ap_fixed<10,2>(fc_1_bias2D[i]);
    }

	float fc_1_out_a[10000][10] = { 0 };
    ap_fixed<24,16>   fc_1_out_a_int[10000][10] = {0};
	float fc_1_out_temp[10][1][1] = { 0 };
    ap_fixed<24,16>   fc_1_out_temp_int[10][1][1];
    for(int i = 0; i < 10; i++){
        fc_1_out_temp_int[i][0][0] = 0;
    }
    cout<< endl;

#if _BATCH_MODE_
	cout << "starting forward network batch process..........................." << endl;
	cout << "................................................................." << endl;

	clock_t start, finish;
	double totaltime;
	start = clock();

	for (int i = 0; i < 10000; i++) {
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
			mnist_test_data_fixed[i], //input test dataset
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
			fc_1_out_a[i][j] = float(fc_1_out_temp_int[j][0][0]);
//			fc_1_out_temp[j][0][0] = 0;
		}
	}
	cout << endl;
	softmax(fc_1_out_a);

	predict(fc_1_out_a, label_list);

	accuracy(fc_1_out_a, mnist_test_label);

	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "predicted time is: " << totaltime << " s" << endl;
	getchar();
#endif

	return 0;

}
