//
// Created by yaochen on 29/12/16.
//

#ifndef _FC_LAYER_H_
#define _FC_LAYER_H_

//#pragma once
#include "config.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include "data_type.h"
#include "activation_functions.h"


using namespace std;

template <int _IN_CHANNEL_NUM_, int _INPUT_SIZE_, int _OUT_CHANNEL_NUM_>
class fc_layer{

private:
    int fc_layer_num;

public:
    fc_layer():fc_layer_num(0){};

    /************************************************************************************************/
    //fc kernel function with array input
    void fc_kernel_a(
            float in_data[_INPUT_SIZE_][_INPUT_SIZE_],
            float kernel_weights[_INPUT_SIZE_][_INPUT_SIZE_],
            float out_data[_INPUT_SIZE_][_INPUT_SIZE_]) {

        for (int i = 0; i < _INPUT_SIZE_; i++)
        {
            for (int j = 0; j < _INPUT_SIZE_; j++)
            {
                float sum = 0;
                float data = in_data[i][j];
                float weight = kernel_weights[i][j];
                out_data[i][j] += data * weight;
            }
        }

#if _C_DEBUG_MODE_
        cout << "array fc kernel output ...." << endl;
        ofstream fc_kernel_a;
        fc_kernel_a.open("fc_kernel_a.txt", ios::app);
        fc_kernel_a << "fc kernel in data" << endl;
        for (int i = 0; i < _INPUT_SIZE_; i++) {
            for (int j = 0; j < _INPUT_SIZE_; j++) {
                fc_kernel_a << in_data[i][j] << " ";
            }
            fc_kernel_a << endl;
        }
        fc_kernel_a << endl;
        fc_kernel_a << "fc kernel in weight" << endl;
        for (int i = 0; i < _INPUT_SIZE_; i++) {
            for (int j = 0; j < _INPUT_SIZE_; j++) {
                fc_kernel_a << kernel_weights[i][j] << " ";
            }
            fc_kernel_a << endl;
        }
        fc_kernel_a << endl;
        fc_kernel_a << "fc kernel out data" << endl;
        for (int i = 0; i < 1; i++) {
            for (int j = 0; j < 1; j++) {
                fc_kernel_a << out_data[i][j] << " ";
            }
            fc_kernel_a << endl;
        }
        fc_kernel_a << endl;
        fc_kernel_a.close();
        cout << endl;
#endif
    }

    /************************************************************************************************/
    //fc layer function with with tensor input
    void fc_layer_a(
            char activation_type,
            float in_data3D[_IN_CHANNEL_NUM_][_INPUT_SIZE_][_INPUT_SIZE_],
            float kernel_weights[_IN_CHANNEL_NUM_ * _OUT_CHANNEL_NUM_][_INPUT_SIZE_][_INPUT_SIZE_],
            float kernel_bias[_OUT_CHANNEL_NUM_],
            float out_data3D[_OUT_CHANNEL_NUM_][1][1] ) {

        cout << "starting fully_connect ...." << endl;

        for (int b = 0; b < _OUT_CHANNEL_NUM_; b++) {//output kernel loop
            for (int a = 0; a < _IN_CHANNEL_NUM_; a++) {//input kernel loop
                float out_data_temp[1][1] = {0};//每一个卷积计算的结果
                fc_kernel_a(in_data3D[a],
                            kernel_weights[a * _OUT_CHANNEL_NUM_ + b],//weight的存放顺序跟convolution层的不同
                            out_data_temp);
                out_data3D[b][0][0] += out_data_temp[0][0];
            }
            out_data3D[b][0][0] = f(activation_type, (out_data3D[b][0][0] + kernel_bias[b]));
        }

        //debugging output
#if _C_DEBUG_MODE_
        cout << "finished fully_connect ...." << endl;
        ofstream fc_layer_a;
        fc_layer_a.open("fc_layer_a.txt", ios::app);
        for (int i = 0; i < _OUT_CHANNEL_NUM_; i++) {
            for (int j = 0; j < _INPUT_SIZE_; j++) {
                for (int k = 0; k < _INPUT_SIZE_; k++) {
                    fc_layer_a << out_data3D[i][j][k] << " ";
                }
                fc_layer_a << endl;
            }
            fc_layer_a << endl;
        }
        fc_layer_a.close();
        cout << endl;
#endif
    }

};


#endif //FFNET_FC_LAYER_H
