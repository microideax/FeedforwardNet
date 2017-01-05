//
// Created by yaochen on 29/12/16.
//TODO: modify conv function to compute input arrays' dimensions are not equal

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

    /************************************************************************************************/
    void conv_kernel_a(
            float in_data[][_INPUT_SIZE_],
            float kernel_weights[][_KERNEL_SIZE_],
            float out_data[][_INPUT_SIZE_ - _KERNEL_SIZE_ + 1]) {

        for (int i = _KERNEL_SIZE_ / 2; i < _INPUT_SIZE_ - _KERNEL_SIZE_ / 2; ++i) {
            for (int j = _KERNEL_SIZE_ / 2; j < _INPUT_SIZE_ - _KERNEL_SIZE_ / 2; ++j) {
                for (int ii = - _KERNEL_SIZE_ / 2; ii <= _KERNEL_SIZE_ / 2; ++ii) {
                    for (int jj = -_KERNEL_SIZE_ / 2; jj <= _KERNEL_SIZE_ / 2; ++jj) {
                        float data = in_data[i + ii][j + jj];
                        float weight = kernel_weights[ii + _KERNEL_SIZE_ / 2][jj + _KERNEL_SIZE_ / 2];
                        out_data[i - _KERNEL_SIZE_ / 2 ][j - _KERNEL_SIZE_ / 2] += data * weight;
                    }
                }
            }
        }

        #if _C_DEBUG_MODE_
        int conv_layer_count = 0;
        cout << "array conv kernel output ...." << endl;
        ofstream conv_kernel_a;
        conv_kernel_a.open("conv_kernel_a.txt", ios::app);
        for (int j = 0; j < _INPUT_SIZE_ ; j++) {
            for (int k = 0; k < _INPUT_SIZE_ ; k++) {
                conv_kernel_a << in_data[j][k] << " "; // i?
            }
            conv_kernel_a << endl;
        }
        conv_kernel_a << endl;
        for (int j = 0; j < _KERNEL_SIZE_ ; j++) {
            for (int k = 0; k < _KERNEL_SIZE_ ; k++) {
                conv_kernel_a << kernel_weights[j][k] << " "; // i?
            }
            conv_kernel_a << endl;
        }
        conv_kernel_a << endl;
        for (int j = 0; j < _INPUT_SIZE_ - _KERNEL_SIZE_ +1 ; j++) {
            for (int k = 0; k < _INPUT_SIZE_ - _KERNEL_SIZE_ +1; k++) {
                conv_kernel_a << out_data[j][k] << " "; //
            }
            conv_kernel_a << endl;
        }
        conv_kernel_a << endl;
        conv_kernel_a.close();
        cout << endl;
        #endif
    }

    /************************************************************************************************/
    //3D array to 3D array convolution layer with connection table
    void conv_layer_a(
            char activation_type,
            float in_data3D[_IN_CHANNEL_NUM_][_INPUT_SIZE_][_INPUT_SIZE_],
            float kernel_weights[][_KERNEL_SIZE_][_KERNEL_SIZE_],
            float kernel_bias[],
            float out_data3D[][_INPUT_SIZE_ - _KERNEL_SIZE_ + 1][_INPUT_SIZE_ - _KERNEL_SIZE_ + 1] ) {

        cout << "starting convolution ...." << endl;

        for (int b = 0; b < _OUT_CHANNEL_NUM_; b++) {//output kernel loop
            for (int a = 0; a < _IN_CHANNEL_NUM_; a++) {//input kernel loop
                float out_data2D[_INPUT_SIZE_ - _KERNEL_SIZE_ + 1][_INPUT_SIZE_ - _KERNEL_SIZE_ + 1] = {0};
                conv_kernel_a(in_data3D[a],
                              kernel_weights[b * _IN_CHANNEL_NUM_ + a],
                              out_data2D);

                for (int i = 0; i < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; i++){
                    for (int j = 0; j < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; j++){
                        out_data3D[b][i][j] += out_data2D[i][j];
                    }
                }
            }
            for(int j = 0; j < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; j++){
                for (int k = 0; k < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; k++){
                    out_data3D[b][j][k] = f(activation_type, (out_data3D[b][j][k] + kernel_bias[b]));
                }
            }
    }

        //debugging output
        #if _C_DEBUG_MODE_
        cout << "finished convolution ...." << endl;
        ofstream out_conv_a;
        out_conv_a.open("conv_layer_a.txt", ios::app);
        out_conv_a << "input 3D arry to conv layer ..................." << endl;
        for (int i = 0; i < _IN_CHANNEL_NUM_; i++) {
            for (int j = 0; j < _INPUT_SIZE_; j++) {
                for (int k = 0; k < _INPUT_SIZE_; k++) {
                    out_conv_a << in_data3D[i][j][k] << " ";
                }
                out_conv_a << endl;
            }
            out_conv_a << endl;
        }
        out_conv_a << endl;

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

    /************************************************************************************************/
    // convolution layer with array input and connection table
    void conv_layer_a_table(
            char activation_type,
            bool has_connection_table,
            float in_data3D[][_INPUT_SIZE_][_INPUT_SIZE_],  //in_data[6][
            float kernel_weights[][_KERNEL_SIZE_][_KERNEL_SIZE_],
            float kernel_bias[],
            float out_data3D[][_INPUT_SIZE_ - _KERNEL_SIZE_ + 1][_INPUT_SIZE_ - _KERNEL_SIZE_ + 1] ) {

        cout << "starting convolution ...." << endl;

        for (int b = 0; b < _OUT_CHANNEL_NUM_; b++) {//output kernel loop
            for (int a = 0; a < _IN_CHANNEL_NUM_; a++) {//input kernel loop
                if (tbl[a][b]) {
                    float out_data2D[_INPUT_SIZE_ - _KERNEL_SIZE_ + 1][_INPUT_SIZE_ - _KERNEL_SIZE_ + 1] = {0};
                    conv_kernel_a(in_data3D[a],
                                  kernel_weights[b * _IN_CHANNEL_NUM_ + a],
                                  out_data2D);

                    for (int i = 0; i < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; i++){
                        for (int j = 0; j < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; j++){
                            out_data3D[b][i][j] += out_data2D[i][j];
                        }
                    }
                }
                else {;}
            }
            for(int j = 0; j < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; j++){
                for (int k = 0; k < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; k++){
                    out_data3D[b][j][k] = f(activation_type, (out_data3D[b][j][k] + kernel_bias[b]));
                }
            }
        }

        //debugging output
#if _C_DEBUG_MODE_
        cout << "finished convolution ...." << endl;
        ofstream out_conv_a;
        out_conv_a.open("conv_layer_a.txt", ios::app);

        out_conv_a << "input 3D arry to conv layer ..................." << endl;
        for (int i = 0; i < _IN_CHANNEL_NUM_; i++) {
            for (int j = 0; j < _INPUT_SIZE_; j++) {
                for (int k = 0; k < _INPUT_SIZE_; k++) {
                    out_conv_a << in_data3D[i][j][k] << " ";
                }
                out_conv_a << endl;
            }
            out_conv_a << endl;
        }
        out_conv_a << endl;

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

#endif
