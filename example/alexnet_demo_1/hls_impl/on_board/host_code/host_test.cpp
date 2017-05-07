// host processor functions, read data from files to PS-DDR and starts computaiton
// edited by Yao Chen on 6 May, 2017
#include <iostream>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <ap_fixed.h>
#include "config.h"

#include "../../fpga_cnn/data_type.h"
#include "../../fpga_cnn/image_converter.h"
#include "../../fpga_cnn/weight_bias_one_dim.h"
#include "../../fpga_cnn/read_mnist_one_dim.h"
#include "../../fpga_cnn/softmax_one_dim.h"
#include "../../fpga_cnn/predict_one_dim.h"
#include "../../fpga_cnn/accuracy_one_dim.h"

#include "../drivers/xinference_net.h"
#include "../drivers/xinferecne_net_hw.h"

#include <sys/time.h>

#define __linux__

#define BASE_ADDRESS_CONV_WEIGHT 0x1a000000
#define BASE_ADDRESS_CONV_BIAS   0x1b000000
#define BASE_ADDRESS_FC_WEIGHT   0x1c000000
#define BASE_ADDRESS_FC_BIAS     0x1d000000

#define CRTL_ADDR    0x43C00000
#define IN_DATA_ADDR 0x2A000000
#define CONV_W_ADDR  0x2B000000
#define CONV_B_ADDR  0x2C000000
#define FC_W_ADDR    0x10000000
#define FC_B_ADDR    0x2D000000
#define FC_OUT_ADDR  0x2E000000
#define OUT_1_ADDR   0X2F000000
#define OUT_2_ADDR   0X30000000

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
    int mem_dev = open("/dev/mem", O_RDWR | O_SYNC);
    if(mem_dev == -1)
    {
        // Error: can not ger mem device
        printf("Error when opening /dev/mem\n");
        return;
    }

    //net weight src *****************************
    const char* weight_src = "net_weights.txt";

    //load mean file *****************************
    ifstream ifs1("net_mean.txt");
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
    ifstream ifs("val.txt");
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
#if _KERNEL_DEBUG_
#if _HLS_MODE_
	string image_dir = "ILSVRC2012_val_00000003.JPEG";//validation dataset dir
#else
	string image_dir = "./ILSVRC2012_img_val/ILSVRC2012_val_00000003.JPEG";
#endif
	float in_data_3D_channel_swap[3][375][500] = { 0 };
	//input data array
	float in_data_3D[3][227][227] = { 0 };
	data_type in_data_3D_int[3*227*227] = { 0 };
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

	cout << "testing point 2" << endl;

    int in_data_size=0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < crop_h; j++) {
			for (int k = 0; k < crop_w; k++) {
				in_data_3D_int[in_data_size] = (data_type)in_data_3D[i][j][k];
				in_data_size++;
			}
		}
	}

#endif

    char tan_h = 't';
    char relu = 'r';
    char none = 'i';
    int in_number_conv = 0;  // number of convolutional layer
    int in_number_fc = 0;// number of fully_connected layer
    int in_number_pooling = 0;

    // Prepare weights and bias for convolution layer 1
    float conv_1_weight2D[288*11*11] = { 0 };
    data_type_w          conv_1_weight2D_int[288*11*11] = {0};
    float conv_1_bias2D[96] = { 0 };
    data_type_w          conv_1_bias2D_int[96] = {0};
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

    for(int i = 0; i < 96; i++){
        conv_1_bias2D_int[i] = (data_type_w)(conv_1_bias2D[i]);
        cout << conv_1_bias2D_int[i] << "  " ;
    }
    cout << endl;

    // Prepare weights and bias for convolution layer 2
    float conv_2_weight2D[12288*5*5] = { 0 };
    data_type_w          conv_2_weight2D_int[12288*5*5] = {0};
    float conv_2_bias2D[256] = { 0 };
    data_type_w          conv_2_bias2D_int[256] = {0};

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

    for(int i = 0; i < 256; i++){
        conv_2_bias2D_int[i] = (data_type_w)(conv_2_bias2D[i]);
        cout << conv_2_bias2D_int[i] << "  " ;
    }
    cout << endl;

    // Prepare weights and bias for convolution layer 3
    float conv_3_weight2D[98304*3*3] = { 0 };
    data_type_w          conv_3_weight2D_int[98304*3*3] = { 0 };//
    float conv_3_bias2D[384] = { 0 };
    data_type_w          conv_3_bias2D_int[384] = {0};

    load_weight_conv(
            weight_src,
            conv_3_weight2D,
            weight_bias_record,
            nn_channel_size_conv,
            nn_in_number_conv,
            nn_out_number_conv,
            in_number_conv);
    load_bias_conv(
            weight_src,
            conv_3_bias2D,
            weight_bias_record,
            nn_channel_size_conv,
            nn_in_number_conv,
            nn_out_number_conv,
            in_number_conv);
    in_number_conv++;

    for(int i = 0; i < 384; i++){
        conv_3_bias2D_int[i] = (data_type_w)(conv_3_bias2D[i]);
        cout << conv_3_bias2D_int[i] << "  " ;
    }
    cout << endl;

    // Prepare weights and bias for convolution layer 4
    float conv_4_weight2D[73728*3*3] = { 0 };
    data_type_w          conv_4_weight2D_int[73728*3*3] = { 0 };//
    float conv_4_bias2D[384] = { 0 };
    data_type_w          conv_4_bias2D_int[384] = {0};

    load_weight_conv(
            weight_src,
            conv_4_weight2D,
            weight_bias_record,
            nn_channel_size_conv,
            nn_in_number_conv,
            nn_out_number_conv,
            in_number_conv);
    load_bias_conv(
            weight_src,
            conv_4_bias2D,
            weight_bias_record,
            nn_channel_size_conv,
            nn_in_number_conv,
            nn_out_number_conv,
            in_number_conv);
    in_number_conv++;

    for(int i = 0; i < 384; i++){
        conv_4_bias2D_int[i] = (data_type_w)(conv_4_bias2D[i]);
        cout << conv_4_bias2D_int[i] << "  " ;
    }
    cout << endl;

    // Prepare weights and bias for convolution layer 5
    float conv_5_weight2D[49152*3*3] = { 0 };
    data_type_w          conv_5_weight2D_int[49152*3*3] = {0};
    float conv_5_bias2D[256] = { 0 };
    data_type_w          conv_5_bias2D_int[256] = {0};

    load_weight_conv(
            weight_src,
            conv_5_weight2D,
            weight_bias_record,
            nn_channel_size_conv,
            nn_in_number_conv,
            nn_out_number_conv,
            in_number_conv);
    load_bias_conv(
            weight_src,
            conv_5_bias2D,
            weight_bias_record,
            nn_channel_size_conv,
            nn_in_number_conv,
            nn_out_number_conv,
            in_number_conv);
    in_number_conv++;

    for(int i = 0; i < 256; i++){
        conv_5_bias2D_int[i] = (data_type_w)(conv_5_bias2D[i]);
        cout << conv_5_bias2D_int[i] << "  " ;
    }
    cout << endl;

    // Prepare weights and bias for fc layer 6
    float fc_6_weight2D[1048576*6*6] = { 0 };
    data_type_w   fc_6_weight2D_int[1048576*6*6] = {0};
    float fc_6_bias2D[4096] = { 0 };
    data_type_w   fc_6_bias2D_int[4096] = {0};
    load_weight_fc(
            weight_src,
            fc_6_weight2D,
            weight_bias_record,
            nn_channel_size_fc,
            nn_in_number_fc,
            nn_out_number_fc,
            in_number_fc);
    load_bias_fc(
            weight_src,
            fc_6_bias2D,
            weight_bias_record,
            nn_channel_size_fc,
            nn_in_number_fc,
            nn_out_number_fc,
            in_number_fc);
    in_number_fc++;

    for(int i = 0; i < 4096; i++){
        fc_6_bias2D_int[i] = (data_type_w)(fc_6_bias2D[i]);
    }

    // Prepare weights and bias for fc layer 7
    float fc_7_weight2D[16777216*1*1] = { 0 };
    data_type_w   fc_7_weight2D_int[16777216*1*1] = {0};
    float fc_7_bias2D[4096] = { 0 };
    data_type_w   fc_7_bias2D_int[4096] = {0};
    load_weight_fc(
            weight_src,
            fc_7_weight2D,
            weight_bias_record,
            nn_channel_size_fc,
            nn_in_number_fc,
            nn_out_number_fc,
            in_number_fc);
    load_bias_fc(
            weight_src,
            fc_7_bias2D,
            weight_bias_record,
            nn_channel_size_fc,
            nn_in_number_fc,
            nn_out_number_fc,
            in_number_fc);
    in_number_fc++;

    for(int i = 0; i < 4096; i++){
        fc_7_bias2D_int[i] = (data_type_w)(fc_7_bias2D[i]);
    }

    // Prepare weights and bias for fc layer 8
    float fc_8_weight2D[4096000*1*1] = { 0 };
    data_type_w   fc_8_weight2D_int[4096000*1*1] = {0};
    float fc_8_bias2D[1000] = { 0 };
    data_type_w   fc_8_bias2D_int[1000] = {0};
    load_weight_fc(
            weight_src,
            fc_8_weight2D,
            weight_bias_record,
            nn_channel_size_fc,
            nn_in_number_fc,
            nn_out_number_fc,
            in_number_fc);
    load_bias_fc(
            weight_src,
            fc_8_bias2D,
            weight_bias_record,
            nn_channel_size_fc,
            nn_in_number_fc,
            nn_out_number_fc,
            in_number_fc);
    in_number_fc++;

    for(int i = 0; i < 1000; i++){
        fc_8_bias2D_int[i] = (data_type_w)(fc_8_bias2D[i]);
    }

    data_type_w   conv_weight_port[2332704] = {0};
    data_type_w   conv_bias_port[1376] = {0};
    data_type_w   fc_weight_port[58621952] = {0};
    data_type_w   fc_bias_port[9192] = {0};

    int conv_weight_num=0;
    for(int i = 0; i < 288*11*11; i++){
        conv_weight_port[conv_weight_num]=conv_1_weight2D_int[i];
        conv_weight_num++;
    }
    for(int i = 0; i < 12288*5*5; i++){
        conv_weight_port[conv_weight_num]=conv_2_weight2D_int[i];
        conv_weight_num++;
    }
    for(int i = 0; i < 98304*3*3; i++){
        conv_weight_port[conv_weight_num]=conv_3_weight2D_int[i];
        conv_weight_num++;
    }
    for(int i = 0; i < 73728*3*3; i++){
        conv_weight_port[conv_weight_num]=conv_4_weight2D_int[i];
        conv_weight_num++;
    }
    for(int i = 0; i < 49152*3*3; i++){
        conv_weight_port[conv_weight_num]=conv_5_weight2D_int[i];
        conv_weight_num++;
    }

    int conv_bias_num=0;
    for(int i = 0; i < 96; i++){
        conv_bias_port[conv_bias_num]=conv_1_bias2D_int[i];
        conv_bias_num++;
    }
    for(int i = 0; i < 256; i++){
        conv_bias_port[conv_bias_num]=conv_2_bias2D_int[i];
        conv_bias_num++;
    }
    for(int i = 0; i < 384; i++){
        conv_bias_port[conv_bias_num]=conv_3_bias2D_int[i];
        conv_bias_num++;
    }
    for(int i = 0; i < 384; i++){
        conv_bias_port[conv_bias_num]=conv_4_bias2D_int[i];
        conv_bias_num++;
    }
    for(int i = 0; i < 256; i++){
        conv_bias_port[conv_bias_num]=conv_5_bias2D_int[i];
        conv_bias_num++;
    }

    int fc_weight_num=0;
    for(int i = 0; i < 1048576*6*6; i++){
        fc_weight_port[fc_weight_num]=fc_6_weight2D_int[i];
        fc_weight_num++;
    }
    for(int i = 0; i < 16777216*1*1; i++){
        fc_weight_port[fc_weight_num]=fc_7_weight2D_int[i];
        fc_weight_num++;
    }
    for(int i = 0; i < 4096000*1*1; i++){
        fc_weight_port[fc_weight_num]=fc_8_weight2D_int[i];
        fc_weight_num++;
    }

    int fc_bias_num=0;
    for(int i = 0; i < 4096; i++){
        fc_bias_port[fc_bias_num]=fc_6_bias2D_int[i];
        fc_bias_num++;
    }
    for(int i = 0; i < 4096; i++){
        fc_bias_port[fc_bias_num]=fc_7_bias2D_int[i];
        fc_bias_num++;
    }
    for(int i = 0; i < 1000; i++){
        fc_bias_port[fc_bias_num]=fc_8_bias2D_int[i];
        fc_bias_num++;
    }

    data_type_o output_1[96*55*55] = { 0 };
    data_type_o output_2[96*55*55] = { 0 };

    float fc_8_out[1000*1*1] = { 0 };
	data_type   fc_8_out_int[1000*1*1];

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

    cout<< "Calculating memory space ... ... ... ..." << endl;

    // data size calculation
    unsigned int in_data_size     = (3*227*227) * sizeof(data_type);
    unsigned int conv_weight_size = (unsigned int)conv_weight_num * sizeof(data_type_w);
    unsigned int conv_bias_size   = (unsigned int)conv_bias_num * sizeof(data_type_w);
    unsigned int fc_weight_size   = (unsigned int)fc_weight_num * sizeof(data_type_w);
    unsigned int fc_bias_size     = (unsigned int)fc_bias_num * sizeof(data_type_w);
    unsigned int out_1_size       = (96*55*55) * sizeof(data_type_o);
    unsigned int out_2_size       = (96*55*55) * sizeof(data_type_o);

    cout<< "Assign memory space to different ports ... ... ... ..." << endl;

    // assign memory space to different ports
    data_type   *in_data_port     = (data_type*)mem_alloc(in_data_size, IN_DATA_ADDR, mem_dev);
    data_type_w *conv_weight_port = (data_type_w*)mem_alloc(conv_weight_size, CONV_W_ADDR, mem_dev);
    data_type_w *conv_bias_port   = (data_type_w*)mem_alloc(conv_bias_size, CONV_B_ADDR, mem_dev);
    data_type_w *fc_weight_port   = (data_type_w*)mem_alloc(fc_weight_size, FC_W_ADDR, mem_dev);
    data_type_w *fc_bias_port     = (data_type_w*)mem_alloc(fc_bias_size, FC_B_ADDR, mem_dev);
    data_type_o *fc_8_out_int     = (data_type_o*)mem_alloc(1000, FC_OUT_ADDR, mem_dev);
    data_type_o *temp_out_1       = (data_type_o*)mem_alloc(96*55*55, OUT_1_ADDR, mem_dev);
    data_type_o *temp_out_2       = (data_type_o*)mem_alloc(96*55*55, OUT_1_ADDR, mem_dev);

    cout<< "Copy data to memory ... ... ... ..." << endl;

    // copy input data to PS-DDR
    memcpy(in_data_port, in_data_3D_int, 3*227*227);
    memcpy(conv_weight_port, conv_weight_port, conv_weight_num);
    memcpy(conv_bias_port, conv_bias_port, conv_bias_num);
    memcpy(fc_weight_port, fc_weight_port, fc_weight_num);
    memcpy(fc_bias_port, fc_bias_port, fc_bias_num);

    cout<< "Initial output space and temp storage space ... ... ... ..." << endl;

    // set output memory space to 0
    memset(fc_8_out_int, 0, 1000);

    // initial temp storage space to 0
    memset(temp_out_1, 0, 96*55*55);
    memset(temp_out_2, 0, 96*55*55);

    cout<< "Starting inference process ... ... ... ..." << endl;

    // Start the IP execution
    XInference_net_Start(&inference_net);
    while(!XInference_net_IsDone(&inference_net));

    cout<< "Finished inference process ... ... ... ..." << endl;

    // copy the execution results out
    memcpy(fc_8_out_int, fc_8_out_int, 1000);
    for(int i=0;i<1000;i++){
		fc_8_out[i]=(float)(fc_8_out_int[i]);
	}

    softmax(fc_8_out,1000);
	predict(fc_8_out,1000);

    cout<< "Finished output & free memory ... ... ... ..." << endl;

    mem_free((void*)in_data_port, 3*227*227, IN_DATA_ADDR);
    mem_free((void*)conv_weight_port, conv_weight_num, CONV_W_ADDR);
    mem_free((void*)conv_bias_port, conv_bias_num, CONV_B_ADDR);
    mem_free((void*)fc_weight_port, fc_weight_num, FC_W_ADDR);
    mem_free((void*)fc_bias_port, fc_bias_num, FC_B_ADDR);
    mem_free((void*)fc_8_out_int, 1000, FC_OUT_ADDR);
    mem_free((void*)temp_out_1, 96*55*55, OUT_1_ADDR);
    mem_free((void*)temp_out_2, 96*55*55, OUT_2_ADDR);
    close(mem_dev);

}
