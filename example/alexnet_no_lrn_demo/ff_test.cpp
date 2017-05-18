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
#include "inference_net/conv_layer_one_dim.h"
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
    unsigned int in_data_mem_size = (3*32*32) * sizeof(data_type);
    unsigned int imagenet_test_data_mem_size = (10*3*32*32) * sizeof(data_type);
    unsigned int conv_weight_size = (288*3*3 + 12288*3*3 + 98304*3*3 + 73728*3*3 + 49152*3*3) * sizeof(data_type_w);
    unsigned int conv_bias_size   = (96 + 256 + 384 + 384 +256) * sizeof(data_type_w);
    unsigned int fc_weight_size   = (1048576 + 16777216 + 4096000) * sizeof(data_type_w);
    unsigned int fc_bias_size     = (4096 + 4096 + 10) * sizeof(data_type_w);
    unsigned int fc_8_out_size    = (10)*sizeof(data_type_o);
    unsigned int fc_out_size = (10*10*1*1) * sizeof(data_type_o);
    unsigned int out_1_size  = (96*16*16) * sizeof(data_type_o);
    unsigned int out_2_size  = (96*16*16) * sizeof(data_type_o);

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
#if _BATCH_MODE_
    data_type   *imagenet_test_data_mem_port     = (data_type*)malloc(imagenet_test_data_mem_size);
    if (imagenet_test_data_mem_port == NULL){
        printf("False memory allocation of imagenet_test_data_mem_port\n");
    }
    else {
        printf("imagenet_test_data memory location = 0x%x \n", imagenet_test_data_mem_port);
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
#if _BATCH_MODE_
    data_type_o *fc_out_mem_int = (data_type_o*)malloc(fc_out_size);
    if (fc_out_mem_int == NULL){
        printf("False memory allocation of fc_out_mem_int\n");
    }
    else {
        printf("fc_out_mem_int memory location = 0x%x \n", fc_out_mem_int);
    }
#endif

#if _KERNEL_DEBUG_
    cout << "FC8 mem init\n";
    memset(fc_8_out_mem_int, 0, fc_8_out_size);
    memset(temp_out_1, 0, out_1_size);
    memset(temp_out_2, 0, out_2_size);
#endif
#if _BATCH_MODE_
    cout << "FC out mem init\n";
    memset(fc_out_mem_int, 0, fc_out_size);
#endif

	//net weight src *****************************
#if _HLS_MODE_
	const char* weight_src = "net_weights.txt";
#else
	cout << "net_inputs/net_weights.txt" << endl;
	const char* weight_src = "net_inputs/net_weights.txt";
	cout << "finished weight src !!!" << endl;
#endif
	//load mean file *****************************
#if _HLS_MODE_
	ifstream ifs1("net_mean.txt");
#else
	ifstream ifs1("net_inputs/net_mean.txt");
#endif
	float channel_mean[3] = { 0 };
	string str1;
	string y1 = "[";
	string y2 = "]";
	if (!ifs1) {
		cout << "mean data file not found !" << endl;
		getchar();
	}
	int index = 0;
	while (ifs1 >> str1) {
		int p1 = str1.find(y1, 0);  // p为找到的位置,-1为未找到
		if (p1 >= 0) {
			str1.erase(p1, y1.length());
		}
		int p2 = str1.find(y2, 0);  // p为找到的位置,-1为未找到
		if (p2 >= 0) {
			str1.erase(p2, y2.length());
		}
		float f = atof(str1.c_str());
		channel_mean[index] = f;
		index++;
	}
	ifs1.close();

	//load val (image_name,image_class) set file *****************************
#if _HLS_MODE_
	ifstream ifs("val.txt");
#else
	ifstream ifs("net_inputs/val.txt");
#endif
	string val_name[10];
	float val_class[10];
	string str;

	if (!ifs) {
		cout << "val data file not found !" << endl;
		getchar();
	}
	int num = 0;
	while (ifs >> str&&num<20) {//num:4 pair (image_name,image_class)
		if (num % 2 == 1) {//image_name
			val_class[num / 2] = int(atof(str.c_str()));
		}
		else {//image_class
			val_name[num / 2] = str;
		}
		num++;
	}
	ifs.close();


	//load val image file *****************************
#if _KERNEL_DEBUG_
#if _HLS_MODE_
	string image_dir = "48870.png";//validation dataset dir
#else
	string image_dir = "./net_inputs/cifar10_val/48870.png";
#endif
	float in_data_3D_channel_swap[3][32][32] = { 0 };
	//input data array
	float in_data_3D[3][32][32] = { 0 };
	//data_type in_data_3D_int[3*227*227] = { 0 };
	int crop_w = 32;
	int crop_h = 32;
	int w;
	int h;
	int channels;
	int size;
	const unsigned char * data = loadfile(image_dir, size);
	const unsigned char * image_orig = stbi_load_from_memory(data, size, &w, &h, &channels, 3);

	for (int i = 0; i < 3; i++) {
		for (int j = i; j < w * h * 3; j += 3) {
			in_data_3D_channel_swap[2 - i][j / (w * 3)][(j % (w * 3) - i) / 3] = (float)image_orig[j];//range:0--255
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < h; j++) {
			for (int k = 0; k < w; k++) {
				in_data_3D_channel_swap[i][j][k] /= 255;//range:0--1
			}
		}
	}

	//resize_image(in_data_3D_channel_swap, h, w, in_data_3D);//in_data after crop

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < crop_h; j++) {
			for (int k = 0; k < crop_w; k++) {
				in_data_3D[i][j][k] = in_data_3D_channel_swap[i][j][k] * 255 - channel_mean[i];
			}
		}
	}

	cout << "Writing data to input data memory space ... ... ..." << endl;
        cout << endl;
        cout << endl;
        int in_data_size=0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < crop_h; j++) {
			for (int k = 0; k < crop_w; k++) {
				in_data_mem_port[in_data_size] = (data_type)in_data_3D[i][j][k];
				in_data_size++;
			}
		}
	}
        cout << "Finished writing data to input data memory space ... ..." << endl;

#endif

#if _BATCH_MODE_
	//load val image set file *****************************
	string root_dir = "./net_inputs/cifar10_val/";
	float imagenet_test_data_channel_swap[10][3][32][32] = { 0 };
	float imagenet_test_data[10][3][32][32] = { 0 };
	//data_type imagenet_test_data_int[4*3*227*227] = { 0 };
	for (int image_num = 0; image_num < 10; image_num++) {
		string image_dir = root_dir + val_name[image_num];
		int crop_w = 32;
                int crop_h = 32;
		int w;
		int h;
		int channels;
		int size;
		const unsigned char * data = loadfile(image_dir, size);
		const unsigned char * image_orig = stbi_load_from_memory(data, size, &w, &h, &channels, 3);

		for (int i = 0; i < 3; i++) {
			for (int j = i; j < w * h * 3; j += 3) {
				imagenet_test_data_channel_swap[image_num][2 - i][j / (w * 3)][(j % (w * 3) - i) / 3] = (float)image_orig[j];//range:0--255
			}
		}

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < h; j++) {
				for (int k = 0; k < w; k++) {
					imagenet_test_data_channel_swap[image_num][i][j][k] /= 255;//range:0--1
				}
			}
		}

		//resize_image(imagenet_test_data_channel_swap[image_num], h, w, imagenet_test_data[image_num]);//in_data after crop

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < crop_h; j++) {
				for (int k = 0; k < crop_w; k++) {
					imagenet_test_data[image_num][i][j][k] = imagenet_test_data_channel_swap[image_num][i][j][k] * 255 - channel_mean[i];
				}
			}
		}

		int in_data_size=0;
		for (int i = 0; i < 3; i++) {
		for (int j = 0; j < crop_h; j++) {
			for (int k = 0; k < crop_w; k++) {
				imagenet_test_data_mem_port[image_num*3*crop_h*crop_w+in_data_size] = (data_type)imagenet_test_data[image_num][i][j][k];
				in_data_size++;
			}
		}
	}

		ofstream indata;
		indata.open("in_data_crop_mean.txt", ios::app);
		indata << "image_num: " << image_num << "**********" << endl;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < crop_h; j++) {
				for (int k = 0; k < crop_w; k++) {
					indata << imagenet_test_data_mem_port[image_num*3*crop_h*crop_w+i*crop_h*crop_w+j*crop_w+k] << " ";
				}
				indata << endl;
			}
			indata << endl;
		}
		indata << endl;
		indata.close();
	}
#endif

	char tan_h = 't';
	char relu = 'r';
	char none = 'i';
	int in_number_conv = 0;  // number of convolutional layer
	int in_number_fc = 0;// number of fully_connected layer
	int in_number_pooling = 0;

	// Prepare weights and bias for convolution layer 1
	float *conv_1_weight2D = (float*)malloc(288 * 3 * 3 * sizeof(float));
    memset(conv_1_weight2D, 0, 288 * 3 * 3 * sizeof(float));
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
    for(int i = 0; i < 288*3 * 3; i++){
        conv_weight_mem_port[conv_weight_num] = (data_type_w)conv_1_weight2D[i];
        conv_weight_num++;
    }
    free(conv_1_weight2D);
    float *conv_1_bias2D = (float*)malloc(96 * sizeof(float));
    memset(conv_1_bias2D, 0, 96 * sizeof(float));
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
    for(int i = 0; i < 96; i++) {
        conv_bias_mem_port[conv_bias_num] = (data_type_w)conv_1_bias2D[i];
        conv_bias_num++;
    }
    free(conv_1_bias2D);

	// Prepare weights and bias for convolution layer 2
	float *conv_2_weight2D = (float*)malloc(12288*3 * 3 * sizeof(float));
    memset(conv_2_weight2D, 0, 12288 * 3 * 3 * sizeof(float));
	load_weight_conv(
		weight_src,
		conv_2_weight2D,
		weight_bias_record,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_out_number_conv,
		in_number_conv);
	cout << "Loading conv weight 2 to memory space, starting at: " << conv_weight_num << '\n';
    for(int i = 0; i < 12288*3 * 3; i++){
        conv_weight_mem_port[conv_weight_num] = (data_type_w)conv_2_weight2D[i];
        conv_weight_num++;
    }
    free(conv_2_weight2D);
    float *conv_2_bias2D = (float*)malloc(256 * sizeof(float));
    memset(conv_2_bias2D, 0, 256 * sizeof(float));
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
    for(int i = 0; i < 256; i++){
        conv_bias_mem_port[conv_bias_num] = (data_type_w)conv_2_bias2D[i];
        conv_bias_num++;
    }
    free(conv_2_bias2D);

	// Prepare weights and bias for convolution layer 3
	float *conv_3_weight2D = (float*)malloc(98304*3*3 * sizeof(float));
    memset(conv_3_weight2D, 0, 98304 * 3 * 3 * sizeof(float));
    load_weight_conv(
        weight_src,
        conv_3_weight2D,
        weight_bias_record,
        nn_channel_size_conv,
        nn_in_number_conv,
        nn_out_number_conv,
        in_number_conv);
    cout << "Loading conv weight 3 to memory space, starting at: " << conv_weight_num << '\n';
    for(int i = 0; i < 98304*3*3; i++){
        conv_weight_mem_port[conv_weight_num] = (data_type_w)conv_3_weight2D[i];
        conv_weight_num++;
    }
    free(conv_3_weight2D);
    float *conv_3_bias2D = (float*)malloc(384 * sizeof(float));
    memset(conv_3_bias2D, 0, 384 * sizeof(float));
    load_bias_conv(
        weight_src,
        conv_3_bias2D,
        weight_bias_record,
        nn_channel_size_conv,
        nn_in_number_conv,
        nn_out_number_conv,
        in_number_conv);
    in_number_conv++;
    cout << "Loading conv bias 3 to memory space, starting at: " << conv_bias_num << '\n';
    for(int i = 0; i < 384; i++){
        conv_bias_mem_port[conv_bias_num] = (data_type_w)conv_3_bias2D[i];
        conv_bias_num++;
    }
    free(conv_3_bias2D);

	// Prepare weights and bias for convolution layer 4
	float *conv_4_weight2D = (float*)malloc(73728*3*3 * sizeof(float));
    memset(conv_4_weight2D, 0, 73728 * 3 * 3 * sizeof(float));
    load_weight_conv(
        weight_src,
        conv_4_weight2D,
        weight_bias_record,
        nn_channel_size_conv,
        nn_in_number_conv,
        nn_out_number_conv,
        in_number_conv);
    cout << "Loading conv weight 4 to memory space, starting at: " << conv_weight_num << '\n';
    for(int i = 0; i < 73728*3*3; i++){
        conv_weight_mem_port[conv_weight_num] = (data_type_w)conv_4_weight2D[i];
        conv_weight_num++;
    }
    free(conv_4_weight2D);
    float *conv_4_bias2D = (float*)malloc(384 * sizeof(float));
    memset(conv_4_bias2D, 0, 384 * sizeof(float));
    load_bias_conv(
        weight_src,
        conv_4_bias2D,
        weight_bias_record,
        nn_channel_size_conv,
        nn_in_number_conv,
        nn_out_number_conv,
        in_number_conv);
    in_number_conv++;
    cout << "Loading conv bias 4 to memory space, starting at: " << conv_bias_num << '\n';
    for(int i = 0; i < 384; i++){
        conv_bias_mem_port[conv_bias_num] = (data_type_w)conv_4_bias2D[i];
        conv_bias_num++;
    }
    free(conv_4_bias2D);

	// Prepare weights and bias for convolution layer 5
	float *conv_5_weight2D = (float*)malloc(49152*3*3 * sizeof(float));
    memset(conv_5_weight2D, 0, 49152 * 3 * 3 * sizeof(float));
    load_weight_conv(
        weight_src,
        conv_5_weight2D,
        weight_bias_record,
        nn_channel_size_conv,
        nn_in_number_conv,
        nn_out_number_conv,
        in_number_conv);
    cout << "Loading conv weight 5 to memory space, starting at: " << conv_weight_num << '\n';
    for(int i = 0; i < 49152*3*3; i++){
        conv_weight_mem_port[conv_weight_num] = (data_type_w)conv_5_weight2D[i];
        conv_weight_num++;
    }
    free(conv_5_weight2D);
    float *conv_5_bias2D = (float*)malloc(256 * sizeof(float));
    memset(conv_5_bias2D, 0, 256 * sizeof(float));
    load_bias_conv(
        weight_src,
        conv_5_bias2D,
        weight_bias_record,
        nn_channel_size_conv,
        nn_in_number_conv,
        nn_out_number_conv,
        in_number_conv);
    in_number_conv++;
    cout << "Loading conv bias 5 to memory space, starting at: " << conv_bias_num << '\n';
    for(int i = 0; i < 256; i++) {
        conv_bias_mem_port[conv_bias_num] = (data_type_w) conv_5_bias2D[i];
        conv_bias_num++;
    }
    free(conv_5_bias2D);

    cout<<"Finished loading conv weight into memory! Total: " << conv_weight_num  << "... ... ..."<<endl;
    cout<<"Finished loading conv bias into memory! Total: " << conv_bias_num  << "... ... ..."<<endl;

	// Prepare weights and bias for fc layer 6
	float *fc_6_weight2D = (float*)malloc(1048576*1*1 * sizeof(float));
    memset(fc_6_weight2D, 0, 1048576 * 1 * 1 * sizeof(float));
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
    for(int i = 0; i < 1048576*1*1; i++){
        fc_weight_mem_port[fc_weight_num]=(data_type_w)fc_6_weight2D[i];
        fc_weight_num++;
    }
    free(fc_6_weight2D);
    float *fc_6_bias2D = (float*)malloc(4096 * sizeof(float));
    memset(fc_6_bias2D, 0, 4096 * sizeof(float));
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
    for(int i = 0; i < 4096; i++){
        fc_bias_mem_port[fc_bias_num]=(data_type_w)fc_6_bias2D[i];
        fc_bias_num++;
    }
    free(fc_6_bias2D);

	// Prepare weights and bias for fc layer 7
	float *fc_7_weight2D = (float*)malloc(16777216*1*1 * sizeof(float));
    memset(fc_7_weight2D, 0, 16777216 * 1 * 1 * sizeof(float));
    load_weight_fc(
        weight_src,
        fc_7_weight2D,
        weight_bias_record,
        nn_channel_size_fc,
        nn_in_number_fc,
        nn_out_number_fc,
        in_number_fc);
    cout << "Loading fc weight 7 to memory space, starting at: " << fc_weight_num << '\n';
    for(int i = 0; i < 16777216*1*1; i++){
        fc_weight_mem_port[fc_weight_num]=(data_type_w)fc_7_weight2D[i];
        fc_weight_num++;
    }
    free(fc_7_weight2D);
    float *fc_7_bias2D = (float*)malloc(4096 * sizeof(float));
    memset(fc_7_bias2D, 0, 4096 * sizeof(float));
    load_bias_fc(
        weight_src,
        fc_7_bias2D,
        weight_bias_record,
        nn_channel_size_fc,
        nn_in_number_fc,
        nn_out_number_fc,
        in_number_fc);
    in_number_fc++;
    cout << "Loading fc bias 7 to memory space, starting at: " << fc_bias_num << '\n';
    for(int i = 0; i < 4096; i++){
        fc_bias_mem_port[fc_bias_num]=(data_type_w)fc_7_bias2D[i];
        fc_bias_num++;
    }
    free(fc_7_bias2D);

	// Prepare weights and bias for fc layer 8
	float *fc_8_weight2D = (float*)malloc(40960*1*1 * sizeof(float));
    memset(fc_8_weight2D, 0, 40960 * 1 * 1 * sizeof(float));
    load_weight_fc(
        weight_src,
        fc_8_weight2D,
        weight_bias_record,
        nn_channel_size_fc,
        nn_in_number_fc,
        nn_out_number_fc,
        in_number_fc);
    cout << "Loading fc weight 8 to memory space, starting at: " << fc_weight_num << '\n';
    for(int i = 0; i < 40960*1*1; i++){
        fc_weight_mem_port[fc_weight_num]=(data_type_w)fc_8_weight2D[i];
        fc_weight_num++;
    }
    free(fc_8_weight2D);
    float *fc_8_bias2D = (float*)malloc(10 * sizeof(float));
    memset(fc_8_bias2D, 0, 10 * sizeof(float));
    load_bias_fc(
        weight_src,
        fc_8_bias2D,
        weight_bias_record,
        nn_channel_size_fc,
        nn_in_number_fc,
        nn_out_number_fc,
        in_number_fc);
    in_number_fc++;
    cout << "Loading fc bias 8 to memory space, starting at: " << fc_bias_num << '\n';
    for(int i = 0; i < 10; i++){
        fc_bias_mem_port[fc_bias_num]=(data_type_w)fc_8_bias2D[i];
        fc_bias_num++;
    }
    free(fc_8_bias2D);

    cout<<"Finished loading fc weight into memory! Total: " << fc_weight_num  << "... ... ..."<<endl;
    cout<<"Finished loading fc bias into memory! Total: " << fc_bias_num  << "... ... ..."<<endl;

#if _KERNEL_DEBUG_
	float fc_8_out[10*1*1] = { 0 };
	clock_t start, finish;
	double totaltime;
	start = clock();
#endif

#if _BATCH_MODE_
	float fc_out_a[10*10*1*1] = { 0 };

	cout << "starting forward network batch process..........................." << endl;
	cout << "..........................................................." << endl;

	clock_t start, finish;
	double totaltime;
	start = clock();

	for (int i = 0; i < 10; i++) {//test imagenet dataset
#endif

	//Inference network process
	inference_net(
	//activation function
	relu, 

#if _KERNEL_DEBUG_
    //input pic data
	in_data_mem_port, 
#endif

#if _BATCH_MODE_
        //input test imagenet dataset
	imagenet_test_data_mem_port + i * 3 * 32 * 32,
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
    for (int i = 0; i < 10; i++)
        cout << "At " << i <<", output1_tmp=" << temp_out_1[i] << '\n';
    for (int i = 0; i < 10; i++)
        cout << "At " << i <<", output2_tmp=" << temp_out_2[i] << '\n';

    for(int i=0;i<10;i++){
        fc_8_out[i]=(float)(fc_8_out_mem_int[i]);
	}
	softmax(fc_8_out,10);
	predict(fc_8_out,10);
#endif

#if _BATCH_MODE_
	fc_out_mem_int);
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

	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "predicted time is: " << totaltime << " s" << endl;

	return 0;

}
