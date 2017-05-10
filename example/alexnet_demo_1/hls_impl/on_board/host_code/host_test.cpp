// host processor functions, read data from files to PS-DDR and starts computaiton
// edited by Yao Chen on 6 May, 2017
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <ap_fixed.h>

#include "fpga_comm.h"
#include "config.h"
#include "xinference_net.h"

#include "../../../../../fpga_cnn/data_type.h"
#include "../../../../../fpga_cnn/image_converter.h"
#include "../../../../../fpga_cnn/weight_bias_one_dim.h"
#include "../../../../../fpga_cnn/read_mnist_one_dim.h"
#include "../../../../../fpga_cnn/softmax_one_dim.h"
#include "../../../../../fpga_cnn/predict_one_dim.h"
#include "../../../../../fpga_cnn/accuracy_one_dim.h"
#include "../../../../../fpga_cnn/resize_image.h"

//#include "drivers/xinferecne_net_hw.h"

#include <sys/time.h>

#define BASE_ADDRESS_CONV_WEIGHT 0x1a000000
#define BASE_ADDRESS_CONV_BIAS   0x1b000000
#define BASE_ADDRESS_FC_WEIGHT   0x1c000000
#define BASE_ADDRESS_FC_BIAS     0x1d000000

#define CRTL_ADDR    0x43C00000
#define IN_DATA_ADDR 0x0A000000
#define CONV_W_ADDR  0x0B000000
#define CONV_B_ADDR  0x0C000000
#define FC_W_ADDR    0x10000000
#define FC_B_ADDR    0x0D000000
#define FC_OUT_ADDR  0x0E000000
#define OUT_1_ADDR   0X0F000000
#define OUT_2_ADDR   0X0F200000

using namespace std;

struct timeval tv1, tv2;
void runTest(int argc, char** argv);

/////////////////////////////////////////////////////////////////////////////////
////  program main///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
    gettimeofday(&tv1, NULL);
    runTest(argc, argv);
    gettimeofday(&tv2, NULL);
    double time = (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) / 1000000;
    cout<<"Total time of single inference = "<< time <<" seconds"<<endl;
    return 0;
}

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

void runTest(int argc, char** argv)
{

    //initial memory device
    int mem_dev = open("/dev/mem", O_RDWR | O_SYNC);
    if(mem_dev == -1)
    {
        // Error: can not ger mem device
        printf("Error when opening /dev/mem\n");
        return;
    }
    cout<< "Calculating memory space ... ... ... ..." << endl;
    // data size calculation
    unsigned int in_data_mem_size = (3*227*227) * sizeof(data_type);
    unsigned int conv_weight_size = (288*11*11 + 12288*5*5 + 98304*3*3 + 73728*3*3 + 49152*3*3) * sizeof(data_type_w);
    unsigned int conv_bias_size   = (96 + 256 + 384 + 384 +256) * sizeof(data_type_w);
    unsigned int fc_weight_size   = (1048576*6*6 + 16777216 + 4096000) * sizeof(data_type_w);
    unsigned int fc_bias_size     = (4096 + 4096 + 1000) * sizeof(data_type_w);
    unsigned int fc_8_out_size    = (1000)*sizeof(data_type_o);
    unsigned int out_1_size       = (96*55*55) * sizeof(data_type_o);
    unsigned int out_2_size       = (96*55*55) * sizeof(data_type_o);

    cout<< "Assign memory space to different ports ... ... ... ..." << endl;

    // assign memory space to different ports
    data_type   *in_data_mem_port     = (data_type*)mem_alloc(in_data_mem_size, IN_DATA_ADDR, mem_dev);
    if (in_data_mem_port == NULL){
        printf("False memory allocation of in_data_mem_port\n");
    }
    else {
        printf("in data memory location = 0x%x \n", in_data_mem_port);
    }

    data_type_w *conv_weight_mem_port = (data_type_w*)mem_alloc(conv_weight_size, CONV_W_ADDR, mem_dev);
    if (conv_weight_mem_port == NULL){
        printf("False memory allocation of conv_weight_mem_port\n");
    }
    else {
        printf("conv weight memory location = 0x%x \n", conv_weight_mem_port);
    }
    data_type_w *conv_bias_mem_port   = (data_type_w*)mem_alloc(conv_bias_size, CONV_B_ADDR, mem_dev);
    if (conv_bias_mem_port == NULL){
        printf("False memory allocation of conv_bias_mem_port\n");
    }
    else {
        printf("conv bias memory location = 0x%x \n", conv_bias_mem_port);
    }
    data_type_w *fc_weight_mem_port   = (data_type_w*)mem_alloc(fc_weight_size, FC_W_ADDR, mem_dev);
    if (fc_weight_mem_port == NULL){
        printf("False memory allocation of fc_weight_mem_port\n");
    }
    else {
        printf("fc_weight_mem_port memory location = 0x%x \n", fc_weight_mem_port);
    }
    data_type_w *fc_bias_mem_port     = (data_type_w*)mem_alloc(fc_bias_size, FC_B_ADDR, mem_dev);
    if (fc_bias_mem_port == NULL){
        printf("False memory allocation of fc_bias_mem_port\n");
    }
    else {
        printf("fc_bias_mem_port memory location = 0x%x \n", fc_bias_mem_port);
    }
    data_type_o *fc_8_out_mem_int     = (data_type_o*)mem_alloc(fc_8_out_size, FC_OUT_ADDR, mem_dev);
    if (fc_8_out_mem_int == NULL){
        printf("False memory allocation of fc_8_out_mem_int\n");
    }
    else {
        printf("fc_8_out_mem_int memory location = 0x%x \n", fc_8_out_mem_int);
    }
    data_type_o *temp_out_1       = (data_type_o*)mem_alloc(out_1_size, OUT_1_ADDR, mem_dev);
    if (temp_out_1 == NULL){
        printf("False memory allocation of temp_out_1\n");
    }
    else {
        printf("temp_out_1 memory location = 0x%x \n", temp_out_1);
    }
    data_type_o *temp_out_2       = (data_type_o*)mem_alloc(out_2_size, OUT_1_ADDR, mem_dev);
    if (temp_out_2 == NULL){
        printf("False memory allocation of temp_out_2\n");
    }
    else {
        printf("temp_out_2 memory location = 0x%x \n", temp_out_2);
    }

    cout<< "Initialize output space and temp storage space ... ... ... ..." << endl;
//     set output memory space to 0
    cout << "FC8 mem init\n";
    memset(fc_8_out_mem_int, 0, fc_8_out_size);

//     initial temp storage space to 0
    cout << "out_1 mem init\n";
    memset(temp_out_1, 0, out_1_size);
    cout << "out_2 mem init\n";
    memset(temp_out_2, 0, out_2_size);

    //net weight src *****************************
    const char* weight_src = "../../../net_weights.txt";

    //load mean file *****************************
    ifstream ifs1("../../../net_mean.txt");
    float channel_mean[3] = { 0 };
    string str1;
    string y1 = "[";
    string y2 = "]";
    if (!ifs1) {
        cout << "mean data file not found !" << endl;
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
    ifstream ifs("../../../val.txt");
    string val_name[10];
    float val_class[10];
    string str;

    if (!ifs) {
        cout << "val data file not found !" << endl;
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
	string image_dir = "../../../ILSVRC2012_img_val/ILSVRC2012_val_00000003.JPEG";
	float in_data_3D_channel_swap[3][375][500] = { 0 };
	//input data array
	float in_data_3D[3][227][227] = { 0 };
//	data_type in_data_3D_int[3*227*227] = { 0 };
	int crop_w = 227;
	int crop_h = 227;
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

	resize_image(in_data_3D_channel_swap, h, w, in_data_3D);//in_data after crop

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < crop_h; j++) {
			for (int k = 0; k < crop_w; k++) {
				in_data_3D[i][j][k] = in_data_3D[i][j][k] * 255 - channel_mean[i];
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


    char tan_h = 't';
    char relu = 'r';
    char none = 'i';
    int in_number_conv = 0;  // number of convolutional layer
    int in_number_fc = 0;// number of fully_connected layer
    int in_number_pooling = 0;

    // Prepare weights and bias for convolution layer 1
    float *conv_1_weight2D = (float*)malloc(288 * 11 * 11 * sizeof(float));
    memset(conv_1_weight2D, 0, 288 * 11 * 11 * sizeof(float));
    //float conv_1_bias2D[96] = { 0 };
//    data_type_w          conv_1_bias2D_int[96] = {0};
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
    for(int i = 0; i < 288*11*11; i++){
//        conv_weight_port[conv_weight_num]=conv_1_weight2D_int[i];
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
//        conv_bias_port[conv_bias_num]=conv_1_bias2D_int[i];
        conv_bias_mem_port[conv_bias_num] = (data_type_w)conv_1_bias2D[i];
        conv_bias_num++;
    }
    free(conv_1_bias2D);

//    for(int i = 0; i < 96; i++){
//        conv_1_bias2D_int[i] = (data_type_w)(conv_1_bias2D[i]);
//        cout << conv_1_bias2D_int[i] << "  " ;
//    }
//    cout << endl;

    // Prepare weights and bias for convolution layer 2
    //float conv_2_weight2D[12288*5*5] = { 0 };
//    data_type_w          conv_2_weight2D_int[12288*5*5] = {0};
    //float conv_2_bias2D[256] = { 0 };
//    data_type_w          conv_2_bias2D_int[256] = {0};
    float *conv_2_weight2D = (float*)malloc(12288*5*5 * sizeof(float));
    memset(conv_2_weight2D, 0, 12288 * 5 * 5 * sizeof(float));
    load_weight_conv(
            weight_src,
            conv_2_weight2D,
            weight_bias_record,
            nn_channel_size_conv,
            nn_in_number_conv,
            nn_out_number_conv,
            in_number_conv);
    cout << "Loading conv weight 2 to memory space, starting at: " << conv_weight_num << '\n';
    for(int i = 0; i < 12288*5*5; i++){
//        conv_weight_port[conv_weight_num]=conv_2_weight2D_int[i];
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
//        conv_bias_port[conv_bias_num]=conv_2_bias2D_int[i];
        conv_bias_mem_port[conv_bias_num] = (data_type_w)conv_2_bias2D[i];
        conv_bias_num++;
    }
    free(conv_2_bias2D);

    // Prepare weights and bias for convolution layer 3
    //float conv_3_weight2D[98304*3*3] = { 0 };
//    data_type_w          conv_3_weight2D_int[98304*3*3] = { 0 };//
    //float conv_3_bias2D[384] = { 0 };
//    data_type_w          conv_3_bias2D_int[384] = {0};

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
//        conv_bias_port[conv_bias_num]=conv_3_bias2D_int[i];
        conv_bias_mem_port[conv_bias_num] = (data_type_w)conv_3_bias2D[i];
        conv_bias_num++;
    }
    free(conv_3_bias2D);

//    for(int i = 0; i < 384; i++){
//        conv_3_bias2D_int[i] = (data_type_w)(conv_3_bias2D[i]);
//        cout << conv_3_bias2D_int[i] << "  " ;
//    }
//    cout << endl;

    // Prepare weights and bias for convolution layer 4
    //float conv_4_weight2D[73728*3*3] = { 0 };
//    data_type_w          conv_4_weight2D_int[73728*3*3] = { 0 };//
    //float conv_4_bias2D[384] = { 0 };
//    data_type_w          conv_4_bias2D_int[384] = {0};

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
//        conv_weight_port[conv_weight_num]=conv_4_weight2D_int[i];
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
//        conv_bias_port[conv_bias_num]=conv_4_bias2D_int[i];
        conv_bias_mem_port[conv_bias_num] = (data_type_w)conv_4_bias2D[i];
        conv_bias_num++;
    }
    free(conv_4_bias2D);

//    for(int i = 0; i < 384; i++){
//        conv_4_bias2D_int[i] = (data_type_w)(conv_4_bias2D[i]);
//        cout << conv_4_bias2D_int[i] << "  " ;
//    }
//    cout << endl;

    // Prepare weights and bias for convolution layer 5
    //float conv_5_weight2D[49152*3*3] = { 0 };
//    data_type_w          conv_5_weight2D_int[49152*3*3] = {0};
    //float conv_5_bias2D[256] = { 0 };
//    data_type_w          conv_5_bias2D_int[256] = {0};

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
//        conv_weight_port[conv_weight_num]=conv_5_weight2D_int[i];
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
//        conv_bias_port[conv_bias_num]=conv_5_bias2D_int[i];
        conv_bias_mem_port[conv_bias_num] = (data_type_w) conv_5_bias2D[i];
        conv_bias_num++;
    }
    free(conv_5_bias2D);

    cout<<"Finished loading conv weight into memory! Total: " << conv_weight_num  << "... ... ..."<<endl;
    cout<<"Finished loading conv bias into memory! Total: " << conv_bias_num  << "... ... ..."<<endl;
//    for(int i = 0; i < 256; i++){
//        conv_5_bias2D_int[i] = (data_type_w)(conv_5_bias2D[i]);
//        cout << conv_5_bias2D_int[i] << "  " ;
//    }
//    cout << endl;

    // Prepare weights and bias for fc layer 6
    //float fc_6_weight2D[1048576*6*6] = { 0 };
    //data_type_w   fc_6_weight2D_int[1048576*6*6] = {0};
    //float fc_6_bias2D[4096] = { 0 };
    //data_type_w   fc_6_bias2D_int[4096] = {0};

    /*
    float *fc_6_weight2D = (float*)malloc(1048576*6*6 * sizeof(float));
    memset(fc_6_weight2D, 0, 1048576 * 6 * 6 * sizeof(float));
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
    for(int i = 0; i < 1048576*6*6; i++){
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

    //for(int i = 0; i < 4096; i++){
    //    fc_6_bias2D_int[i] = (data_type_w)(fc_6_bias2D[i]);
    //}


    // Prepare weights and bias for fc layer 7
    //float fc_7_weight2D[16777216*1*1] = { 0 };
    //data_type_w   fc_7_weight2D_int[16777216*1*1] = {0};
    //float fc_7_bias2D[4096] = { 0 };
    //data_type_w   fc_7_bias2D_int[4096] = {0};
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

    //for(int i = 0; i < 4096; i++){
    //    fc_7_bias2D_int[i] = (data_type_w)(fc_7_bias2D[i]);
    //}

    // Prepare weights and bias for fc layer 8
    //float fc_8_weight2D[4096000*1*1] = { 0 };
    //data_type_w   fc_8_weight2D_int[4096000*1*1] = {0};
    //float fc_8_bias2D[1000] = { 0 };
    //data_type_w   fc_8_bias2D_int[1000] = {0};
    float *fc_8_weight2D = (float*)malloc(4096000*1*1 * sizeof(float));
    memset(fc_8_weight2D, 0, 4096000 * 1 * 1 * sizeof(float));
    load_weight_fc(
            weight_src,
            fc_8_weight2D,
            weight_bias_record,
            nn_channel_size_fc,
            nn_in_number_fc,
            nn_out_number_fc,
            in_number_fc);
    cout << "Loading fc weight 8 to memory space, starting at: " << fc_weight_num << '\n';
    for(int i = 0; i < 4096000*1*1; i++){
        fc_weight_mem_port[fc_weight_num]=(data_type_w)fc_8_weight2D[i];
        fc_weight_num++;
    }
    free(fc_8_weight2D);

    float *fc_8_bias2D = (float*)malloc(1000 * sizeof(float));
    memset(fc_8_bias2D, 0, 1000 * sizeof(float));
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
    for(int i = 0; i < 1000; i++){
        fc_bias_mem_port[fc_bias_num]=(data_type_w)fc_8_bias2D[i];
        fc_bias_num++;
    }
    free(fc_8_bias2D);
    */



    //for(int i = 0; i < 1000; i++){
    //    fc_8_bias2D_int[i] = (data_type_w)(fc_8_bias2D[i]);
    //}

//    data_type_w   conv_weight_port[2332704] = {0};
//    data_type_w   conv_bias_port[1376] = {0};
/*
    data_type_w   fc_weight_port[58621952] = {0};
    data_type_w   fc_bias_port[9192] = {0};
*/
    /*
    cout<< "Loading conv weights into memory space ... ..." << endl;
    int conv_weight_num=0;
    for(int i = 0; i < 288*11*11; i++){
//        conv_weight_port[conv_weight_num]=conv_1_weight2D_int[i];
        conv_weight_mem_port[conv_weight_num] = (data_type_w)conv_1_weight2D[i];
        conv_weight_num++;
    }

    for(int i = 0; i < 12288*5*5; i++){
//        conv_weight_port[conv_weight_num]=conv_2_weight2D_int[i];
        conv_weight_mem_port[conv_weight_num] = (data_type_w)conv_2_weight2D[i];
        conv_weight_num++;
    }

    for(int i = 0; i < 98304*3*3; i++){
//        conv_weight_port[conv_weight_num]=conv_3_weight2D_int[i];
        conv_weight_mem_port[conv_weight_num] = (data_type_w)conv_3_weight2D[i];
        conv_weight_num++;
    }
    for(int i = 0; i < 73728*3*3; i++){
//        conv_weight_port[conv_weight_num]=conv_4_weight2D_int[i];
        conv_weight_mem_port[conv_weight_num] = (data_type_w)conv_4_weight2D[i];
        conv_weight_num++;
    }
    for(int i = 0; i < 49152*3*3; i++){
//        conv_weight_port[conv_weight_num]=conv_5_weight2D_int[i];
        conv_weight_mem_port[conv_weight_num] = (data_type_w)conv_5_weight2D[i];
        conv_weight_num++;
    }

    cout<<"Finished loading conv weights into memory ... ... ..."<<endl;
    */
    /*
    cout<< "Loading conv bias into memory space ... ..." << endl;

    int conv_bias_num=0;
    for(int i = 0; i < 96; i++){
//        conv_bias_port[conv_bias_num]=conv_1_bias2D_int[i];
        conv_bias_mem_port[conv_bias_num] = (data_type_w)conv_1_bias2D[i];
        conv_bias_num++;
    }
    for(int i = 0; i < 256; i++){
//        conv_bias_port[conv_bias_num]=conv_2_bias2D_int[i];
        conv_bias_mem_port[conv_bias_num] = (data_type_w)conv_2_bias2D[i];
        conv_bias_num++;
    }
    for(int i = 0; i < 384; i++){
//        conv_bias_port[conv_bias_num]=conv_3_bias2D_int[i];
        conv_bias_mem_port[conv_bias_num] = (data_type_w)conv_3_bias2D[i];
        conv_bias_num++;
    }
    for(int i = 0; i < 384; i++){
//        conv_bias_port[conv_bias_num]=conv_4_bias2D_int[i];
        conv_bias_mem_port[conv_bias_num] = (data_type_w)conv_4_bias2D[i];
        conv_bias_num++;
    }
    for(int i = 0; i < 256; i++) {
//        conv_bias_port[conv_bias_num]=conv_5_bias2D_int[i];
        conv_bias_mem_port[conv_bias_num] = (data_type_w) conv_5_bias2D[i];
        conv_bias_num++;
    }

    cout<<"Finished loading conv bias into memory ... ... ..."<<endl;
    */
    /*
    int fc_weight_num=0;
    for(int i = 0; i < 1048576*6*6; i++){
        fc_weight_mem_port[fc_weight_num]=fc_6_weight2D_int[i];
        fc_weight_num++;
    }
    for(int i = 0; i < 16777216*1*1; i++){
        fc_weight_mem_port[fc_weight_num]=fc_7_weight2D_int[i];
        fc_weight_num++;
    }
    for(int i = 0; i < 4096000*1*1; i++){
        fc_weight_mem_port[fc_weight_num]=fc_8_weight2D_int[i];
        fc_weight_num++;
    }

    int fc_bias_num=0;
    for(int i = 0; i < 4096; i++){
        fc_bias_mem_port[fc_bias_num]=fc_6_bias2D_int[i];
        fc_bias_num++;
    }
    for(int i = 0; i < 4096; i++){
        fc_bias_mem_port[fc_bias_num]=fc_7_bias2D_int[i];
        fc_bias_num++;
    }
    for(int i = 0; i < 1000; i++){
        fc_bias_mem_port[fc_bias_num]=fc_8_bias2D_int[i];
        fc_bias_num++;
    }
    */
//    data_type_o output_1[96*55*55] = { 0 };
//    data_type_o output_2[96*55*55] = { 0 };
    float fc_8_out[1000*1*1];

    // initial accelerator IP
    XInference_net inference_net;
    int IP_status;
    IP_status = XInference_net_Initialize(&inference_net, "inference_net");
    if(IP_status != XST_SUCCESS){
        printf("Initialization failed %d\r\n", IP_status);
        return;
    }
    else {
        printf("IP Initialized !!!\n");
    }

    cout<< "Copy data to memory ... ... ... ..." << endl;

    // copy input data to PS-DDR
//    memcpy(in_data_mem_port, in_data_3D_int, in_data_mem_size);
//    cout<< "Copied input data to memory loaction = "<< std::hex <<IN_DATA_ADDR << std::oct <<" lenght = "<<in_data_mem_size <<endl;
//    memcpy(conv_weight_mem_port, conv_weight_port, conv_weight_size);
//    cout<< "Copied conv weights to memory location = "<< std::hex <<CONV_W_ADDR << std::oct <<" length = "<<conv_weight_size <<endl;
//    memcpy(conv_bias_mem_port, conv_bias_port, conv_bias_size);
//    cout<< "Copied conv bias to memory location = "<< std::hex<<CONV_B_ADDR << std::oct <<" length = "<<conv_bias_size <<endl;
//    memcpy(fc_weight_mem_port, fc_weight_port, fc_weight_size);
//    cout<< "Copied fc weights to memory location = "<< std::hex <<FC_W_ADDR << std::oct <<"  length = "<<fc_weight_size <<endl;
//    memcpy(fc_bias_mem_port, fc_bias_port, fc_bias_size);
//    cout<< "Copied fc bias to memory location = "<< std::hex <<FC_B_ADDR << std::oct <<"  length = "<<fc_bias_size <<endl;

    u32 activation_type;
    XInference_net_Set_activation_type(&inference_net, 'r');
    activation_type = XInference_net_Get_activation_type(&inference_net);
    cout<< "Read back activation type -----> " << activation_type << endl;

    cout<< "Starting inference process ... ... ... ..." << endl;

    // Start the IP execution
    XInference_net_Start(&inference_net);
    while(!XInference_net_IsDone(&inference_net));

    cout<< "Finished inference process ... ... ... ..." << endl;

    for(int i=0;i<1000;i++){
		fc_8_out[i]=(float)(fc_8_out_mem_int[i]);
	}

    for (int i = 0; i < 10; i++)
        cout << "At " << i << ", output1_tmp=" << temp_out_1 << '\n';

    softmax(fc_8_out,1000);
	predict(fc_8_out,1000);

    cout<< "Finished output & free memory ... ... ... ..." << endl;
    /*
    mem_free((void*)in_data_mem_port, in_data_mem_size, IN_DATA_ADDR);
    mem_free((void*)conv_weight_mem_port, conv_weight_size, CONV_W_ADDR);
    mem_free((void*)conv_bias_mem_port, conv_bias_size, CONV_B_ADDR);
    mem_free((void*)fc_weight_mem_port, fc_weight_size, FC_W_ADDR);
    mem_free((void*)fc_bias_mem_port, fc_bias_size, FC_B_ADDR);
    mem_free((void*)fc_8_out_mem_int, fc_8_out_size, FC_OUT_ADDR);
    mem_free((void*)temp_out_1, out_1_size, OUT_1_ADDR);
    mem_free((void*)temp_out_2, out_2_size, OUT_2_ADDR);
    */
    close(mem_dev);
}
