//
// Created by yaochen on 29/12/16.
//TODO-1: modify conv function to compute input arrays' dimensions that are not equal
//TODO-2: add conv function with pooling (conv fusion pool)

#ifndef _CONV_POOL_LAYER_H_
#define _CONV_POOL_LAYER_H_

#include <iostream>
#include <fstream>
#include "activation_functions.h"
//#include "array_reset.h"

using namespace std;

extern const bool tbl[6][16];

template <typename T, typename W, typename G, int _INPUT_SIZE_, int _CONV_KERNEL_SIZE_, int _CONV_PADDING_, int _CONV_STRIDE_, int _POOL_KERNEL_SIZE_, int _POOL_PADDING_, int _POOL_STRIDE_, int _IN_CHANNEL_NUM_, int _OUT_CHANNEL_NUM_, int _GROUP_>
class conv_pool_layer {

private:
	int conv_layer_number;
	int out_data_size;
public:
	conv_pool_layer() :conv_layer_number(0) { out_data_size = (((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1) + _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1; };

	/************************************************************************************************/
	void conv_kernel_a(
		T *in_data,
		W *kernel_weights,
		G& out_data_unit_temp) {
		
		out_data_unit_temp += (*in_data) * (*kernel_weights);

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
		//int conv_layer_count = 0;
        //ofstream conv_kernel_a;
        //conv_kernel_a.open("conv_kernel_a.txt", ios::app);
		//conv_kernel_a << "input from conv layer .........................." << endl;
        //for (int j = 0; j < _INPUT_SIZE_ ; j++) {
        //    for (int k = 0; k < _INPUT_SIZE_ ; k++) {
        //        conv_kernel_a << *in_data << " "; // i?
        //    }
        //    conv_kernel_a << endl;
        //}
        //conv_kernel_a << endl;
        //for (int j = 0; j < _CONV_KERNEL_SIZE_; j++) {
        //    for (int k = 0; k < _CONV_KERNEL_SIZE_; k++) {
        //        conv_kernel_a << *kernel_weights << " "; // i?
        //   }
        //    conv_kernel_a << endl;
        //}
        //conv_kernel_a << endl;
        //for (int j = 0; j < _INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_ + 1; j++) {
        //    for (int k = 0; k < _INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_ + 1; k++) {
        //        conv_kernel_a << out_data[j][k] << " "; //
        //    }
        //    conv_kernel_a << endl;
        //}
        //conv_kernel_a << endl;
        //conv_kernel_a.close();
#endif
#endif
	}

	/************************************************************************************************/
	//3D array to 3D array convolution layer fuse pooling layer
	void conv_layer_w_pool_a(
		char activation_type,
		T *in_data3D,
		W *kernel_weights,
		W *kernel_bias,
		G *out_data3D) {
		//T out_data3d_temp[_OUT_CHANNEL_NUM_*((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1)
		//	*((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1)];
		////internal memory initiallization
		//array_reset(out_data3d_temp);

#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
		cout << "Starting convolution with pooling layer ...." << endl;
#endif
#endif
		G out_data_max = 0;
		G out_data_max_temp = 0;
		//convolution...
		for (int c = 0; c < _GROUP_; c++) {//group loop
			for (int b = c * _OUT_CHANNEL_NUM_ / _GROUP_; b < c * _OUT_CHANNEL_NUM_ / _GROUP_ + _OUT_CHANNEL_NUM_ / _GROUP_; b++) {//output kernel loop
				if (_CONV_KERNEL_SIZE_ % 2 != 0) {//_CONV_KERNEL_SIZE_ is an odd or even,the loop is different
					for (int m = 0;m<(((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1) 
						+ _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1;m++) {
						for (int n = 0; n<(((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1)
							+ _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1; n++) {
							int k = 0;
							for (int i = _CONV_KERNEL_SIZE_ / 2 - _CONV_PADDING_ + m * _POOL_STRIDE_ * _CONV_STRIDE_;
								k < _POOL_KERNEL_SIZE_&&(i < _INPUT_SIZE_ + _CONV_PADDING_ - _CONV_KERNEL_SIZE_ / 2); i += _CONV_STRIDE_) {
								int l = 0;
								for (int j = _CONV_KERNEL_SIZE_ / 2 - _CONV_PADDING_ + n * _POOL_STRIDE_ * _CONV_STRIDE_; 
									l < _POOL_KERNEL_SIZE_ && (j < _INPUT_SIZE_ + _CONV_PADDING_ - _CONV_KERNEL_SIZE_ / 2); j += _CONV_STRIDE_) {
									for (int ii = (-_CONV_KERNEL_SIZE_ / 2); ii <= _CONV_KERNEL_SIZE_ / 2; ii = ii + 1) {
										for (int jj = -_CONV_KERNEL_SIZE_ / 2; jj <= _CONV_KERNEL_SIZE_ / 2; jj = jj + 1) {
											if ((i + ii >= 0) && (i + ii < _INPUT_SIZE_) && (j + jj >= 0) && (j + jj < _INPUT_SIZE_)) {//if overlapped
												for (int a = c * _IN_CHANNEL_NUM_ / _GROUP_; a < c * _IN_CHANNEL_NUM_ / _GROUP_ + _IN_CHANNEL_NUM_ / _GROUP_; a++) {//input kernel loop
													conv_kernel_a(in_data3D + a*_INPUT_SIZE_*_INPUT_SIZE_ + _INPUT_SIZE_*(i + ii) + (j + jj),
														kernel_weights + (b * _IN_CHANNEL_NUM_ / _GROUP_ + a % (_IN_CHANNEL_NUM_ / _GROUP_))
														*_CONV_KERNEL_SIZE_*_CONV_KERNEL_SIZE_ + (ii + _CONV_KERNEL_SIZE_ / 2)*_CONV_KERNEL_SIZE_ + (jj + _CONV_KERNEL_SIZE_ / 2),
														out_data_max_temp);
												}
											}
										}
									}
#if _ACT_RELU_
                                    out_data_max_temp = Relu_64(out_data_max_temp + *(kernel_bias + b));
#else
									out_data_max_temp = f(activation_type, out_data_max_temp + *(kernel_bias + b));
#endif
									//max pooling...
									if (k == 0 && l == 0) {
										out_data_max = out_data_max_temp;
									}
									else {
										out_data_max = out_data_max_temp > out_data_max ? out_data_max_temp : out_data_max;
									}
									out_data_max_temp = 0;
									l++;
								}
								k++;
							}
							*(out_data3D + b*((((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1)
								+ _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1)*((((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1)
									+ _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1) + m*((((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1)
								+ _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1)+n) = out_data_max;
							out_data_max = 0;
						}
					}
				}
				else {
					for (int m = 0; m<(((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1)
						+ _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1; m++) {
						for (int n = 0; n<(((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1)
							+ _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1; n++) {
							int k = 0;
							for (int i = _CONV_KERNEL_SIZE_ / 2 - _CONV_PADDING_ + m * _POOL_STRIDE_ * _CONV_STRIDE_;
								k < _POOL_KERNEL_SIZE_ && (i <= _INPUT_SIZE_ + _CONV_PADDING_ - _CONV_KERNEL_SIZE_ / 2); i += _CONV_STRIDE_) {
								int l = 0;
								for (int j = _CONV_KERNEL_SIZE_ / 2 - _CONV_PADDING_ + n * _POOL_STRIDE_ * _CONV_STRIDE_;
									l < _POOL_KERNEL_SIZE_ && (j <= _INPUT_SIZE_ + _CONV_PADDING_ - _CONV_KERNEL_SIZE_ / 2); j += _CONV_STRIDE_) {
									for (int ii = -_CONV_KERNEL_SIZE_ / 2; ii < _CONV_KERNEL_SIZE_ / 2; ++ii) {
										for (int jj = -_CONV_KERNEL_SIZE_ / 2; jj < _CONV_KERNEL_SIZE_ / 2; ++jj) {
											if ((i + ii >= 0) && (i + ii < _INPUT_SIZE_) && (j + jj >= 0) && (j + jj < _INPUT_SIZE_)) {//if overlapped
												for (int a = c * _IN_CHANNEL_NUM_ / _GROUP_; a < c * _IN_CHANNEL_NUM_ / _GROUP_ + _IN_CHANNEL_NUM_ / _GROUP_; a++) {//input kernel loop
													conv_kernel_a(in_data3D + a*_INPUT_SIZE_*_INPUT_SIZE_ + _INPUT_SIZE_*(i + ii) + (j + jj),
														kernel_weights + (b * _IN_CHANNEL_NUM_ / _GROUP_ + a % (_IN_CHANNEL_NUM_ / _GROUP_))
														*_CONV_KERNEL_SIZE_*_CONV_KERNEL_SIZE_ + (ii + _CONV_KERNEL_SIZE_ / 2)*_CONV_KERNEL_SIZE_ + (jj + _CONV_KERNEL_SIZE_ / 2),
														out_data_max_temp);
												}
											}
										}
									}
#if _ACT_RELU_
                                    out_data_max_temp = Relu_64(out_data_max_temp + *(kernel_bias + b));
#else
									out_data_max_temp = f(activation_type, out_data_max_temp + *(kernel_bias + b));
#endif
									//max pooling...
									if (k == 0 && l == 0) {
										out_data_max = out_data_max_temp;
									}
									else {
										out_data_max = out_data_max_temp > out_data_max ? out_data_max_temp : out_data_max;
									}
									out_data_max_temp = 0;
									l++;
								}
								k++;
							}
							*(out_data3D + b*((((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1)
								+ _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1)*((((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1)
									+ _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1) + m*((((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1)
										+ _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1) + n) = out_data_max;
							out_data_max = 0;
						}
					}
				}
			}
		}		

		//debugging output
#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
		cout << "finished convolution and pooling...." << endl;
		ofstream out_pool_a;
		out_pool_a.open("conv_pool_layer_a.txt", ios::app);
		out_pool_a << "input 3D arry to conv and pooling layer ..................." << endl;
		for (int i = 0; i < _IN_CHANNEL_NUM_; i++) {
			for (int j = 0; j < _INPUT_SIZE_; j++) {
				for (int k = 0; k < _INPUT_SIZE_; k++) {
					out_pool_a << *(in_data3D + i*_INPUT_SIZE_ *_INPUT_SIZE_+ j*_INPUT_SIZE_ + k) << " ";
				}
				out_pool_a << endl;
			}
			out_pool_a << endl;
		}
		out_pool_a << endl;

		out_pool_a << "pooling layer out_data3D..................." << endl;
		for (int i = 0; i < _OUT_CHANNEL_NUM_; i++) {
			for (int j = 0; j < (((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1) + _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1; j++) {
				for (int k = 0; k < (((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1) + _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1; k++) {
					out_pool_a << *(out_data3D + i*((((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1) + _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1)
						*((((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1) + _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1)
						+ j*((((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1) + _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1) + k) << " ";
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

#endif
