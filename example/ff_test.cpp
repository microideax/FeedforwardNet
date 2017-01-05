//This is the main function of a LeNet-5 model based application.
//Application description: LeNet-5 image recognition with std image.
//Using stb_image instead of OpenCV to eliminate library dependency.

//#include "stdafx.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>

#include "../fpga_cnn/config.h"
#include "../fpga_cnn/data_type.h"
#include "../fpga_cnn/image_converter.h"
#include "../fpga_cnn/weight_bias.h"
#include "../fpga_cnn/construct_net.h"
#include "../fpga_cnn/conv_layer.h"

using namespace std;



int weight_bias_count_1 = 0;
int weight_bias_count_2 = 0;

int main(int argc, char** argv) {

    int in_number_conv = 0;  // number of convolutional layer
    int in_number_fc = 0;// number of fully_connected layer
    int in_number_pooling = 0;
	std_vec_t data_in;  //image data storage
	std_tensor_t in_data_temp;
	tensor_t in_data;

	//convert image to data matrix
	const std::string filename = "./4.bmp";
	convert_image(filename, -1.0, 1.0, 32, 32, data_in);
	in_data_temp = in_2_3D(data_in);

	tensor_t_3d in_data2D;
	std_tensor_t_3d in_data2D_temp;
	in_data2D_temp = in_2_2D_conv(nn_in_data_size_conv[0], in_data_temp);//

	cout << "Finished network weights and data space preparation" << endl;
	cout << endl;
	cout << "starting convolution layer 1" << endl;

	char tan_h = 't';

	std_2_static(in_data2D, in_data2D_temp);

    //convert input data into 3D array
	ofstream indata;
    float in_data_3D[1][32][32];
	indata.open("in_data.txt", ios::app);
	for (int i = 0; i < in_data2D.size(); i++) {
		for (int j = 0; j < in_data2D[i].size(); j++) {
			for (int k = 0; k < in_data2D[i][j].size(); k++) {
				indata << in_data2D[i][j][k] << " ";
                in_data_3D[i][j][k] = in_data2D[i][j][k];
			}
			indata << endl;
		}
		indata << endl;
	}
	indata.close();

// Prepare weights and bias for convolution layer 1
	tensor_t_3d  conv_1_weight2D;//
	vec_t 		 conv_1_bias2D;//
	conv_1_weight2D = load_weight_conv(weight_bias_count_1,
		                               weight_bias_count_2,
		                               nn_channel_size_conv,
		                               nn_in_number_conv,
		                               nn_channel_number_conv,
		                               in_number_conv);
	conv_1_bias2D = load_bias_conv(weight_bias_count_1,
		                           weight_bias_count_2,
                                   nn_channel_size_conv,
		                           nn_in_number_conv,
		                           nn_channel_number_conv,
		                           in_number_conv);
    in_number_conv++;

    //convert conv_1 weight into 3D array
    float conv_1_weight_a[6][5][5];
    ofstream conv_1_weight_array;
    conv_1_weight_array.open("conv_1_weight.txt", ios::app);
    for (int i = 0; i < conv_1_weight2D.size(); i++){
        for (int j = 0; j < conv_1_weight2D[i].size(); j++){
            for (int k = 0; k < conv_1_weight2D[i][j].size(); k++){
                conv_1_weight_a[i][j][k] = conv_1_weight2D[i][j][k];
                conv_1_weight_array << conv_1_weight2D[i][j][k] << " ";
            }
            conv_1_weight_array << endl;
        }
        conv_1_weight_array << endl;
    }
    conv_1_weight_array.close();

    //convert conv_1 bias into 1D array
    float conv_1_bias_a[6];
    ofstream conv_1_bias;
    conv_1_bias.open("conv_1_bias.txt", ios::app);
    for (int i = 0; i < conv_1_bias2D.size(); i++){
        conv_1_bias << conv_1_bias2D[i]<< " ";
        conv_1_bias_a[i] = conv_1_bias2D[i];
    }
    conv_1_bias << endl;
    conv_1_bias.close();


    //Prepare weights and bias for pooling layer 1
    vec_t pooling_1_weight;
    vec_t pooling_1_bias2D;
    pooling_1_weight = load_weight_pooling(weight_bias_count_1,
                                           weight_bias_count_2,
                                           nn_channel_size_pooling,
                                           nn_in_number_pooling,
                                           in_number_pooling);
    pooling_1_bias2D = load_bias_pooling(weight_bias_count_1,
                                         weight_bias_count_2,
                                         nn_channel_size_pooling,
                                         nn_in_number_pooling,
                                         nn_channel_number_pooling,
                                         in_number_pooling);
    in_number_pooling++;

    float pool_1_weight_a[24];

    ofstream pool_1_weight_array;
    pool_1_weight_array.open("pool_1_weight.txt", ios::app);
    for (int i = 0; i < pooling_1_weight.size(); i++){
        pool_1_weight_a[i] = pooling_1_weight[i];
        pool_1_weight_array << pooling_1_weight[i] << " ";
    }
    pool_1_weight_array << endl;
    pool_1_weight_array.close();

    //convert conv_1 bias into 1D array
    float pool_1_bias_a[6];
    ofstream pool_1_bias;
    pool_1_bias.open("pool_1_bias.txt", ios::app);
    for (int i = 0; i < pooling_1_bias2D.size(); i++){
        pool_1_bias_a[i] = pooling_1_bias2D[i];
        pool_1_bias << pool_1_bias_a[i] << " ";
    }
    pool_1_bias << endl;
    pool_1_bias.close();


    // Prepare weights and bias for convolution layer 2
    tensor_t_3d conv_2_weight2D;
    vec_t conv_2_bias2D;
    conv_2_weight2D = load_weight_conv(weight_bias_count_1,
                                       weight_bias_count_2,
                                       nn_channel_size_conv,
                                       nn_in_number_conv,
                                       nn_channel_number_conv,
                                       in_number_conv);
    conv_2_bias2D = load_bias_conv(weight_bias_count_1,
                                   weight_bias_count_2,
                                   nn_channel_size_conv,
                                   nn_in_number_conv,
                                   nn_channel_number_conv,
                                   in_number_conv);
    in_number_conv++;


    //convert conv_2 weight into 3D array
    float conv_2_weight_a[96][5][5];

    ofstream conv_2_weight_array;
    conv_2_weight_array.open("conv_2_weight.txt", ios::app);
    for (int i = 0; i < conv_2_weight2D.size(); i++){
        for (int j = 0; j < conv_2_weight2D[i].size(); j++){
            for (int k = 0; k < conv_2_weight2D[i][j].size(); k++){
                conv_2_weight_a[i][j][k] = conv_2_weight2D[i][j][k];
                conv_2_weight_array << conv_2_weight2D[i][j][k] << " ";
            }
            conv_2_weight_array << endl;
        }
        conv_2_weight_array << endl;
    }
    conv_2_weight_array.close();


    //convert conv_1 bias into 1D array
    float conv_2_bias_a[16];
    ofstream conv_2_bias_array;
    conv_2_bias_array.open("conv_2_bias.txt", ios::app);
    for (int i = 0; i < conv_2_bias2D.size(); i++){
        conv_2_bias_array << conv_2_bias2D[i]<< " ";
        conv_2_bias_a[i] = conv_2_bias2D[i];
    }
    conv_2_bias_array << endl;
    conv_2_bias_array.close();


    // Prepare weights and bias for pooling layer 2
    vec_t pooling_2_weight;
    float pool_2_weight_a[64];
    vec_t pooling_2_bias2D;
    float pool_2_bias_a[16];
    pooling_2_weight = load_weight_pooling(weight_bias_count_1,
                                           weight_bias_count_2,
                                           nn_channel_size_pooling,
                                           nn_in_number_pooling,
                                           in_number_pooling);
    pooling_2_bias2D = load_bias_pooling(weight_bias_count_1,
                                         weight_bias_count_2,
                                         nn_channel_size_pooling,
                                         nn_in_number_pooling,
                                         nn_channel_number_pooling,
                                         in_number_pooling);
    in_number_pooling++;

    cout << "pooling 2 weight" << endl;
    for (int i = 0; i < pooling_2_weight.size(); i++){
        pool_2_weight_a[i] = pooling_2_weight[i];
//        cout << pooling_2_weight[i] << " ";
    }
    cout << endl;
    cout << "pooling 2 bias" << endl;
    for (int j = 0; j < pooling_2_bias2D.size(); j++){
        pool_2_bias_a[j] = pooling_2_bias2D[j];
//        cout << pooling_2_bias2D[j] << " ";
    }
    cout << endl;

    // Prepare weights and bias for convolution layer 3
    tensor_t_3d conv_3_weight2D;
    float conv_3_weight_a[1920][5][5];
    vec_t conv_3_bias2D;
    float conv_3_bias_a[120];
    conv_3_weight2D = load_weight_conv(weight_bias_count_1,
                                       weight_bias_count_2,
                                       nn_channel_size_conv,
                                       nn_in_number_conv,
                                       nn_channel_number_conv,
                                       in_number_conv);
    conv_3_bias2D = load_bias_conv(weight_bias_count_1,
                                   weight_bias_count_2,
                                   nn_channel_size_conv,
                                   nn_in_number_conv,
                                   nn_channel_number_conv,
                                   in_number_conv);
    in_number_conv++;


    for (int i = 0; i < conv_3_weight2D.size(); i++){
        for (int j = 0; j < conv_3_weight2D[i].size(); j++){
            for (int k = 0; k < conv_3_weight2D[i][j].size(); k++){
                conv_3_weight_a[i][j][k] = conv_3_weight2D[i][j][k];
            }
        }
    }
    for (int i = 0; i < conv_3_bias2D.size(); i++){
        conv_3_bias_a[i] = conv_3_bias2D[i];
    }


    tensor_t_3d conv_3_out_t;
    float conv_3_out_a[120][1][1];

    // Prepare weights and bias for fully connected layer 1
    tensor_t_3d fc_1_weight2D;
    float fc_1_weight_a[1200][1][1];
    vec_t fc_1_bias2D;
    float fc_1_bias_a[10];
    fc_1_weight2D = load_weight_fc(weight_bias_count_1,
                                   weight_bias_count_2,
                                   nn_channel_size_fc,
                                   nn_in_number_fc,
                                   nn_channel_number_fc,
                                   in_number_fc);
    fc_1_bias2D = load_bias_fc(weight_bias_count_1,
                               weight_bias_count_2,
                               nn_channel_size_fc,
                               nn_in_number_fc,
                               nn_channel_number_fc,
                               in_number_fc);
    in_number_fc++;

    for (int i = 0; i < fc_1_weight2D.size(); i++){
        for (int j = 0; j < fc_1_weight2D[i].size(); j++){
            for (int k = 0; k < fc_1_weight2D[i][j].size(); k++){
                fc_1_weight_a[i][j][k] = fc_1_weight2D[i][j][k];
            }
        }
    }
    for (int i = 0; i < fc_1_bias2D.size(); i++){
        fc_1_bias_a[i] = fc_1_bias2D[i];
    }

    float fc_1_out_a[10][1][1];



    // Inference network process
   inference_net(
        tan_h,
        // input pic data
        in_data_3D,
        // layer weights and bias inputs
        conv_1_weight_a,
        conv_1_bias_a,
        pool_1_weight_a,
        pool_1_bias_a,
        conv_2_weight_a,
        conv_2_bias_a,
        pool_2_weight_a,
        pool_2_bias_a,
        conv_3_weight_a,
        conv_3_bias_a,
        fc_1_weight_a,
        fc_1_bias_a,

        // output fc data
        fc_1_out_a
);
	return 0;
}

