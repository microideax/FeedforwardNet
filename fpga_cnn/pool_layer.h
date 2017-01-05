//
// Created by yaochen on 29/12/16.
//
//average pooling layer with kernel weights and without kernel weights
//TODO: Add max pooling without kernel weights functions.

#ifndef _POOLING_LAYER_H_
#define _POOLING_LAYER_H_

//#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include "data_type.h"
#include "activation_functions.h"
#include "config.h"

using namespace std;

template < int _INPUT_SIZE_, int _KERNEL_SIZE_, int _IN_CHANNEL_NUM_ >
class pool_layer {

private:
    int pooling_layer_number;

public:
    pool_layer():pooling_layer_number(0){};

    /************************************************************************************/
    // pooling kernel function with array input
    void pooling_kernel_a(
            float in_data[_INPUT_SIZE_][_INPUT_SIZE_],
            float kernel_weight,
            float out_data[][_INPUT_SIZE_ / _KERNEL_SIZE_]) {

        for (int i = 0; i < _INPUT_SIZE_ - _KERNEL_SIZE_ / 2; i = i + _KERNEL_SIZE_) {
            for (int j = 0; j < _INPUT_SIZE_ - _KERNEL_SIZE_ / 2; j = j + _KERNEL_SIZE_) {
                float sum = 0;
                for (int ii = 0; ii < _KERNEL_SIZE_; ++ii) {
                    for (int jj = 0; jj < _KERNEL_SIZE_; ++jj) {
                        float data = in_data[i + ii][j + jj];
                        sum += data;
                    }
                }
                sum = (float)(sum / (_KERNEL_SIZE_ * _KERNEL_SIZE_));//求出每个pooling窗口内的均值
                out_data[i/ _KERNEL_SIZE_][j / _KERNEL_SIZE_] = sum * kernel_weight;//每个输入乘同一个weight
            }
        }

#if _C_DEBUG_MODE_
        cout << "pooling kernel a input array...." << endl;
        ofstream pool_kernel_a;
        pool_kernel_a.open("pool_kernel_a.txt", ios::app);
        pool_kernel_a << "pooling kernel a input data" << endl;
        pool_kernel_a << kernel_weight << endl;
        for (int i = 0; i < _INPUT_SIZE_; i++) {
            for (int j = 0; j < _INPUT_SIZE_; j++) {
                pool_kernel_a << in_data[i][j] << " ";
            }
            pool_kernel_a << endl;
        }
        pool_kernel_a << endl;

        pool_kernel_a << "pooling kernel a output data" << endl;
        for (int i = 0; i < _INPUT_SIZE_ / _KERNEL_SIZE_; i++) {
            for (int j = 0; j < _INPUT_SIZE_ / _KERNEL_SIZE_; j++) {
                pool_kernel_a << out_data[i][j] << " ";
            }
            pool_kernel_a << endl;
        }
        pool_kernel_a.close();
        cout << endl;
#endif

    }

    /************************************************************************************/
    //pooling layer function with array input
    void pooling_layer_a(
            char activation_type,
            float in_data3D[_IN_CHANNEL_NUM_][_INPUT_SIZE_][_INPUT_SIZE_],
            float kernel_weights[],
            float kernel_bias[],
            float out_data3D[_IN_CHANNEL_NUM_][_INPUT_SIZE_/_KERNEL_SIZE_][_INPUT_SIZE_/_KERNEL_SIZE_] ) {

        cout << "Starting average_pooling ...." << endl;

        float out_data2D[_INPUT_SIZE_ / _KERNEL_SIZE_][_INPUT_SIZE_ / _KERNEL_SIZE_];

        for (int a = 0; a < _IN_CHANNEL_NUM_; a++) {//input kernel loop
            pooling_kernel_a(
                    in_data3D[a],
                    kernel_weights[a],
                    out_data3D[a]);
            //循环遍历out_data2D矩阵加偏置和激活
            for (int i = 0; i < _INPUT_SIZE_ / _KERNEL_SIZE_; i++) {
                for (int j = 0; j < _INPUT_SIZE_ / _KERNEL_SIZE_; j++) {
                    out_data3D[a][i][j] = f(activation_type, (out_data3D[a][i][j] + kernel_bias[a]));
                }
            }
        }
        cout << "Finished pooling array layer...." << endl;

        //debugging output
#if _C_DEBUG_MODE_
        ofstream out_pool_a;
        out_pool_a.open("pool_layer_a.txt", ios::app);
        for (int i = 0; i < _IN_CHANNEL_NUM_; i++) {
            for (int j = 0; j < _INPUT_SIZE_ / _KERNEL_SIZE_; j++) {
                for (int k = 0; k < _INPUT_SIZE_ / _KERNEL_SIZE_; k++) {
                    out_pool_a << out_data3D[i][j][k] << " ";
                }
                out_pool_a << endl;
            }
            out_pool_a << endl;
        }
        out_pool_a.close();
        cout << endl;
#endif
    }
};

#endif //_POOLING_LAYER_H
