//Network construction function, will be modified after all functional blocks are tested.
// Created by yaochen on 19/12/16.
//TODO: change all functions to store data in array, array dimensions are defined by network model
//

#ifndef _WEIGHT_BIAS_H_
#define _WEIGHT_BIAS_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "data_type.h"

/*
void load_weight_conv(
	float conv_1_weight2D[][5][5],
	int& weight_bias_count_1,
	int& weight_bias_count_2,
	int nn_channel_size_conv,
	int nn_in_number_conv[],
	int nn_channel_number_conv[],
	int in_number_conv) {

    cout << "Loading CONV layer weights ..." << endl;
	ifstream ifs("./weights_batch.txt");
	string str;
    if (!ifs) {
        cout << "weight file not found !" << endl;
    }
	int weight_count = weight_bias_count_1 +
                        nn_channel_size_conv * nn_channel_size_conv
                        * nn_in_number_conv[in_number_conv]
		                * nn_channel_number_conv[in_number_conv];
	int weight_bias_count_3_conv = 0;
	while (ifs >> str && weight_bias_count_3_conv < (weight_count + 1))
	{
        cout << str << "  ";
		if (weight_bias_count_3_conv >= weight_bias_count_2) {
			int serial_no = weight_bias_count_3_conv - weight_bias_count_2 - 1;
			float f = atof(str.c_str());
			conv_1_weight2D[serial_no / (nn_channel_size_conv*nn_channel_size_conv)][(serial_no % (nn_channel_size_conv*nn_channel_size_conv)) / nn_channel_size_conv][serial_no%nn_channel_size_conv] = f;
			weight_bias_count_1++;
		}
		weight_bias_count_3_conv++;
	}
    cout << endl;
	cout << "conv layer weights number in total is = " << weight_bias_count_1 << endl;
	ifs.close();
}*/

void load_weight_conv(
        float conv_weight2D[][5][5],
        float conv_bias[],
        int& weight_bias_record,
        int nn_channel_size_conv,
        int nn_in_number_conv[],
        int nn_out_number_conv[],
        int in_number_conv) {

    cout << "Loading CONV layer weights ..." << endl;
    ifstream ifs("./weights_batch.txt");
    string str;
    if (!ifs) {
        cout << "weight file not found !" << endl;
    }
    int layer_weight_num = nn_channel_size_conv * nn_channel_size_conv
                       * nn_in_number_conv[in_number_conv]
                       * nn_out_number_conv[in_number_conv];
    int weight_bias_count = 0;

    ifs >> str;
    if(ifs.eof())  cout << "file error" << endl;
    else cout << str << endl;
    while (ifs >> str && weight_bias_count < layer_weight_num + weight_bias_record) {
        if (weight_bias_count >= weight_bias_record) {
            cout << str << "  ";
            int serial_no = weight_bias_count - weight_bias_record;
            float f = atof(str.c_str());
            int channel = serial_no / (nn_channel_size_conv * nn_channel_size_conv);
            int y_index = (serial_no % (nn_channel_size_conv * nn_channel_size_conv)) / nn_channel_size_conv;
            int x_index = serial_no % nn_channel_size_conv;
            conv_weight2D[channel][y_index][x_index] = f;
        }
        weight_bias_count++;
    }
    weight_bias_record = weight_bias_count;
    cout << endl;
    cout << "conv layer weights number in total is = " << weight_bias_count << endl;
    cout << "network weights in total is = " << weight_bias_count << endl;
}
void load_bias_conv(
        float conv_1_bias2D[],
        int& weight_bias_count_1,
        int& weight_bias_count_2,
        int nn_channel_size_conv,
        int nn_in_number_conv[],
        int nn_channel_number_conv[],
        int in_number_conv) {

    cout << "Loading CONV layer bias ..." << endl;
    ifstream ifs("./weights_batch.txt");
    if (!ifs) {
        cout << "weight file not found !" << endl;
    }
    string str;
    int layer_bias_num = nn_in_number_conv[in_number_conv] * nn_out_number_conv[in_number_conv];
    int weight_bias_count = weight_count + nn_channel_number_conv[in_number_conv];
    int weight_bias_count_3_conv = 0;
    while (ifs >> str && weight_bias_count_3_conv < weight_bias_count)
    {
        if (weight_bias_count_3_conv >= weight_bias_count_1 && weight_bias_count_1 >= weight_count
            && weight_bias_count_1 <= weight_bias_count) {
            cout << str << "  ";
            float f = atof(str.c_str());
            conv_1_bias2D[weight_bias_count_1 - weight_count] = f;
            weight_bias_count_1++;
        }
        weight_bias_count_3_conv++;
    }
    weight_bias_count_2 = weight_bias_count_1;
    cout << endl;
    cout << "conv layer bias number in total is = " << weight_bias_count_1 << endl;
    ifs.close();
    //return bias2D;
}

void load_weight_pooling(
	float pooling_1_weight[],
	int& weight_bias_count_1,
	int& weight_bias_count_2,
	int nn_channel_size_pooling,
	int nn_in_number_pooling[],
	int in_number_pooling) {

    cout << "Loading POOL layer weights ..." << endl;

	ifstream ifs("./weights_batch.txt");
	string str;
	int weight_count = weight_bias_count_1 +
		nn_channel_size_pooling*nn_channel_size_pooling*nn_in_number_pooling[in_number_pooling];
	int weight_bias_count_3_pooling = 0;
	while (ifs >> str&&weight_bias_count_3_pooling<weight_count)
	{
        cout << str << "  ";
		if (weight_bias_count_3_pooling >= weight_bias_count_2) {
			//
			int serial_no = weight_bias_count_3_pooling - weight_bias_count_2;
			float f = atof(str.c_str());
			//weight_v.push_back(f);
			pooling_1_weight[serial_no] = f;
			weight_bias_count_1++;
		}
		weight_bias_count_3_pooling++;
	}
	cout << "pooling layer weights number in total is = " << weight_bias_count_1 << endl;
	ifs.close();
	//return weight_v;
}

void load_weight_fc(
	float fc_1_weight2D[][1][1],
	int& weight_bias_count_1,
	int& weight_bias_count_2,
	int nn_channel_size_fc,
	int nn_in_number_fc[],
	int nn_channel_number_fc[],
	int in_number_fc) {

    cout << "Loading FC layer weights ..." << endl;

	ifstream ifs("./weights_batch.txt");
	string str;
	int weight_count = weight_bias_count_1 + nn_channel_size_fc*nn_channel_size_fc*nn_in_number_fc[in_number_fc]
		* nn_channel_number_fc[in_number_fc];
	int weight_bias_count_3_fc = 0;
	while (ifs >> str && weight_bias_count_3_fc < weight_count)
	{
		if (weight_bias_count_3_fc >= weight_bias_count_2) {
			//
			if (nn_channel_size_fc == 1) {
				int serial_no = weight_bias_count_3_fc - weight_bias_count_2;
				float f = atof(str.c_str());
				fc_1_weight2D[serial_no / (nn_channel_size_fc*nn_channel_size_fc)][0][0] = f;
			}
			else {
				int serial_no = weight_bias_count_3_fc - weight_bias_count_2;
				float f = atof(str.c_str());
				fc_1_weight2D[serial_no / (nn_channel_size_fc*nn_channel_size_fc)][(serial_no % (nn_channel_size_fc*nn_channel_size_fc)) / nn_channel_size_fc][serial_no%nn_channel_size_fc] = f;
			}
			weight_bias_count_1++;
		}
		weight_bias_count_3_fc++;
	}
	cout << "fc layer weights number in total is = " << weight_bias_count_1 << endl;
	ifs.close();
}
/*
void load_bias_conv(
	float conv_1_bias2D[],
	int& weight_bias_count_1,
	int& weight_bias_count_2,
	int nn_channel_size_conv,
	int nn_in_number_conv[],
	int nn_channel_number_conv[],
	int in_number_conv) {

    cout << "Loading CONV layer bias ..." << endl;

	ifstream ifs("./weights_batch.txt");
    if (!ifs) {
        cout << "weight file not found !" << endl;
    }
	string str;
	vec_t bias2D;
	int weight_count = weight_bias_count_2
		+ nn_channel_size_conv
		* nn_channel_size_conv*nn_in_number_conv[in_number_conv]
		* nn_channel_number_conv[in_number_conv];
	int weight_bias_count = weight_count + nn_channel_number_conv[in_number_conv];
	int weight_bias_count_3_conv = 0;
	while (ifs >> str && weight_bias_count_3_conv < weight_bias_count)
	{
        cout << str << "  ";
		if (weight_bias_count_3_conv >= weight_bias_count_1 && weight_bias_count_1 >= weight_count
			&& weight_bias_count_1 <= weight_bias_count) {
			float f = atof(str.c_str());
			conv_1_bias2D[weight_bias_count_1 - weight_count] = f;
			weight_bias_count_1++;
		}
		weight_bias_count_3_conv++;
	}
	weight_bias_count_2 = weight_bias_count_1;
    cout << endl;
	cout << "conv layer bias number in total is = " << weight_bias_count_1 << endl;
	ifs.close();
	//return bias2D;
}
*/
void load_bias_pooling(
	float pooling_1_bias2D[],
	int& weight_bias_count_1,
	int& weight_bias_count_2,
	int nn_channel_size_pooling,
	int nn_in_number_pooling[],
	int nn_channel_number_pooling[],
	int in_number_pooling) {

    cout << "Loading POOL layer bias ..." << endl;

	ifstream ifs("./weights_batch.txt");
	string str;
	//vec_t bias2D;
	int weight_count = weight_bias_count_2
		+ nn_channel_size_pooling*nn_channel_size_pooling * nn_in_number_pooling[in_number_pooling];
	int weight_bias_count = weight_count + nn_channel_number_pooling[in_number_pooling];
	int weight_bias_count_3_pooling = 0;
	while (ifs >> str&&weight_bias_count_3_pooling<weight_bias_count)
	{
		if (weight_bias_count_3_pooling >= weight_bias_count_1 &&
			weight_bias_count_1 >= weight_count &&
			weight_bias_count_1 <= weight_bias_count) {
			float f = atof(str.c_str());
			//bias2D.push_back(f);//
			pooling_1_bias2D[weight_bias_count_1 - weight_count] = f;
			weight_bias_count_1++;
		}
		weight_bias_count_3_pooling++;
	}
	weight_bias_count_2 = weight_bias_count_1;
	cout << "pooling layer bias number in total is = " << weight_bias_count_1 << endl;
	ifs.close();
	//return bias2D;
}

void load_bias_fc(
	float fc_1_bias2D[],
	int& weight_bias_count_1,
	int& weight_bias_count_2,
	int nn_channel_size_fc,
	int nn_in_number_fc[],
	int nn_channel_number_fc[],
	int in_number_fc) {

    cout << "Loading FC layer bias ..." << endl;

	ifstream ifs("./weights_batch.txt");
	string str;
	//vec_t bias2D;
	int weight_count = weight_bias_count_2
		+ nn_channel_size_fc*nn_channel_size_fc*nn_in_number_fc[in_number_fc]
		* nn_channel_number_fc[in_number_fc];
	int weight_bias_count = weight_count + nn_channel_number_fc[in_number_fc];
	int weight_bias_count_3_fc = 0;
	while (ifs >> str&&weight_bias_count_3_fc<weight_bias_count)
	{
		if (weight_bias_count_3_fc >= weight_bias_count_1 &&
			weight_bias_count_1 >= weight_count &&
			weight_bias_count_1 <= weight_bias_count) {
			float f = atof(str.c_str());
			//bias2D.push_back(f);
			fc_1_bias2D[weight_bias_count_1 - weight_count] = f;
			weight_bias_count_1++;
		}
		weight_bias_count_3_fc++;
	}
	weight_bias_count_2 = weight_bias_count_1;
	cout << "fc layer bias number in total is = " << weight_bias_count_1 << endl;
	ifs.close();
	//return bias2D;
}

#endif //
