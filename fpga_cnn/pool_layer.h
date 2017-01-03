//
// Created by yaochen on 29/12/16.
//
//average pooling layer with kernel weights and without kernel weights
//TODO: Add average/max pooling without kernel weights functions.

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

template < int _INPUT_SIZE_, int _KERNEL_SIZE_, int _IN_CHANNEL_NUM_, char _METHOD_ >
class pool_layer {

private:
    int pooling_layer_number;

public:
    pool_layer():pooling_layer_number(0){};

    /************************************************************************************/
    void pooling_kernel(
            tensor_t& in_data,
            float kernel_weights,
            tensor_t& out_data) {

        out_data.clear();
        vec_t vec2;//output row vector
        for (int i = 0; i < _INPUT_SIZE_ - _KERNEL_SIZE_ / 2; i = i + _KERNEL_SIZE_) //遍历输入map
        {
            for (int j = 0; j < _INPUT_SIZE_ - _KERNEL_SIZE_ / 2; j = j + _KERNEL_SIZE_)
            {
            #if _HLS_MODE_
            #pragma HLS PIPELINE
            #endif
                float sum = 0;
                for (int ii = 0; ii < _KERNEL_SIZE_; ++ii) //遍历kernel
                {
                #if _HLS_MODE_
                #pragma HLS UNROLL
                #endif
                    for (int jj = 0; jj < _KERNEL_SIZE_; ++jj)
                    {
                    #if _HLS_MODE_
                    #pragma HLS UNROLL
                    #endif
                        float data = in_data[i + ii][j + jj];
                        sum += data;
                    }
                }
                switch (_METHOD_){
                    case 'a':
                        sum = (float)(sum / (_KERNEL_SIZE_ * _KERNEL_SIZE_));//求出每个pooling窗口内的均值
//                    case 'm':
//                        sum =
                }
                sum = sum * kernel_weights;//每个输入乘同一个weight
                //sum += kernel_bias;
                vec2.push_back(sum);//放入sum构成输出行向量
            }
            out_data.push_back(vec2);//放入输出行向量构成输出map
            vec2.clear();
        }

#if _C_DEBUG_MODE_
        cout << "tensor pooling kernel input ...." << endl;
        ofstream pool_kernel_t;
        pool_kernel_t.open("pool_kernel_t.txt", ios::app);
        for (int i = 0; i < in_data.size(); i++) {
            for (int j = 0; j < in_data[i].size(); j++) {
                pool_kernel_t << in_data[i][j] << " ";
            }
            pool_kernel_t << endl;
        }
        pool_kernel_t << endl;

        for (int i = 0; i < out_data.size(); i++) {
            for (int j = 0; j < out_data[i].size(); j++) {
                pool_kernel_t << out_data[i][j] << " ";
            }
            pool_kernel_t << endl;
        }
        pool_kernel_t.close();
        cout << endl;
#endif

    }

    /************************************************************************************/
    void pooling_kernel_w_array(
            float in_data[][_INPUT_SIZE_],
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
                switch (_METHOD_){
                    case 'a':
                        sum = (float)(sum / (_KERNEL_SIZE_ * _KERNEL_SIZE_));//求出每个pooling窗口内的均值
                    default:
                        sum = 0;
                }
                out_data[i][j] = sum * kernel_weight;//每个输入乘同一个weight
            }
        }

#if _C_DEBUG_MODE_
        cout << "array pooling kernel input ...." << endl;
        ofstream pool_kernel_a;
        pool_kernel_a.open("pool_kernel_a.txt", ios::app);
        for (int i = 0; i < _INPUT_SIZE_; i++) {
            for (int j = 0; j < _INPUT_SIZE_; j++) {
                pool_kernel_a << in_data[i][j] << " ";
            }
            pool_kernel_a << endl;
        }
        pool_kernel_a << endl;

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
    void pooling_layer(
            char activation_type,
            tensor_t_3d& in_data3D,
            vec_t& kernel_weights,
            vec_t& kernel_bias,
            tensor_t_3d& out_data3D ) {

        cout << "Starting average_pooling ...." << endl;
        out_data3D.clear();

        tensor_t out_data2D;
        float out_data2D_final_f;
        vec_t out_data2D_final_v;
        tensor_t out_data2D_final;

        for (int a = 0; a < _IN_CHANNEL_NUM_; a++) {//input kernel loop
            pooling_kernel(
                    in_data3D[a],
                    kernel_weights[a],
                    out_data2D);
            //循环遍历out_data2D矩阵加偏置和激活
            for (int i = 0; i < out_data2D.size(); i++) {
                for (int j = 0; j < out_data2D[i].size(); j++) {
                    out_data2D_final_f = out_data2D[i][j] + kernel_bias[a];

                    out_data2D_final_f = f(activation_type, out_data2D_final_f);//激活函数激活

                    out_data2D_final_v.push_back(out_data2D_final_f);//放入每个像素最终输出值
                }
                out_data2D_final.push_back(out_data2D_final_v);//把每个行向量累加结果放入out_data2D_final
                out_data2D_final_v.clear();
            }
            out_data3D.push_back(out_data2D_final);
            out_data2D.clear();
            out_data2D_final.clear();
        }
        cout << "Finished average_pooling ...." << endl;

        //debugging output
        #if _C_DEBUG_MODE_
        ofstream out_pool_class;
        out_pool_class.open("out_pool_class.txt", ios::app);
	    for (int i = 0; i < out_data3D.size(); i++) {
		    for (int j = 0; j < out_data3D[i].size(); j++) {
			    for (int k = 0; k < out_data3D[i][j].size(); k++) {
				    out_pool_class << out_data3D[i][j][k] << " ";
			    }
			    out_pool_class << endl;
		    }
            out_pool_class << endl;
	    }
	    out_pool_class.close();
	    cout << endl;
        #endif
    }

    /************************************************************************************/
    void pooling_layer_w_array(
            char activation_type,
            float in_data3D[][_INPUT_SIZE_][_INPUT_SIZE_],
            float kernel_weights[],
            float kernel_bias[],
            float out_data3D[][_INPUT_SIZE_/_KERNEL_SIZE_][_INPUT_SIZE_/_KERNEL_SIZE_] ) {

        cout << "Starting average_pooling ...." << endl;

        float out_data2D[_INPUT_SIZE_ / _KERNEL_SIZE_][_INPUT_SIZE_ / _KERNEL_SIZE_];

        for (int a = 0; a < _IN_CHANNEL_NUM_; a++) {//input kernel loop
            pooling_kernel_w_array(
                    in_data3D[a],
                    kernel_weights[a],
                    out_data2D);
            //循环遍历out_data2D矩阵加偏置和激活
            for (int i = 0; i < _INPUT_SIZE_ / _KERNEL_SIZE_; i++) {
                for (int j = 0; j < _INPUT_SIZE_ / _KERNEL_SIZE_; j++) {
                    out_data3D[a][i][j] = f(_METHOD_, (out_data2D[i][j] + kernel_bias[a]));
                }
            }
        }
        cout << "Finished pooling array layer...." << endl;

        //debugging output
#if _C_DEBUG_MODE_
        ofstream out_pool_a;
        out_pool_a.open("out_pool_a.txt", ios::app);
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
