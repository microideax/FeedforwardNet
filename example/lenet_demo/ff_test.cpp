//This is the main function of a LeNet-5 model based application.
//Application description: LeNet-5 image recognition with std image.
//Using stb_image instead of OpenCV to eliminate library dependency.

#include <iostream>
#include <cstring>
//#include <cmath>
//#include <vector>
#include <fstream>
#include <algorithm>
//#include <iterator>
//#include <cstring>
//#include <cstdlib>
#include <time.h>
#include "../fpga_cnn/config.h"
#include "../fpga_cnn/data_type.h"
#include "../fpga_cnn/image_converter.h"
#include "../fpga_cnn/weight_bias.h"
#include "../fpga_cnn/construct_net.h"
//#include "../fpga_cnn/conv_layer.h"
//#include "../fpga_cnn/data_quantize.h"
#include "../fpga_cnn/read_mnist.h"
#include "../fpga_cnn/softmax.h"
#include "../fpga_cnn/predict.h"
#include "../fpga_cnn/accuracy.h"
#include "../fpga_cnn/pow_function.h"
#include "../fpga_cnn/resize_image.h"
//#include "../fpga_cnn/set_mean.h"

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
	//load mean file *****************************
	ifstream ifs1("mean_alexnet.txt");
	float channel_mean[3] = { 0 };
	string str1;
	string y1 = "[";
	string y2 = "]";

	if (!ifs1) {
		cout << "weight file not found !" << endl;
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
	string val_name_class[10][2];
	string str;

	if (!ifs) {
		cout << "weight file not found !" << endl;
	}
	int num = 0;
	while (ifs >> str&&num<20) {//num:10 pair (image_name,image_class)
		if (num % 2 == 0) {//image_name
			val_name_class[num / 2][0] = str;
		}
		else {//image_class
			val_name_class[num / 2][1] = str;
		}
		num++;
	}
	ifs.close();

	//load val image file *****************************
#if _KERNEL_DEBUG_
	string image_dir = "C:/Users/Administrator/Desktop/ILSVRC2012_img_val/ILSVRC2012_val_00000001.JPEG";//validation dataset dir
	float in_data_3D_channel_swap[3][375][500] = { 0 };
	//input data array
	float in_data_3D[3][227][227] = { 0 };
	int w;
	int h;
	int channels;
	int size;
	const unsigned char * data = loadfile(image_dir, size);
	const unsigned char * image_orig = stbi_load_from_memory(data, size, &w, &h, &channels, 3);
	//STBI_default = 0, // only used for req_comp 
	//STBI_grey = 1,
	//STBI_grey_alpha = 2,
	//STBI_rgb = 3,
	//STBI_rgb_alpha = 4

	/*ofstream image_a;
	image_a.open("image_a.txt", ios::app);
	for (int j = 0; j < h * w * 3; j++) {
	image_a << (float)image_orig[j] << " ";
	if (j%w == w-1) {
	image_a << endl;
	}
	}
	image_a << endl;
	image_a.close();*/

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
		for (int j = 0; j < 227; j++) {
			for (int k = 0; k < 227; k++) {
				in_data_3D[i][j][k] = in_data_3D[i][j][k] * 255 - channel_mean[i];
			}
		}
	}
	ofstream indata;
	indata.open("in_data_crop_mean_1.txt", ios::app);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 227; j++) {
			for (int k = 0; k < 227; k++) {
				indata << in_data_3D[i][j][k] << " ";
			}
			indata << endl;
		}
		indata << endl;
	}
	indata << endl;
	indata.close();
#endif

#if _BATCH_MODE_
	//load val image set file *****************************
	string root_dir = "C:/Users/Administrator/Desktop/ILSVRC2012_img_val/";
	float imagenet_test_data_channel_swap[10][3][1000][900] = { 0 };
	float imagenet_test_data[10][3][227][227] = { 0 };
	for (int image_num = 0; image_num < 10; image_num++) {
		string image_dir = root_dir + val_name_class[image_num][0];
		int w;
		int h;
		int channels;
		int size;
		const unsigned char * data = loadfile(image_dir, size);
		const unsigned char * image_orig = stbi_load_from_memory(data, size, &w, &h, &channels, 3);
		//STBI_default = 0, // only used for req_comp 
		//STBI_grey = 1,
		//STBI_grey_alpha = 2,
		//STBI_rgb = 3,
		//STBI_rgb_alpha = 4

		/*ofstream image_a;
		image_a.open("image_a.txt", ios::app);
		for (int j = 0; j < h * w * 3; j++) {
		image_a << (float)image_orig[j] << " ";
		if (j%w == w-1) {
		image_a << endl;
		}
		}
		image_a << endl;
		image_a.close();*/

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

		resize_image(imagenet_test_data_channel_swap[image_num], h, w, imagenet_test_data[image_num]);//in_data after crop

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 227; j++) {
				for (int k = 0; k < 227; k++) {
					imagenet_test_data[image_num][i][j][k] = imagenet_test_data[image_num][i][j][k] * 255 - channel_mean[i];
				}
			}
		}
		ofstream indata;
		indata.open("in_data_crop_mean.txt", ios::app);
		indata << "image_num: " << image_num << "**********" << endl;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 227; j++) {
				for (int k = 0; k < 227; k++) {
					indata << imagenet_test_data[image_num][i][j][k] << " ";
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

	//std_vec_t data_in;
	//float in_data_1[3][227][227] = { 0 };
	////convert image to data matrix
	//	const std::string filename = ".\\example\\cat.jpg";
	//	convert_image(filename, 0, 1, 227, 227, data_in);
	//	for (int i = 0; i < data_in.size(); i++) {
	//		in_data_1[i/51529][(i % 51529) / 227][(i % 51529) % 227] = data_in[i];
	//	}

#if _BATCH_MODE_
	/*float mnist_train_data[60000][1][32][32];
	float mnist_train_label[60000][10] = { 0 };
	float mnist_test_data[10000][1][32][32];
	float mnist_test_label[10000][10] = { 0 };
	getSrcData(mnist_train_data, mnist_train_label, mnist_test_data, mnist_test_label);
	cout << "getSrcData end!!!!!!!!!!!!!!" << endl;*/
#endif

	// Prepare weights and bias for convolution layer 1
	float conv_1_weight2D[288][11][11] = { 0 };
	float conv_1_bias2D[96] = { 0 };
	load_weight_conv(
		conv_1_weight2D,
		weight_bias_record,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_channel_number_conv,
		in_number_conv);
	load_bias_conv(
		conv_1_bias2D,
		weight_bias_record,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_channel_number_conv,
		in_number_conv);
	in_number_conv++;

	//	for (int i = 0; i < 6; i++) {
	//	for (int j = 0; j < 5; j++) {
	//	for (int k = 0; k < 5; k++) {
	//	 conv_1_weight2D[i][j][k] = roundf(conv_1_weight2D[i][j][k] * 100) / 100.0;
	//	            quantize(-1, 1, conv_1_weight2D[i][j][k]);
	//	}
	//	}
	//	}
	//	for (int i = 0; i<6; i++) {
	//	conv_1_bias2D[i] = roundf(conv_1_bias2D[i] * 100) / 100;
	//	conv_1_bias2D[i] = quantize(-0.2, 0.2, conv_1_bias2D[i]);
	//	}

	// Prepare weights and bias for convolution layer 2
	float conv_2_weight2D[12288][5][5] = { 0 };
	float conv_2_bias2D[256] = { 0 };

	load_weight_conv(
		conv_2_weight2D,
		weight_bias_record,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_channel_number_conv,
		in_number_conv);
	load_bias_conv(
		conv_2_bias2D,
		weight_bias_record,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_channel_number_conv,
		in_number_conv);
	in_number_conv++;

	//	for (int i = 0; i < 96; i++) {
	//	for (int j = 0; j < 5; j++) {
	//	for (int k = 0; k < 5; k++) {
	////	conv_2_weight2D[i][j][k] = roundf(conv_2_weight2D[i][j][k] * 10) / 10.0;
	//        quantize(-0.3, 0.3, conv_2_weight2D[i][j][k]);
	//	}
	//	}
	//	}
	//	for (int i = 0; i<16; i++) {
	//	//	conv_2_bias2D[i] = roundf(conv_2_bias2D[i] * 10) / 10;
	////	conv_2_bias2D[i] = quantize(conv_2_bias2D[i]);
	//	}

	// Prepare weights and bias for convolution layer 3
	float conv_3_weight2D[98304][3][3] = { 0 };
	float conv_3_bias2D[384] = { 0 };

	load_weight_conv(
		conv_3_weight2D,
		weight_bias_record,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_channel_number_conv,
		in_number_conv);
	load_bias_conv(
		conv_3_bias2D,
		weight_bias_record,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_channel_number_conv,
		in_number_conv);
	in_number_conv++;

	// Prepare weights and bias for convolution layer 4
	float conv_4_weight2D[73728][3][3] = { 0 };
	float conv_4_bias2D[384] = { 0 };

	load_weight_conv(
		conv_4_weight2D,
		weight_bias_record,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_channel_number_conv,
		in_number_conv);
	load_bias_conv(
		conv_4_bias2D,
		weight_bias_record,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_channel_number_conv,
		in_number_conv);
	in_number_conv++;

	// Prepare weights and bias for convolution layer 5
	float conv_5_weight2D[49152][3][3] = { 0 };
	float conv_5_bias2D[256] = { 0 };

	load_weight_conv(
		conv_5_weight2D,
		weight_bias_record,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_channel_number_conv,
		in_number_conv);
	load_bias_conv(
		conv_5_bias2D,
		weight_bias_record,
		nn_channel_size_conv,
		nn_in_number_conv,
		nn_channel_number_conv,
		in_number_conv);
	in_number_conv++;

	// Prepare weights and bias for fc layer 6
	float fc_6_weight2D[1048576][6][6] = { 0 };
	float fc_6_bias2D[4096] = { 0 };
	load_weight_fc(
		fc_6_weight2D,
		weight_bias_record,
		nn_channel_size_fc,
		nn_in_number_fc,
		nn_channel_number_fc,
		in_number_fc);
	load_bias_fc(
		fc_6_bias2D,
		weight_bias_record,
		nn_channel_size_fc,
		nn_in_number_fc,
		nn_channel_number_fc,
		in_number_fc);
	in_number_fc++;

	// Prepare weights and bias for fc layer 7
	float fc_7_weight2D[16777216][1][1] = { 0 };
	float fc_7_bias2D[4096] = { 0 };
	load_weight_fc(
		fc_7_weight2D,
		weight_bias_record,
		nn_channel_size_fc,
		nn_in_number_fc,
		nn_channel_number_fc,
		in_number_fc);
	load_bias_fc(
		fc_7_bias2D,
		weight_bias_record,
		nn_channel_size_fc,
		nn_in_number_fc,
		nn_channel_number_fc,
		in_number_fc);
	in_number_fc++;

	// Prepare weights and bias for fc layer 8
	float fc_8_weight2D[4096000][1][1] = { 0 };
	float fc_8_bias2D[1000] = { 0 };
	load_weight_fc(
		fc_8_weight2D,
		weight_bias_record,
		nn_channel_size_fc,
		nn_in_number_fc,
		nn_channel_number_fc,
		in_number_fc);
	load_bias_fc(
		fc_8_bias2D,
		weight_bias_record,
		nn_channel_size_fc,
		nn_in_number_fc,
		nn_channel_number_fc,
		in_number_fc);
	in_number_fc++;

	//	for (int i = 0; i < 160; i++) {
	//	    for (int j = 0; j < 5; j++) {
	//	        for (int k = 0; k < 5; k++) {
	//	            fc_1_weight2D[i][j][k] = roundf(fc_1_weight2D[i][j][k] * 10) / 10.0;
	////	fc_1_weight2D[i][j][k] = quantize(fc_1_weight2D[i][j][k]);
	//	        }
	//	    }
	//	}
	//	for (int i = 0; i<10; i++) {
	//	fc_1_bias2D[i] = roundf(fc_1_bias2D[i] * 10) / 10;
	////	fc_1_bias2D[i] = quantize(fc_1_bias2D[i]);
	//	}
#if _KERNEL_DEBUG_
	float fc_8_out[1000][1][1] = { 0 };
#endif

#if _BATCH_MODE_
	//float fc_1_out_a[10000][10] = { 0 };
	//float fc_1_out_temp[10] = { 0 };
	float fc_1_out_a[10][1000][1][1] = { 0 };
	float fc_1_out_temp[1000][1][1] = { 0 };

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

	cout << "starting forward network batch process..........................." << endl;
	cout << "..........................................................." << endl;

	clock_t start, finish;
	double totaltime;
	start = clock();

	//for (int i = 0; i < 10000; i++) {//test mnist dataset
	for (int i = 0; i < 10; i++) {//test imagenet dataset
#endif

								  //Inference network process
		inference_net(
			relu, //activation function

#if _KERNEL_DEBUG_
			in_data_3D, //input pic data
#endif

#if _BATCH_MODE_
						//mnist_test_data[i], //input test mnist dataset
			imagenet_test_data[i],//input test imagenet dataset
#endif
								  //layer weights and bias inputs
			conv_1_weight2D,
			conv_1_bias2D,
			conv_2_weight2D,
			conv_2_bias2D,
			conv_3_weight2D,
			conv_3_bias2D,
			conv_4_weight2D,
			conv_4_bias2D,
			conv_5_weight2D,
			conv_5_bias2D,
			fc_6_weight2D,
			fc_6_bias2D,
			fc_7_weight2D,
			fc_7_bias2D,
			fc_8_weight2D,
			fc_8_bias2D,


#if _BATCH_MODE_
			fc_1_out_temp);
		//test mnist dataset
		/*for (int j = 0; j < 10; j++) {
		fc_1_out_a[i][j] = fc_1_out_temp[j];
		fc_1_out_temp[j] = 0;
		}*/
		//test imagenet dataset
		for (int j = 0; j < 1000; j++) {
			for (int k = 0; k < 1; k++) {
				for (int l = 0; l < 1; l++) {
					fc_1_out_a[i][j][k][l] = fc_1_out_temp[j][k][l];
					fc_1_out_temp[j][k][l] = 0;
				}
			}
		}
	}

	softmax(fc_1_out_a);

	predict(fc_1_out_a);

	//accuracy(fc_1_out_a, mnist_test_label);//for mnist dataset
	accuracy(fc_1_out_a, val_name_class);//for imagenet dataset

	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "predicted time is: " << totaltime << " s" << endl;
	getchar();
#endif

#if _KERNEL_DEBUG_
    //output fc data
	fc_8_out);
	softmax(fc_8_out);

	predict(fc_8_out);
#endif
	return 0;

}
