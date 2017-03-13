//This is the main function of a LeNet-5 model based application.
//Application description: LeNet-5 image recognition with std image.
//Using stb_image instead of OpenCV to eliminate library dependency.

#include <iostream>
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

//using namespace std;

int main() {

    char tan_h = 't';
    char relu = 'r';
    char none = 'i';
    int in_number_conv = 0;  // number of convolutional layer
    int in_number_fc = 0;// number of fully_connected layer
    int in_number_pooling = 0;

    //std_vec_t data_in;
    ////std_tensor_t in_data_temp;
    //float in_data_1[1][32][32] = { 0 };
    ////	//convert image to data matrix
    //	const std::string filename = ".\\example\\1.bmp";
    //	convert_image(filename, 0, 1.0, 32, 32, data_in);
    //	for (int i = 0; i < data_in.size(); i++) {
    //		in_data_1[0][i / 32][i % 32] = data_in[i];
    //	}

    //input data array
    float in_data_3D[3][227][227] = {0};

    ifstream ifs("alexnet_input.txt");
    string str;
    int count = 0;
    while (ifs >> str) {
        float f = atof(str.c_str());
        in_data_3D[count / 51529][(count % 51529) / 227][(count % 51529) % 227] = f;
        count++;
    }

    ofstream indata;
    indata.open("in_data.txt", ios::app);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 227; j++) {
            for (int k = 0; k < 227; k++) {
                indata << in_data_3D[i][j][k] << " ";
            }
            indata << endl;
        }
        indata << endl;
    }
    indata.close();

#if _BATCH_MODE_
    float mnist_train_data[60000][1][32][32];
    float mnist_train_label[60000][10] = { 0 };
    float mnist_test_data[10000][1][32][32];
    float mnist_test_label[10000][10] = { 0 };
    getSrcData(mnist_train_data, mnist_train_label, mnist_test_data, mnist_test_label);
    cout << "getSrcData end!!!!!!!!!!!!!!" << endl;
#endif

    // Prepare weights and bias for convolution layer 1
    float conv_1_weight2D[288][11][11] = {0};
    float conv_1_bias2D[96] = {0};
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
    float conv_2_weight2D[12288][5][5] = {0};
    float conv_2_bias2D[256] = {0};

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
    float conv_3_weight2D[98304][3][3] = {0};
    float conv_3_bias2D[384] = {0};

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
    float conv_4_weight2D[73728][3][3] = {0};
    float conv_4_bias2D[384] = {0};

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
    float conv_5_weight2D[49152][3][3] = {0};
    float conv_5_bias2D[256] = {0};

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
    float fc_6_weight2D[1048576][6][6] = {0};
    float fc_6_bias2D[4096] = {0};
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
    float fc_7_weight2D[16777216][1][1] = {0};
    float fc_7_bias2D[4096] = {0};
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
    float fc_8_weight2D[4096000][1][1] = {0};
    float fc_8_bias2D[1000] = {0};
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
    float fc_8_out[1000][1][1] = {0};
#endif

#if _BATCH_MODE_
    /*float fc_1_out_a[10000][10] = { 0 };
    float fc_1_out_temp[10] = { 0 };*/

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

    //clock_t start, finish;
    //double totaltime;
    //start = clock();

    //for (int i = 0; i < 10000; i++) {
#endif

    //Inference network process
    inference_net(
            relu, //activation function

#if _KERNEL_DEBUG_
            in_data_3D, //input pic data
#endif

#if _BATCH_MODE_
            mnist_test_data[i], //input test dataset
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

            //output fc data
            fc_8_out);

#if _BATCH_MODE_
    //		fc_1_out_temp);
    for (int j = 0; j < 10; j++) {
        fc_1_out_a[i][j] = fc_1_out_temp[j];
        fc_1_out_temp[j] = 0;
    }
}

softmax(fc_1_out_a);

predict(fc_1_out_a);

accuracy(fc_1_out_a, mnist_test_label);

finish = clock();
totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
cout << "predicted time is: " << totaltime << " s" << endl;
getchar();
#endif

#if _KERNEL_DEBUG_
    softmax(fc_8_out);

    predict(fc_8_out);
#endif
    return 0;

}