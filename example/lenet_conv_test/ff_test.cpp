//This is the main function of a LeNet-5 model based application.
//Application description: LeNet-5 image recognition with std image.
//Using stb_image instead of OpenCV to eliminate library dependency.

#include <iostream>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <malloc.h>
#include <ap_fixed.h>

#include "inference_net/config.h"
#include "inference_net/construct_net.h"
#include "inference_net/image_converter.h"
#include "inference_net/weight_bias_one_dim.h"
//#include "inference_net/conv_layer_one_dim.h"
#include "inference_net/softmax_one_dim.h"
#include "inference_net/predict_one_dim.h"
#include "inference_net/accuracy_one_dim.h"
#include "inference_net/pow_function.h"
#include "inference_net/resize_image.h"


using namespace std;

const unsigned char * loadfile(const std::string &file, int &size)
{
	std::ifstream fs(file.c_str(), std::ios::binary);
	fs.seekg(0, std::ios::end);
	size = fs.tellg();
	char * data = new char[size];
	fs.seekg(0);
	fs.read(data, sizeof(char) * size);
	fs.close();
	return (unsigned char *)data;
}

int main() {
	
    cout<< "Calculating memory space ... ... ... ..." << endl;
    // data size calculation
    unsigned int in_data_mem_size = (32*32) * sizeof(data_type);
    unsigned int imagenet_test_data_mem_size = (10*32*32) * sizeof(data_type);
    unsigned int conv_weight_size = (150 + 2400) * sizeof(data_type_w);
    unsigned int conv_bias_size   = (6 + 16) * sizeof(data_type_w);
    unsigned int fc_weight_size   = (4000) * sizeof(data_type_w);
    unsigned int fc_bias_size     = (10) * sizeof(data_type_w);
    unsigned int fc_8_out_size    = (10)*sizeof(data_type_o);
    unsigned int fc_out_size = (10*10*1*1) * sizeof(data_type_o);
    unsigned int out_1_size  = (96*32*32) * sizeof(data_type_o);
    unsigned int out_2_size  = (96*32*32) * sizeof(data_type_o);

    // assign memory space to different ports
#if _KERNEL_DEBUG_
    data_type   *in_data_mem_port     = (data_type*)malloc(in_data_mem_size);
    if (in_data_mem_port == NULL){
        printf("False memory allocation of in_data_mem_port\n");
    }
    else {
        printf("in data memory location = 0x%x \n", in_data_mem_port);
    }
#endif

    data_type_w *conv_weight_mem_port = (data_type_w*)malloc(conv_weight_size);
    if (conv_weight_mem_port == NULL){
        printf("False memory allocation of conv_weight_mem_port\n");
    }
    else {
        printf("conv weight memory location = 0x%x \n", conv_weight_mem_port);
    }
    data_type_w *conv_bias_mem_port   = (data_type_w*)malloc(conv_bias_size);
    if (conv_bias_mem_port == NULL){
        printf("False memory allocation of conv_bias_mem_port\n");
    }
    else {
        printf("conv bias memory location = 0x%x \n", conv_bias_mem_port);
    }
    data_type_w *fc_weight_mem_port   = (data_type_w*)malloc(fc_weight_size);
    if (fc_weight_mem_port == NULL){
        printf("False memory allocation of fc_weight_mem_port\n");
    }
    else {
        printf("fc_weight_mem_port memory location = 0x%x \n", fc_weight_mem_port);
    }
    data_type_w *fc_bias_mem_port     = (data_type_w*)malloc(fc_bias_size);
    if (fc_bias_mem_port == NULL){
        printf("False memory allocation of fc_bias_mem_port\n");
    }
    else {
        printf("fc_bias_mem_port memory location = 0x%x \n", fc_bias_mem_port);
    }
    data_type_o *temp_out_1  = (data_type_o*)malloc(out_1_size);
    if (temp_out_1 == NULL){
        printf("False memory allocation of temp_out_1\n");
    }
    else {
        printf("temp_out_1 memory location = 0x%x \n", temp_out_1);
    }
    data_type_o *temp_out_2  = (data_type_o*)malloc(out_2_size);
    if (temp_out_2 == NULL){
        printf("False memory allocation of temp_out_2\n");
    }
    else {
        printf("temp_out_2 memory location = 0x%x \n", temp_out_2);
    }

#if _KERNEL_DEBUG_
    data_type_o *fc_8_out_mem_int     = (data_type_o*)malloc(fc_8_out_size);
    if (fc_8_out_mem_int == NULL){
        printf("False memory allocation of fc_8_out_mem_int\n");
    }
    else {
        printf("fc_8_out_mem_int memory location = 0x%x \n", fc_8_out_mem_int);
    }
#endif

#if _KERNEL_DEBUG_
    cout << "FC8 mem init\n";
    memset(fc_8_out_mem_int, 0, fc_8_out_size);
    memset(temp_out_1, 0, out_1_size);
    memset(temp_out_2, 0, out_2_size);
#endif

        //net weight src *****************************
#if _HLS_MODE_
	const char* weight_src = "net_weights.txt";
#else
	cout << "net_inputs/net_weights.txt" << endl;
	const char* weight_src = "net_inputs/net_weights.txt";
	cout << "finished weight src !!!" << endl;
#endif

#if _KERNEL_DEBUG_
    //input data array
    data_type in_data_3D[784] = { 0 };
    ifstream ifs("./net_inputs/input_3_28.txt");
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
        for (int j = 0; j < 28; j++) {
            for (int k = 0; k < 28; k++) {
                indata << in_data_3D[i * 784 + 28 * j + k] << " ";
            }
            indata << endl;
        }
        indata << endl;
    }
    indata.close();

	cout << "Writing data to input data memory space ... ... ..." << endl;
    cout << endl;
    cout << endl;
    int in_data_size=0;
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 28; j++) {
			for (int k = 0; k < 28; k++) {
				temp_out_2[in_data_size] = (data_type)in_data_3D[i * 784 + 28 * j + k];
				in_data_size++;
			}
		}
	}
    cout << "Finished writing data to input data memory space ... ..." << endl;

#endif


	char tan_h = 't';
	char relu = 'r';
	char none = 'i';
	int in_number_conv = 0;  // number of convolutional layer
	int in_number_fc = 0;// number of fully_connected layer
	int in_number_pooling = 0;

	// Prepare weights and bias for convolution layer 1
	float *conv_1_weight2D = (float*)malloc(150 * sizeof(float));
    memset(conv_1_weight2D, 0, 150 * sizeof(float));
	load_weight_conv(
		weight_src,
		conv_1_weight2D,
		weight_bias_record,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_out_number_conv,
		in_number_conv);
	int conv_weight_num=0;
    cout << "Loading conv weight 1 to memory space, starting at: " << conv_weight_num << '\n';
    for(int i = 0; i < 150; i++){
        conv_weight_mem_port[conv_weight_num] = (data_type_w)conv_1_weight2D[i];
        conv_weight_num++;
    }
    free(conv_1_weight2D);
    float *conv_1_bias2D = (float*)malloc(6 * sizeof(float));
    memset(conv_1_bias2D, 0, 6 * sizeof(float));
	load_bias_conv(
		weight_src,
		conv_1_bias2D,
		weight_bias_record,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_out_number_conv,
		in_number_conv);
	in_number_conv++;
	int conv_bias_num=0;
    cout << "Loading conv bias 1 to memory space, starting at: " << conv_bias_num << '\n';
    for(int i = 0; i < 6; i++) {
        conv_bias_mem_port[conv_bias_num] = (data_type_w)conv_1_bias2D[i];
        conv_bias_num++;
    }
    free(conv_1_bias2D);

	// Prepare weights and bias for convolution layer 2
	float *conv_2_weight2D = (float*)malloc(2400 * sizeof(float));
    memset(conv_2_weight2D, 0, 2400 * sizeof(float));
	load_weight_conv(
		weight_src,
		conv_2_weight2D,
		weight_bias_record,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_out_number_conv,
		in_number_conv);
	cout << "Loading conv weight 2 to memory space, starting at: " << conv_weight_num << '\n';
    for(int i = 0; i < 2400; i++){
        conv_weight_mem_port[conv_weight_num] = (data_type_w)conv_2_weight2D[i];
        conv_weight_num++;
    }
    free(conv_2_weight2D);
    float *conv_2_bias2D = (float*)malloc(16 * sizeof(float));
    memset(conv_2_bias2D, 0, 16 * sizeof(float));
	load_bias_conv(
		weight_src,
		conv_2_bias2D,
		weight_bias_record,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_out_number_conv,
		in_number_conv);
	in_number_conv++;
	cout << "Loading conv bias 2 to memory space, starting at: " << conv_bias_num << '\n';
    for(int i = 0; i < 16; i++){
        conv_bias_mem_port[conv_bias_num] = (data_type_w)conv_2_bias2D[i];
        conv_bias_num++;
    }
    free(conv_2_bias2D);

    cout<<"Finished loading conv weight into memory! Total: " << conv_weight_num  << "... ... ..."<<endl;
    cout<<"Finished loading conv bias into memory! Total: " << conv_bias_num  << "... ... ..."<<endl;

	// Prepare weights and bias for fc layer 6
	float *fc_6_weight2D = (float*)malloc(4000 * sizeof(float));
    memset(fc_6_weight2D, 0, 4000 * sizeof(float));
    load_weight_fc(
        weight_src,
        fc_6_weight2D,
        weight_bias_record,
        nn_channel_size_fc,
        nn_in_number_fc,
        nn_out_number_fc,
        in_number_fc);
    int fc_weight_num=0;
    cout << "Loading fc weight 6 to memory space, starting at: " << fc_weight_num << '\n';
    for(int i = 0; i < 4000; i++){
        fc_weight_mem_port[fc_weight_num]=(data_type_w)fc_6_weight2D[i];
        fc_weight_num++;
    }
    free(fc_6_weight2D);
    float *fc_6_bias2D = (float*)malloc(10 * sizeof(float));
    memset(fc_6_bias2D, 0, 10 * sizeof(float));
    load_bias_fc(
        weight_src,
        fc_6_bias2D,
        weight_bias_record,
        nn_channel_size_fc,
        nn_in_number_fc,
        nn_out_number_fc,
        in_number_fc);
    in_number_fc++;
    int fc_bias_num=0;
    cout << "Loading fc bias 6 to memory space, starting at: " << fc_bias_num << '\n';
    for(int i = 0; i < 10; i++){
        fc_bias_mem_port[fc_bias_num]=(data_type_w)fc_6_bias2D[i];
        fc_bias_num++;
    }
    free(fc_6_bias2D);

    cout<<"Finished loading fc weight into memory! Total: " << fc_weight_num  << "... ... ..."<<endl;
    cout<<"Finished loading fc bias into memory! Total: " << fc_bias_num  << "... ... ..."<<endl;

#if _KERNEL_DEBUG_
	float fc_8_out[10*1*1] = { 0 };
	clock_t start, finish, inf_start, inf_finish;
	double totaltime, inf_time;
	start = clock();
#endif


	//Inference network process
	inference_net(
	//activation function
	relu, 

#if _KERNEL_DEBUG_
    //input pic data
	in_data_mem_port, 
#endif

	//layer weights and bias inputs
    conv_weight_mem_port,
    conv_bias_mem_port,
    fc_weight_mem_port,
    fc_bias_mem_port,

#if _KERNEL_DEBUG_
	//output fc data
	fc_8_out_mem_int,
    temp_out_1,
    temp_out_2);

    finish = clock();
    totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
    cout <<"inference time is: " << totaltime << " s" << endl;

    for(int i=0;i<10;i++){
        fc_8_out[i]=(float)(fc_8_out_mem_int[i]);
	}
	softmax(fc_8_out,10);
	predict(fc_8_out,10);
#endif

#if _BATCH_MODE_
	fc_out_mem_int);

	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "batch mode time cost is: " << totaltime << " s" << endl;

	//test imagenet dataset
	for (int j = 0; j < 10; j++) {
		fc_out_a[i * 10 + j] = (float)(fc_out_mem_int[j]);
		fc_out_mem_int[j] = (data_type_o)(0);
		}
	}
	softmax(fc_out_a,10,10);
	predict(fc_out_a,10,10);
	accuracy(fc_out_a,10, val_class,10);//for imagenet dataset


#endif


	return 0;

}
