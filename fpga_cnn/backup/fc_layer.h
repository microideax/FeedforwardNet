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
    //fc kernel function with tensor input
    void fc_kernel_t(
            tensor_t& in_data,
            tensor_t& kernel_weights,
            tensor_t& out_data) {

        out_data.clear();
        vec_t vec2;//output row vector
        for (int i = 0; i < _INPUT_SIZE_; i++)
        {
            for (int j = 0; j < _INPUT_SIZE_; j++)
            {
                float sum = 0;
                float data = in_data[i][j];
                float weight = kernel_weights[i][j];
                sum += data * weight;
                vec2.push_back(sum);
            }
            out_data.push_back(vec2);
            vec2.clear();
        }

#if _C_DEBUG_MODE_
        cout << "tensor fc kernel output ...." << endl;
        ofstream fc_kernel_t;
        fc_kernel_t.open("fc_kernel_t.txt", ios::app);
        fc_kernel_t << "fc kernel in data" << endl;
        for (int i = 0; i < in_data.size(); i++) {
            for (int j = 0; j < in_data[i].size(); j++) {
                fc_kernel_t << in_data[i][j] << " ";
            }
            fc_kernel_t << endl;
        }
        fc_kernel_t << endl;
        fc_kernel_t << "fc kernel in weight" << endl;
        for (int i = 0; i < kernel_weights.size(); i++) {
            for (int j = 0; j < kernel_weights[i].size(); j++) {
                fc_kernel_t << kernel_weights[i][j] << " ";
            }
            fc_kernel_t << endl;
        }
        fc_kernel_t << endl;
        fc_kernel_t << "fc kernel out data" << endl;
        for (int i = 0; i < out_data.size(); i++) {
            for (int j = 0; j < out_data[i].size(); j++) {
                fc_kernel_t << out_data[i][j] << " ";
            }
            fc_kernel_t << endl;
        }
        fc_kernel_t << endl;
        fc_kernel_t.close();
        cout << endl;
#endif
    }

    /************************************************************************************************/
    //fc layer function with with tensor input
    void fc_layer_t(
            char activation_type,
            tensor_t_3d& in_data3D,
            tensor_t_3d& kernel_weights,
            vec_t& kernel_bias,
            tensor_t_3d& out_data3D ) {


        cout << "starting fully_connect ...." << endl;
        out_data3D.clear();

        tensor_t out_data2D_plus;//每一个滤波器filter中所有卷积核卷积计算结果的累加结果
        float out_data2D_final_f;//每个像素加偏置、激活后的值
        vec_t out_data2D_final_v;//每个像素加偏置、激活后的值组成的行向量
        tensor_t out_data2D_final;//最终的输出矩阵

        for (int b = 0; b < _OUT_CHANNEL_NUM_; b++) {//output kernel loop
            int connection_num = 0;
            for (int a = 0; a < _IN_CHANNEL_NUM_; a++) {//input kernel loop
                tensor_t out_data2D;//每一个卷积计算的结果
                fc_kernel_t(in_data3D[a],
                              kernel_weights[a * _OUT_CHANNEL_NUM_ + b],//weight的存放顺序跟convolution层的不同
                              out_data2D);
                for (int i = 0; i < out_data2D.size(); i++) {
                    vec_t result_1;
                    if (connection_num == 0) {//第一个连接
                        for (int j = 0; j < out_data2D[i].size(); j++) {
                            result_1.push_back(0);//行向量的累加和初始置0
                        }
                    }
                    else if (connection_num != 0) {
                        for (int j = 0; j < out_data2D[i].size(); j++) {
                            result_1.push_back(out_data2D_plus[i][j]);//行向量的累加和
                        }
                    }
                    for (int r = 0; r < result_1.size(); r++) {
                        result_1[r] = result_1[r] + out_data2D[i][r];
                    }//行向量累加

                    out_data2D_plus.push_back(result_1);//把每个行向量累加结果放入out_data2D_plus，每次放入会增加10行
                }
                //将行向量累加放入out_data2D_plus中后，删除前10个行向量
                if (connection_num != 0) {
                    for (int i = 0; i < out_data2D.size(); i++) {
                        out_data2D_plus.erase(0);
                    }
                }
                connection_num++;
            }
            //循环遍历out_data2D_plus矩阵加偏置和激活
            for (int i = 0; i < out_data2D_plus.size(); i++) {
                for (int j = 0; j < out_data2D_plus[i].size(); j++) {
                    out_data2D_final_f = out_data2D_plus[i][j] + kernel_bias[b];

                    out_data2D_final_f = f(activation_type, out_data2D_final_f);//激活函数激活

                    out_data2D_final_v.push_back(out_data2D_final_f);//放入每个像素最终输出值
                }
                out_data2D_final.push_back(out_data2D_final_v);//把每个行向量累加结果放入out_data2D_final
                out_data2D_final_v.clear();
            }
            out_data3D.push_back(out_data2D_final);
            //out_data2D.clear();
            out_data2D_final.clear();
            out_data2D_plus.clear();
        }

        //debugging output
#if _C_DEBUG_MODE_
        cout << "finished fully_connect ...." << endl;
        ofstream fc_layer_t;
        fc_layer_t.open("fc_layer_t.txt", ios::app);
        for (int i = 0; i < out_data3D.size(); i++) {
            for (int j = 0; j < out_data3D[i].size(); j++) {
                for (int k = 0; k < out_data3D[i][j].size(); k++) {
                    fc_layer_t << out_data3D[i][j][k] << " ";
                }
                fc_layer_t << endl;
            }
            fc_layer_t << endl;
        }
        fc_layer_t.close();
        cout << endl;
#endif
    }

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
