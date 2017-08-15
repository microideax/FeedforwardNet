//
// Created by yaochen on 29/12/16.
//
//TODO: if conv_layer use relu, then max_pooling do not need to use relu again. Need to modify this part of code.

#ifndef _POOLING_LAYER_H_
#define _POOLING_LAYER_H_

#include <iostream>
#include <fstream>
#include "activation_functions.h"
//#include "config.h"

//using namespace std;

template <typename T, typename W, typename G, int _INPUT_SIZE_, int _POOL_KERNEL_SIZE_, int _POOL_PADDING_, int _POOL_STRIDE_, int _IN_CHANNEL_NUM_>
class pool_layer_buf {

private:
	int pooling_layer_number;

public:
	pool_layer_buf() :pooling_layer_number(0) {};

    /************************************************************************************/
    // max pooling kernel function with array input without kernel weights
    void max_pooling_kernel_buf(
            T in_data[BUF_1_Y_DIM][BUF_1_Z_DIM],
            G out_data[][BUF_1_Z_DIM]) {

        if (_POOL_KERNEL_SIZE_ % 2 != 0) {//_POOL_KERNEL_SIZE_ is an odd or even,the loop is different
            for (int i = _POOL_KERNEL_SIZE_ / 2 - _POOL_PADDING_; i < _INPUT_SIZE_ + _POOL_PADDING_ - _POOL_KERNEL_SIZE_ / 2; i += _POOL_STRIDE_) {
                for (int j = _POOL_KERNEL_SIZE_ / 2 - _POOL_PADDING_; j < _INPUT_SIZE_ + _POOL_PADDING_ - _POOL_KERNEL_SIZE_ / 2; j += _POOL_STRIDE_) {
                    T max = 0;
                    for (int ii = -_POOL_KERNEL_SIZE_ / 2; ii <= _POOL_KERNEL_SIZE_ / 2; ++ii) {
                        for (int jj = -_POOL_KERNEL_SIZE_ / 2; jj <= _POOL_KERNEL_SIZE_ / 2; ++jj) {
                            if (i + ii >= 0 && i + ii< _INPUT_SIZE_
                                && j + jj >= 0 && j + jj< _INPUT_SIZE_) {//if overlapped
                                max = (in_data[i + ii][j + jj] > max ? in_data[i + ii][j + jj] : max);
                            }
                        }
                    }
                    out_data[(i - _POOL_KERNEL_SIZE_ / 2 + _POOL_PADDING_) / _POOL_STRIDE_][(j - _POOL_KERNEL_SIZE_ / 2 + _POOL_PADDING_) / _POOL_STRIDE_] = max;
                }
            }
        }
        else {
            for (int i = _POOL_KERNEL_SIZE_ / 2 - _POOL_PADDING_; i <= _INPUT_SIZE_ + _POOL_PADDING_ - _POOL_KERNEL_SIZE_ / 2; i += _POOL_STRIDE_) {
                for (int j = _POOL_KERNEL_SIZE_ / 2 - _POOL_PADDING_; j <= _INPUT_SIZE_ + _POOL_PADDING_ - _POOL_KERNEL_SIZE_ / 2; j += _POOL_STRIDE_) {
                    T max = 0;
                    for (int ii = -_POOL_KERNEL_SIZE_ / 2; ii < _POOL_KERNEL_SIZE_ / 2; ++ii) {
                        for (int jj = -_POOL_KERNEL_SIZE_ / 2; jj < _POOL_KERNEL_SIZE_ / 2; ++jj) {
                            if (i + ii >= 0 && i + ii< _INPUT_SIZE_
                                && j + jj >= 0 && j + jj< _INPUT_SIZE_) {//if overlapped
                                max = (in_data[i + ii][j + jj] > max ? in_data[i + ii][j + jj] : max);
                            }
                        }
                    }
                    out_data[(i - _POOL_KERNEL_SIZE_ / 2 + _POOL_PADDING_) / _POOL_STRIDE_][(j - _POOL_KERNEL_SIZE_ / 2 + _POOL_PADDING_) / _POOL_STRIDE_] = max;
                }
            }
        }

#if _C_DEBUG_MODE_
        #if _KERNEL_DEBUG_
//		cout << "pooling kernel buf input array...." << endl;
		ofstream pool_kernel_a;
		pool_kernel_a.open("pool_kernel_a.txt", ios::app);
		pool_kernel_a << "pooling kernel a input data" << endl;
		for (int i = 0; i < _INPUT_SIZE_; i++) {
			for (int j = 0; j < _INPUT_SIZE_; j++) {
				pool_kernel_a << in_data[i][j] << " ";
			}
			pool_kernel_a << endl;
		}
		pool_kernel_a << endl;

		pool_kernel_a << "pooling kernel a output data" << endl;
		for (int i = 0; i < (_INPUT_SIZE_ + _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1; i++) {
			for (int j = 0; j < (_INPUT_SIZE_ + _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1; j++) {
				pool_kernel_a << out_data[i][j] << " ";
			}
			pool_kernel_a << endl;
		}
		pool_kernel_a.close();
//		cout << endl;
#endif
#endif
    }

    /************************************************************************************/
    //max pooling layer function with array input without pooling weights
    void max_pooling_layer_buf(
            char activation_type,
            T in_data3D[BUF_1_X_DIM][BUF_1_Y_DIM][BUF_1_Z_DIM],
            G out_data3D[BUF_1_X_DIM][BUF_1_Y_DIM][BUF_1_Z_DIM]) {

#if _C_DEBUG_MODE_
        #if _KERNEL_DEBUG_
		cout << "Starting Max_pooling layer with buf ...." << endl;
#endif
#endif

        for (int a = 0; a < _IN_CHANNEL_NUM_; a++) {//input kernel loop
            max_pooling_kernel_buf(
                    in_data3D[a],
                    out_data3D[a]);
            //循环遍历out_data2D矩阵加偏置和激活
            for (int i = 0; i < (_INPUT_SIZE_ + _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1; i++) {
                for (int j = 0; j < (_INPUT_SIZE_ + _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1; j++) {
#if _ACT_RELU_
                    out_data3D[a][i][j] = Relu_64(out_data3D[a][i][j]);
#else
                    out_data3D[a][i][j] = f(activation_type, out_data3D[a][i][j]);
#endif
                }
            }
        }

#if _C_DEBUG_MODE_
        #if _KERNEL_DEBUG_
		cout << "Finished Max_pooling layer with buf ...." << endl;
        //debugging output
		ofstream out_pool_a;
		out_pool_a.open("pool_layer_a.txt", ios::app);
		out_pool_a << "output from pool layer .........................." << endl;
		for (int i = 0; i < _IN_CHANNEL_NUM_; i++) {
			for (int j = 0; j < (_INPUT_SIZE_ + _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1; j++) {
				for (int k = 0; k < (_INPUT_SIZE_ + _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1; k++) {
					out_pool_a << out_data3D[i][j][k] << " ";
				}
				out_pool_a << endl;
			}
			out_pool_a << endl;
		}
		out_pool_a.close();
		cout << endl;
#endif
#endif
    }
};

#endif //_POOLING_LAYER_H
