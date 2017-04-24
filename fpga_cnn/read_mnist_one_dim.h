//Read images and labels data in mnist 

#ifndef _READ_MNIST_H_
#define _READ_MNIST_H_
 
#include <assert.h>  
#include <time.h>  
#include <iostream>  
#include <fstream>  
#include <numeric>
#include <stdio.h>
#include <algorithm>
#include <string>  
#include <ap_fixed.h>
//#include "../fpga_cnn/data_type.h"

#define num_patterns_train_CNN      60000 //mnist训练模式对数(总数),10个数字为一对
#define num_patterns_test_CNN       10000 //mnist测试模式对数(总数),10个数字为一对  
#define width_image_input_CNN       32 //归一化图像宽  
#define height_image_input_CNN      32 //归一化图像高  
#define num_map_output_CNN      10 //输出层map个数  

//template<int size>
void init_variable(data_type val_array[], data_type value, int len)
{
	printf("begin_init!!!!!!!!\n");
	for (int i = 0; i < num_patterns_test_CNN; i++) {
		for (int j = 0; j < height_image_input_CNN; j++) {
			for (int k = 0; k < width_image_input_CNN; k++) {
				val_array[i*height_image_input_CNN*width_image_input_CNN+j*width_image_input_CNN +k] = value;
			}
		}
	}
	printf("end_init!!!!!!!!\n");
}

static int reverseInt(int i)////英特尔处理器和其他低端机用户必须翻转头字节
{
	unsigned char ch1, ch2, ch3, ch4;
	ch1 = i & 255;
	ch2 = (i >> 8) & 255;
	ch3 = (i >> 16) & 255;
	ch4 = (i >> 24) & 255;
	return((int)ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
}

/*************************读取MNIST文件的images数据**********************/
//readMnistImages(filename_train_images, data_input_train, num_patterns_train_CNN);
/*	filename  文件名（路径）											*/
/*	data_dst  存放Mnist data的四维数组									*/
/*	num_image 读取的mnist images数量									*/
/*************************读取MNIST文件参数******************************/
/*	images train number_of_images = 60000								*/
/*	images test number_of_images = 10000								*/
/*	images magic_number = 2051											*/
/*	images n_rows = 28													*/
/*	images n_cols = 28													*/
/*	labels train number_of_labels = 60000								*/
/*	labels test number_of_labels = 10000								*/
/*	labels magic_number = 2049											*/

//template<int size>
static void readMnistImages(const char* filename, data_type data_dst[], int num_image)
//static void readMnistImages(std::string filename, float data_dst[][1][size][size], int num_image)
{
	const int width_src_image = 28;
	const int height_src_image = 28;
	const int x_padding = 2;
	const int y_padding = 2;
	const data_type scale_min = -1;
	const data_type scale_max = 1;

	std::ifstream file(filename, std::ios::binary);//二进制读取Mnist数据
//	ifstream file(filename, ios::binary);
	assert(file.is_open());

	int magic_number = 0;
	int number_of_images = 0;
	int n_rows = 0;
	int n_cols = 0;
	file.read((char*)&magic_number, sizeof(magic_number));//读取数据，读取到magic_number地址所在内存
														  //转换MNIST参数
	magic_number = reverseInt(magic_number);
	file.read((char*)&number_of_images, sizeof(number_of_images));
	number_of_images = reverseInt(number_of_images);
	assert(number_of_images == num_image);
	file.read((char*)&n_rows, sizeof(n_rows));
	n_rows = reverseInt(n_rows);
	file.read((char*)&n_cols, sizeof(n_cols));
	n_cols = reverseInt(n_cols);
	assert(n_rows == height_src_image && n_cols == width_src_image);

	//int size_single_image = width_image_input_CNN * height_image_input_CNN;//image data 
	printf("begin_read_Images!!!!!!!!\n");
	for (int i = 0; i < number_of_images; ++i) {
		//int addr = size_single_image * i;//每个Images的地址
		for (int r = 0; r < n_rows; ++r) {//rows
			for (int c = 0; c < n_cols; ++c) {//cols
				unsigned char temp = 0;//buffer 像素char类型
				file.read((char*)&temp, sizeof(temp));
				//data_dst[addr + width_image_input_CNN * (r + y_padding) + c + x_padding] = (((int)temp) / 255.0) * (scale_max - scale_min) + scale_min;//归一化
				//mnist_data[addr + width_image_input_CNN * (r + y_padding) + c + x_padding] = (((int)temp) / 255.0) * (scale_max - scale_min) + scale_min;
				data_dst[i*width_image_input_CNN * height_image_input_CNN +(r + y_padding)*width_image_input_CNN +c + x_padding] = (data_type)((int)temp);//归一化
			}
		}
	}
	printf("end_read_Images!!!!!!!!\n");
}

//template<int size>
static void readMnistLabels(const char* filename, data_type data_dst[], int num_image)
//static void readMnistLabels(std::string filename, float data_dst[][size], int num_image)
{
	const data_type scale_max = 1;

	std::ifstream file(filename, std::ios::binary);
//	ifstream file(filename, ios::binary);
    assert(file.is_open());

	int magic_number = 0;
	int number_of_images = 0;
	file.read((char*)&magic_number, sizeof(magic_number));
	magic_number = reverseInt(magic_number);
	file.read((char*)&number_of_images, sizeof(number_of_images));
	number_of_images = reverseInt(number_of_images);
	assert(number_of_images == num_image);
	printf("begin_read_Label!!!!!!!!\n");
	for (int i = 0; i < number_of_images; ++i) {
		unsigned char temp = 0;
		file.read((char*)&temp, sizeof(temp));
		//data_dst[i * num_map_output_CNN + temp] = scale_max;//num_map_output_CNN 输出层个数,每10个输入只有该label为1，其余为0
		data_dst[i*10+temp] = scale_max;//归一化
		//mnist_label[i * num_map_output_CNN + temp] = scale_max;
	}
	printf("end_read_Label!!!!!!!!\n");
}

//template<int size,int label_size>
void getSrcData(data_type mnist_train_data[], data_type mnist_train_label[],
	data_type mnist_test_data[], data_type mnist_test_label[])
{
	/*初始化mnist数据矩阵*/
	//int train_data_length = width_image_input_CNN * width_image_input_CNN * num_patterns_train_CNN;// training data length
	//init_variable(mnist_train_data, -1.0, train_data_length);//init the array with -1
	//int train_label_length = num_map_output_CNN * num_patterns_train_CNN;
	
	int test_data_length = width_image_input_CNN * height_image_input_CNN * num_patterns_test_CNN;
	init_variable(mnist_test_data, (data_type)0.0, test_data_length);//init the array with -1
	int test_label_length = num_map_output_CNN * num_patterns_test_CNN;
  
	/*
	std::string filename_train_images = ".\\data\\train-images.idx3-ubyte";
	std::string filename_train_labels = ".\\data\\train-labels.idx1-ubyte";
	readMnistImages(filename_train_images, mnist_train_data, num_patterns_train_CNN, mnist_train_data);
	readMnistLabels(filename_train_labels, mnist_train_label, num_patterns_train_CNN, mnist_train_label);
	*/
//	std::string filename_test_images = "../example/data/t10k-images.idx3-ubyte";
//	std::string filename_test_labels = "../example/data/t10k-labels.idx1-ubyte";
    const char* filename_test_images = "data/t10k-images-idx3-ubyte";
	const char* filename_test_labels = "data/t10k-labels-idx1-ubyte";
	readMnistImages(filename_test_images, mnist_test_data, num_patterns_test_CNN);
	readMnistLabels(filename_test_labels, mnist_test_label, num_patterns_test_CNN);

	//ofstream image_a;
	//image_a.open("test_set_image_data.txt", ios::app);
	//image_a << "test set image data ..................." << endl;
	////for (int i = 0; i < test_data_length; i++) {
	////	if (i/(width_image_input_CNN * height_image_input_CNN) != (i + 1)/(width_image_input_CNN * height_image_input_CNN)) {//the same image
	////		image_a << mnist_test_data[i] << " ";
	////		image_a << endl;
	////		image_a << endl;
	////	}
	////	else {//different image
	////		if (i/ width_image_input_CNN==(i+1)/ width_image_input_CNN) {//the same row
	////			image_a << mnist_test_data[i] << " ";
	////		}
	////		else {
	////			image_a << mnist_test_data[i] << " " << endl;
	////		}
	////	}
	////}
	//for (int i = 0; i < num_patterns_test_CNN; i++) {
	//	for (int j = 0; j < height_image_input_CNN; j++) {
	//		for (int k = 0; k < width_image_input_CNN; k++) {
	//			image_a << mnist_test_data[i][0][j][k] << " ";
	//		}
	//		image_a << endl;
	//	}
	//	image_a << endl;
	//}
	//image_a << endl;
	//image_a.close();

	ofstream label_a;
	label_a.open("test_set_labels.txt", ios::app);
	label_a << "test set labels ..................." << endl;
	//for (int i = 0; i < test_label_length; i++) {
	//	if (i / num_map_output_CNN == (i + 1) / num_map_output_CNN) {//the same label list
	//		label_a << mnist_test_label[i] << " ";
	//	}
	//	else {
	//		label_a << mnist_test_label[i] << " " << endl;
	//		label_a << endl;
	//	}
	//}
	for (int i = 0; i < num_patterns_test_CNN; i++) {
		for (int j = 0; j < num_map_output_CNN; j++) {
			label_a << mnist_test_label[i*num_map_output_CNN+j] << " ";
		}
		label_a << endl;
		label_a << endl;
	}
	label_a << endl;
	label_a.close();
}
#endif