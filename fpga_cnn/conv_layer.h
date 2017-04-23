//
// Created by yaochen on 29/12/16.
//TODO-1: modify conv function to compute input arrays' dimensions that are not equal
//TODO-2: add conv function with pooling (conv fusion pool)

#ifndef _CONV_LAYER_H_
#define _CONV_LAYER_H_

#include <iostream>
#include <fstream>
#include "activation_functions.h"
//#include "config.h"

using namespace std;

extern const bool tbl[6][16];

template <typename T, typename W, typename G, int _INPUT_SIZE_, int _CONV_KERNEL_SIZE_, int _CONV_PADDING_, int _CONV_STRIDE_, int _IN_CHANNEL_NUM_, int _OUT_CHANNEL_NUM_, int _GROUP_>
class conv_layer {

private:
    int conv_layer_number;
    int out_data_size;
public:
    conv_layer() : conv_layer_number(0) { out_data_size = (_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1;
    };

    /************************************************************************************************/
    void conv_kernel_a(
            T in_data[_INPUT_SIZE_][_INPUT_SIZE_],
            W kernel_weights[_CONV_KERNEL_SIZE_][_CONV_KERNEL_SIZE_],
            G out_data[(_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1][(_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1]) {

        T in_data_par[_INPUT_SIZE_][_INPUT_SIZE_] = {0};
        W kernel_weights_par[_CONV_KERNEL_SIZE_][_CONV_KERNEL_SIZE_] = {0};

        for (uint i = 0; i < _INPUT_SIZE_; i++) {
            for (uint j = 0; j < _INPUT_SIZE_; j++) {
                in_data_par[i][j] = in_data[i][j];
            }
        }
        for (uint i = 0; i < _CONV_KERNEL_SIZE_; i++) {
            for (uint j = 0; j < _CONV_KERNEL_SIZE_; j++) {
                kernel_weights_par[i][j] = kernel_weights[i][j];
            }
        }

		if (_CONV_KERNEL_SIZE_ % 2 != 0) {//_CONV_KERNEL_SIZE_ is an odd or even,the loop is different
			for (int i = _CONV_KERNEL_SIZE_ / 2 - _CONV_PADDING_; i < _INPUT_SIZE_ + _CONV_PADDING_ - _CONV_KERNEL_SIZE_ / 2; i += _CONV_STRIDE_) {
                for (int j = _CONV_KERNEL_SIZE_ / 2 - _CONV_PADDING_; j < _INPUT_SIZE_ + _CONV_PADDING_ - _CONV_KERNEL_SIZE_ / 2; j += _CONV_STRIDE_) {
                    for (int ii = (-_CONV_KERNEL_SIZE_ / 2); ii <= _CONV_KERNEL_SIZE_ / 2; ii = ii + 1) {
                        for (int jj = -_CONV_KERNEL_SIZE_ / 2; jj <= _CONV_KERNEL_SIZE_ / 2; jj = jj + 1) {
                            if ((i + ii >= 0) && (i + ii < _INPUT_SIZE_) && (j + jj >= 0) &&
                                (j + jj < _INPUT_SIZE_)) {//if overlapped
                                T data = in_data_par[i + ii][j + jj];
                                W weight = kernel_weights_par[ii + _CONV_KERNEL_SIZE_ / 2][jj + _CONV_KERNEL_SIZE_ / 2];
                                out_data[(i - _CONV_KERNEL_SIZE_ / 2 + _CONV_PADDING_) / _CONV_STRIDE_][
                                        (j - _CONV_KERNEL_SIZE_ / 2 + _CONV_PADDING_) / _CONV_STRIDE_] += data * weight;
                            }
                        }
                    }
                }
            }
		}
		else {
			for (int i = _CONV_KERNEL_SIZE_ / 2 - _CONV_PADDING_; i <= _INPUT_SIZE_ + _CONV_PADDING_ - _CONV_KERNEL_SIZE_ / 2; i += _CONV_STRIDE_) {
				for (int j = _CONV_KERNEL_SIZE_ / 2 - _CONV_PADDING_; j <= _INPUT_SIZE_ + _CONV_PADDING_ - _CONV_KERNEL_SIZE_ / 2; j += _CONV_STRIDE_) {
					for (int ii = -_CONV_KERNEL_SIZE_ / 2; ii < _CONV_KERNEL_SIZE_ / 2; ++ii) {
						for (int jj = -_CONV_KERNEL_SIZE_ / 2; jj < _CONV_KERNEL_SIZE_ / 2; ++jj) {
							if (i + ii >= 0 && i + ii< _INPUT_SIZE_ && j + jj >= 0 && j + jj<_INPUT_SIZE_) {//if overlapped
								T data = in_data_par[i + ii][j + jj];
								W weight = kernel_weights_par[ii + _CONV_KERNEL_SIZE_ / 2][jj + _CONV_KERNEL_SIZE_ / 2];
								out_data[(i - _CONV_KERNEL_SIZE_ / 2 + _CONV_PADDING_) / _CONV_STRIDE_][(j - _CONV_KERNEL_SIZE_ / 2 + _CONV_PADDING_) / _CONV_STRIDE_] += data * weight;
							}
						}
					}
				}
			}
		}

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        //int conv_layer_count = 0;
        ofstream conv_kernel_a;
        conv_kernel_a.open("conv_kernel_a.txt", ios::app);
        //for (int j = 0; j < _INPUT_SIZE_ ; j++) {
        //    for (int k = 0; k < _INPUT_SIZE_ ; k++) {
        //        conv_kernel_a << in_data[j][k] << " "; // i?
        //    }
        //    conv_kernel_a << endl;
        //}
        //conv_kernel_a << endl;
        for (int j = 0; j < _CONV_KERNEL_SIZE_; j++) {
            for (int k = 0; k < _CONV_KERNEL_SIZE_; k++) {
                conv_kernel_a << kernel_weights[j][k] << " "; // i?
            }
        //    conv_kernel_a << endl;
        }
        //conv_kernel_a << endl;
        //for (int j = 0; j < _INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_ + 1; j++) {
         //   for (int k = 0; k < _INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_ + 1; k++) {
         //       conv_kernel_a << out_data[j][k] << " "; //
         //   }
        //    conv_kernel_a << endl;
        //}
        //conv_kernel_a << endl;
        conv_kernel_a.close();
#endif
#endif
    }

    /************************************************************************************************/
    //3D array to 3D array convolution layer without connection table
    void conv_layer_a(
            char activation_type,
            T in_data3D[_IN_CHANNEL_NUM_][_INPUT_SIZE_][_INPUT_SIZE_],
            W kernel_weights[_IN_CHANNEL_NUM_ * _OUT_CHANNEL_NUM_][_CONV_KERNEL_SIZE_][_CONV_KERNEL_SIZE_],
            W kernel_bias[_OUT_CHANNEL_NUM_],
            G out_data3D[_OUT_CHANNEL_NUM_][(_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1][(_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1]) {

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        cout << "Starting convolution layer ...." << endl;
#endif
#endif

		for (int c = 0; c < _GROUP_; c++) {//group loop
			for (int b = c * _OUT_CHANNEL_NUM_ / _GROUP_; b < c * _OUT_CHANNEL_NUM_ / _GROUP_ + _OUT_CHANNEL_NUM_ / _GROUP_; b++) {//output kernel loop
				for (int a = c * _IN_CHANNEL_NUM_ / _GROUP_; a < c * _IN_CHANNEL_NUM_ / _GROUP_ + _IN_CHANNEL_NUM_ / _GROUP_; a++) {//input kernel loop
					conv_kernel_a(in_data3D[a],
						kernel_weights[b * _IN_CHANNEL_NUM_ / _GROUP_ + a % (_IN_CHANNEL_NUM_ / _GROUP_)],
						//kernel_weights[b * _IN_CHANNEL_NUM_ + a],
						out_data3D[b]);
				}
				for (int j = 0; j < (_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1; j++) {
					for (int k = 0; k < (_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1; k++) {
#if _ACT_RELU_
                        out_data3D[b][j][k] = Relu_32((out_data3D[b][j][k] + kernel_bias[b]));
#else
						out_data3D[b][j][k] = f(activation_type, (out_data3D[b][j][k] + kernel_bias[b]));
#endif
					}
				}
			}
		}

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
        cout << "Finished convolution layer ...." << endl;
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

        out_conv_a << "output from conv layer .........................." << endl;
        for (int i = 0; i < _OUT_CHANNEL_NUM_; i++) {
            for (int j = 0; j < (_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1; j++) {
                for (int k = 0; k < (_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1; k++) {
                    out_conv_a << out_data3D[i][j][k] << " ";
                }
                out_conv_a << endl;
            }
            out_conv_a << endl;
        }
        out_conv_a.close();
        cout << endl;
#endif
#endif

    }

    /************************************************************************************************/
    // convolution layer with array input and connection table
    void conv_layer_a_table(
            char activation_type,
            bool has_connection_table,
            T in_data3D[][_INPUT_SIZE_][_INPUT_SIZE_],  //in_data[6][
            T kernel_weights[][_CONV_KERNEL_SIZE_][_CONV_KERNEL_SIZE_],
            T kernel_bias[],
            T out_data3D[][_INPUT_SIZE_ - _CONV_KERNEL_SIZE_ + 1][_INPUT_SIZE_ - _CONV_KERNEL_SIZE_ + 1]) {

#if _C_DEBUG_MODE_
        cout << "starting convolution ...." << endl;
#endif

        for (int b = 0; b < _OUT_CHANNEL_NUM_; b++) {//output kernel loop
            for (int a = 0; a < _IN_CHANNEL_NUM_; a++) {//input kernel loop
                if (tbl[a][b]) {
                    conv_kernel_a(in_data3D[a],
                                  kernel_weights[b * _IN_CHANNEL_NUM_ + a],
						          out_data3D[b]);
                } else { ; }
            }
            for (int j = 0; j < (_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1; j++) {
                for (int k = 0; k < (_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1; k++) {
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
            for (int j = 0; j < (_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1; j++) {
                for (int k = 0; k < (_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1; k++) {
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
