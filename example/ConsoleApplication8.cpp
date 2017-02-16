// ConsoleApplication8.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


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

#include "../fpga_cnn/config.h"
#include "../fpga_cnn/data_type.h"
#include "../fpga_cnn/image_converter.h"
#include "../fpga_cnn/weight_bias.h"
#include "../fpga_cnn/construct_net.h"
#include "../fpga_cnn/conv_layer.h"

using namespace std;

int main() {

	char tan_h = 't';
	char none = 'i';
	int in_number_conv = 0;  // number of convolutional layer
	int in_number_fc = 0;// number of fully_connected layer
	int in_number_pooling = 0;

	//	//convert image to data matrix
	//	const std::string filename = "./4.bmp";
	//	convert_image(filename, -1.0, 1.0, 32, 32, data_in);
	//	in_data_temp = in_2_3D(data_in);
	//	tensor_t_3d in_data2D;
	//	std_tensor_t_3d in_data2D_temp;
	//	in_data2D_temp = in_2_2D_conv(nn_in_data_size_conv[0], in_data_temp);

	//input data array
	float in_data_3D[1][32][32] = { 0 };

	ifstream ifs(".\\example\\demo\\lenet\\input.txt");
	string str;
	int count = 0;
	while (ifs >> str)
	{
		float f = atof(str.c_str());
		in_data_3D[0][count / 32][count % 32] = f;
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
										   //float        conv_3_weight2D[1920][nn_channel_size_conv][nn_channel_size_conv];//
										   //float 		 conv_3_bias2D[120];//
										   //load_weight_conv(
										   //	conv_3_weight2D,
										   //	weight_bias_count_1,
										   //	weight_bias_count_2,
										   //	nn_channel_size_conv,
										   //	nn_in_number_conv,
										   //	nn_channel_number_conv,
										   //	in_number_conv);
										   //load_bias_conv(
										   //	conv_3_bias2D,
										   //	weight_bias_count_1,
										   //	weight_bias_count_2,
										   //	nn_channel_size_conv,
										   //	nn_in_number_conv,
										   //	nn_channel_number_conv,
										   //	in_number_conv);
										   //in_number_conv++;

										   //cout << "conv 3 weight size = " << conv_3_weight2D.size() << "  " << conv_3_weight2D[0].size() << "  " << conv_3_weight2D[0][0].size() << endl;
										   //cout << "conv 3 bias size = " << conv_3_bias2D.size() << endl;

										   // Prepare weights and bias for fully connected layer 1
										   //tensor_t_3d fc_1_weight2D;
										   float fc_1_weight2D[160][5][5];
										   //vec_t fc_1_bias2D;
										   float fc_1_bias2D[10];
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
										   
										   float fc_1_out_a[10] = { 0 };


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

										   //Inference network process
										   inference_net(

											   tan_h,

										   //input pic data
										   in_data_3D,

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
										   
	return 0;

}


