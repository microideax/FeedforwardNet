//This file contains the popular activation functions used in CNNs
//TODO: modify the commented function to be compatible with gcc compilation.
//TODO: change the functions into class based expression.

#ifndef _ACTIVATION_FUNCTIONS_H_
#define _ACTIVATION_FUNCTIONS_H_

#include <math.h>

//using namespace std;
/*
identity = i;
sigmod   = s;
relu     = r;
leaky_relu = l;
elu      = e;
tan_h    = t;
tan_hp1m2 = h;
*/

float f(char type, float data) {
	if (type == 'i') // identity
	{
		return data;
	}
	else if (type == 's') { // sigmod
		return float(1) / (float(1) + exp(-data));
	}
	else if (type == 'r') { //relu
		return ((float(0) > data) ? float(0) : data);
	}
	else if (type == 'l') { //leak_relu
		return (data > float(0)) ? data : float(0.01) * data;
	}
	else if (type == 'e') { // elu
		return (data<float(0) ? (exp(data) - float(1)) : data);
	}
	else if (type == 't') { // tanh
		const float ep = exp(data);
		const float em = exp(-data);
		return (ep - em) / (ep + em);
//		return tanh(data);
	}
	else if (type == 'h') { // tan_hp1m2
		const float ep = exp(data);
		return ep / (ep + exp(-data));
	}
    else return false;
}

//float f(string& type, const vec_t& v, int i) {
//	if (type == "softmax")
//	{
//		float alpha = *std::max_element(v.begin(), v.end());
//		float numer = exp(v[i] - alpha);
//		float denom = float(0);
//		for (uint i = 0; i < v.size(); i++) {
//			denom += exp(v[i] - alpha);
//		}
//		return numer / denom;
//	}
//    else return false;
//}
/*
float df(char& type, float data) {
	if (type == "identity")
	{
		return float(1);
	}
	else if (type == "sigmoid") {
		return data * (float(1) - data);
	}
	else if (type == "relu") {
		return data > float(0) ? float(1) : float(0);
	}
	else if (type == "leaky_relu") {
		return data > float(0) ? float(1) : float(0.01);
	}
	else if (type == "elu") {
		return (data > float(0) ? float(1) : (float(1) + data));
	}
	else if (type == "tan_h") {
		return float(1) - data*data;
	}
	else if (type == "tan_hp1m2") {
		return 2 * data *(float(1) - data);
	}
    else return false;
}
*/
//vec_t df(string& type, const vec_t& y, uint index) {
//    vec_t v(0, 0);
//    if (type == "softmax")
//	{
//		vec_t v(y.size(), 0);
//		for (uint i = 0; i < y.size(); i++)
//			v[i] = (i == index) ? y[index] * (float(1) - y[index]) : -y[i] * y[index];
//
//		return v;
//	}
//    else return v;
//}

#endif
