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
	const std::string filename = "4.bmp";
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
    float indata_array[1][32][32];
	indata.open("in_data.txt", ios::app);
	for (int i = 0; i < in_data2D.size(); i++) {
		for (int j = 0; j < in_data2D[i].size(); j++) {
			for (int k = 0; k < in_data2D[i][j].size(); k++) {
				indata << in_data2D[i][j][k] << " ";
                indata_array[i][j][k] = in_data2D[i][j][k];
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
    float conv_1_weight_array[6][5][5];
    ofstream conv_1_weight_a;
    conv_1_weight_a.open("conv_1_weight.txt", ios::app);
    for (int i = 0; i < conv_1_weight2D.size(); i++){
        for (int j = 0; j < conv_1_weight2D[i].size(); j++){
            for (int k = 0; k < conv_1_weight2D[i][j].size(); k++){
                conv_1_weight_array[i][j][k] = conv_1_weight2D[i][j][k];
                conv_1_weight_a << conv_1_weight2D[i][j][k] << " ";
            }
            conv_1_weight_a << endl;
        }
        conv_1_weight_a << endl;
    }
    conv_1_weight_a.close();

    //convert conv_1 bias into 1D array
    float conv_1_bias_array[6];
    ofstream conv_1_bias;
    conv_1_bias.open("conv_1_bias.txt", ios::app);
    for (int i = 0; i < conv_1_bias2D.size(); i++){
        conv_1_bias << conv_1_bias2D[i]<< " ";
        conv_1_bias_array[i] = conv_1_bias2D[i];
    }
      conv_1_bias << endl;

    /* conv kernel testing */
    float kernel_out[28][28];
    float conv_1_out[6][28][28];
    tensor_t kernel_out_tensor;
    tensor_t_3d conv_1_out_test;
    conv_layer<32, 5, 1, 6> conv_layer_test;
    conv_layer_test.conv_kernel_array(indata_array[0], conv_1_weight_array[0], kernel_out);
    conv_layer_test.convolution_kernel(in_data2D[0], conv_1_weight2D[0], kernel_out_tensor);
    conv_layer_test.conv_layer_array(tan_h, indata_array, has_connection_table[0], conv_1_weight_array, conv_1_bias_array, conv_1_out);
    conv_layer_test.convolution_layer_with_table(tan_h, in_data2D, has_connection_table[0], conv_1_weight2D, conv_1_bias2D, conv_1_out_test);

    ofstream kernel_out_a;
    kernel_out_a.open("kernel_out.txt", ios::app);
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 28; j++) {
            kernel_out_a << kernel_out[i][j] << " ";
        }
        kernel_out_a << endl;
    }
    kernel_out_a.close();

    ofstream kernel_out_t;
    kernel_out_t.open("kernel_out_gold.txt", ios::app);
    for (int i = 0; i < kernel_out_tensor.size(); i++) {
        for (int j = 0; j < kernel_out_tensor.size(); j++) {
            kernel_out_t << kernel_out_tensor[i][j] << " ";
        }
        kernel_out_t << endl;
    }
    kernel_out_t.close();

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

    // Prepare weights and bias for pooling layer 2
    vec_t pooling_2_weight;
    vec_t pooling_2_bias2D;
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

    // Prepare weights and bias for convolution layer 3
    tensor_t_3d conv_3_weight2D;
    vec_t conv_3_bias2D;
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

    // Prepare weights and bias for fully connected layer 1
    tensor_t_3d fc_1_weight2D;
    vec_t fc_1_bias2D;
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

    tensor_t_3d fc_1_out_data;

    //temporary data storage with AXI bus interface
    tensor_t_3d conv_1_out_data;
    tensor_t_3d pooling_1_out_data;
    tensor_t_3d conv_2_out_data;
    tensor_t_3d pooling_2_out_data;
    tensor_t_3d conv_3_out_data;

/*
    // Inference network process
    inference_net(
            tan_h,
            nn_in_data_size_conv,
            nn_channel_size_conv,
            has_connection_table,
            nn_in_number_conv,
            nn_channel_number_conv,

            // input pic data
            in_data2D,
            // output fc data
            fc_1_out_data,

            // layer weights and bias inputs
            conv_1_weight2D,
            conv_1_bias2D,
            pooling_1_weight,
            pooling_1_bias2D,
            conv_2_weight2D,
            conv_2_bias2D,
            pooling_2_weight,
            pooling_2_bias2D,
            conv_3_weight2D,
            conv_3_bias2D,
            fc_1_weight2D,
            fc_1_bias2D,

            //temporary data storage with AXI bus interface
             conv_1_out_data,
             pooling_1_out_data,
             conv_2_out_data,
             pooling_2_out_data,
             conv_3_out_data);
*/
	return 0;
}

