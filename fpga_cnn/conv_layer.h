//
// Created by yaochen on 29/12/16.
//

#ifndef _CONV_LAYER_H_
#define _CONV_LAYER_H_

#include <iostream>
#include <fstream>
#include <algorithm>
#include "data_type.h"
#include "activation_functions.h"
#include "config.h"

using namespace std;

extern const bool tbl[6][16];

template <int _INPUT_SIZE_, int _KERNEL_SIZE_, int _IN_CHANNEL_NUM_, int _OUT_CHANNEL_NUM_>
class conv_layer {

private:
    int conv_layer_number;
    int out_data_size;
public:
    conv_layer():conv_layer_number(0){ out_data_size = _INPUT_SIZE_ - _KERNEL_SIZE_ + 1;};

    //convolution kernel function
    void convolution_kernel(
            tensor_t& in_data,
            tensor_t& kernel_weights,
            tensor_t& out_data) {

        out_data.clear();
        vec_t vec2;//output row vector
        for (int i = _KERNEL_SIZE_ / 2; i < _INPUT_SIZE_ - _KERNEL_SIZE_ / 2; ++i)
        {
            for (int j = _KERNEL_SIZE_ / 2; j < _INPUT_SIZE_ - _KERNEL_SIZE_ / 2; ++j)
            {
                #ifdef _HLS_MODE_
                #pragma HLS PIPELINE
                #endif
                float sum = 0;
                for (int ii = - _KERNEL_SIZE_ / 2; ii <= _KERNEL_SIZE_ / 2; ++ii)
                {
                #ifdef _HLS_MODE_
                #pragma HLS UNROLL
                #endif
                    for (int jj = -_KERNEL_SIZE_ / 2; jj <= _KERNEL_SIZE_ / 2; ++jj)
                    {
                        #ifdef _HLS_MODE_
                        #pragma HLS UNROLL
                        #endif
                        float data = in_data[i + ii][j + jj];
                        float weight = kernel_weights[ii + _KERNEL_SIZE_ / 2][jj + _KERNEL_SIZE_ / 2];
                        sum += data * weight;
                    }
                }
                vec2.push_back(sum);
            }
            out_data.push_back(vec2);
            vec2.clear();
        }
    }

    //tensor to tensor convolution layer with connection table
    void convolution_layer_with_table(
            char activation_type,
            tensor_t_3d& in_data3D,  //in_data[6][
            bool has_connection_table,
            tensor_t_3d& kernel_weights,
            vec_t& kernel_bias,
            tensor_t_3d& out_data3D ) {

        cout << "starting convolution ...." << endl;

        tensor_t out_data2D_plus;
        float out_data2D_final_f;
        vec_t out_data2D_final_v;
        tensor_t out_data2D_final;

        for (int b = 0; b < _OUT_CHANNEL_NUM_; b++) {//output kernel loop
            int connection_num = 0;
            for (int a = 0; a < _IN_CHANNEL_NUM_; a++) {//input kernel loop
                if (has_connection_table) {
                    if (tbl[a][b]) {
                        tensor_t out_data2D;
                        convolution_kernel(in_data3D[a],
                                           kernel_weights[b * _IN_CHANNEL_NUM_ + a],
                                           out_data2D);
                        for (int i = 0; i < out_data2D.size(); i++) {
//						vector<float> result_1;
                            vec_t result_1;
                            if (connection_num == 0) {
                                for (int j = 0; j < out_data2D[i].size(); j++) {
                                    result_1.push_back(0);
                                }
                            }
                            else if (connection_num != 0) {
                                for (int j = 0; j < out_data2D[i].size(); j++) {
                                    result_1.push_back(out_data2D_plus[i][j]);
                                }
                            }
                            for (int r = 0; r < result_1.size(); r++){
                                result_1[r] = result_1[r] + out_data2D[i][r];
                            }
                            out_data2D_plus.push_back(result_1);
                        }
                        if (connection_num != 0) {
                            //static vectors
                            for (int i = 0; i < out_data2D.size(); i++) {
                                out_data2D_plus.erase(0);
                            }
                        }
                        connection_num++;
                    }
                }
                else if (!has_connection_table) {
                    tensor_t out_data2D;
                    convolution_kernel(in_data3D[a],
                                       kernel_weights[b * _IN_CHANNEL_NUM_ + a],
                                       out_data2D);
                    for (int i = 0; i < out_data2D.size(); i++) {
                        vec_t result_1;
                        if (connection_num == 0) {
                            for (int j = 0; j < out_data2D[i].size(); j++) {
                                result_1.push_back(0);
                            }
                        }
                        else if (connection_num != 0) {
                            for (int j = 0; j < out_data2D[i].size(); j++) {
                                result_1.push_back(out_data2D_plus[i][j]);
                            }
                        }
                        for (int r = 0; r < result_1.size(); r++){
                            result_1[r] = result_1[r] + out_data2D[i][r];
                        }
                        out_data2D_plus.push_back(result_1);
                    }
                    if (connection_num != 0) {
                        for (int i = 0; i < out_data2D.size(); i++) {
                            out_data2D_plus.erase(0);
                        }
                    }
                    connection_num++;
                }
            }

            for (int i = 0; i < out_data2D_plus.size(); i++) {
                for (int j = 0; j < out_data2D_plus[i].size(); j++) {
                    out_data2D_final_f = out_data2D_plus[i][j] + kernel_bias[b];

                    out_data2D_final_f = f(activation_type, out_data2D_final_f);//

                    out_data2D_final_v.push_back(out_data2D_final_f);//
                }
                out_data2D_final.push_back(out_data2D_final_v);//
                out_data2D_final_v.clear();
            }
            out_data3D.push_back(out_data2D_final);
            out_data2D_final.clear();
            out_data2D_plus.clear();
        }


        //debugging output
        #if _C_DEBUG_MODE_
        cout << "finished convolution ...." << endl;
	    ofstream out_conv_class;
        out_conv_class.open("out_conv_class.txt", ios::app);
	    for (int i = 0; i < out_data3D.size(); i++) {
		    for (int j = 0; j < out_data3D[i].size(); j++) {
			    for (int k = 0; k < out_data3D[i][j].size(); k++) {
				    out_conv_class << out_data3D[i][j][k] << " ";
			    }
			    out_conv_class << endl;
		    }
		    out_conv_class << endl;
	    }
	    out_conv_class.close();
	    cout << endl;
        #endif

    }

    void conv_kernel_array(
            float in_data[][_INPUT_SIZE_],
            float kernel_weights[][_KERNEL_SIZE_],
            float out_data[][_INPUT_SIZE_ - _KERNEL_SIZE_ + 1]) {

        for (int i = _KERNEL_SIZE_ / 2; i < _INPUT_SIZE_ - _KERNEL_SIZE_ / 2; ++i)
        {
            for (int j = _KERNEL_SIZE_ / 2; j < _INPUT_SIZE_ - _KERNEL_SIZE_ / 2; ++j)
            {
                for (int ii = - _KERNEL_SIZE_ / 2; ii <= _KERNEL_SIZE_ / 2; ++ii)
                {
                    for (int jj = -_KERNEL_SIZE_ / 2; jj <= _KERNEL_SIZE_ / 2; ++jj)
                    {
                        float data = in_data[i + ii][j + jj];
                        float weight = kernel_weights[ii + _KERNEL_SIZE_ / 2][jj + _KERNEL_SIZE_ / 2];
                        out_data[i - _KERNEL_SIZE_ / 2 ][j - _KERNEL_SIZE_ / 2] += data * weight;
                    }
                }
            }
        }
    }

    //3D array to 3D array convolution layer with connection table
    void conv_layer_array(
            char activation_type,
            float in_data3D[][_INPUT_SIZE_][_INPUT_SIZE_],  //in_data[6][
            bool has_connection_table,
            float kernel_weights[][_KERNEL_SIZE_][_KERNEL_SIZE_],
            float kernel_bias[],
            float out_data3D[][_INPUT_SIZE_ - _KERNEL_SIZE_ + 1][_INPUT_SIZE_ - _KERNEL_SIZE_ + 1] ) {

        cout << "starting convolution ...." << endl;

        for (int b = 0; b < _OUT_CHANNEL_NUM_; b++) {//output kernel loop
            for (int a = 0; a < _IN_CHANNEL_NUM_; a++) {//input kernel loop
                float out_data2D[_INPUT_SIZE_ - _KERNEL_SIZE_ + 1][_INPUT_SIZE_ - _KERNEL_SIZE_ + 1];
                conv_kernel_array(in_data3D[a],
                                   kernel_weights[a * b + a],
                                   out_data2D);
//                out_data3D[b] += out_data2D;
                for (int i = 0; i < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; i++){
                    for (int j = 0; j < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; j++){
                        out_data3D[b][i][j] += out_data2D[i][j];
                    }
                }
            }
            for (int i = 0; i < _OUT_CHANNEL_NUM_; i++){
                for(int j = 0; j < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; j++){
                    for (int k = 0; k < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; k++){
                        out_data3D[i][j][k] = f(activation_type, (out_data3D[i][j][k] + kernel_bias[i]));
                    }
                }
            }

    }

    //debugging output
#if _C_DEBUG_MODE_
        cout << "finished convolution ...." << endl;
        ofstream out_conv_a;
        out_conv_a.open("out_conv_a.txt", ios::app);
        for (int i = 0; i < _OUT_CHANNEL_NUM_; i++) {
            for (int j = 0; j < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; j++) {
                for (int k = 0; k < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; k++) {
                    out_conv_a << out_data3D[i][j][k] << " ";
                }
                out_conv_a << endl;
            }
            out_conv_a << endl;
        }
        out_conv_a.close();
        cout << endl;
#endif

    }
};

#endif //FFNET_CONV_LAYER_H
