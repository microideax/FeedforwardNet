//Network construction function, will be modified after all functional blocks are tested.
// Created by yaochen on 19/12/16.
//TODO: change all functions to store data in array, array dimensions are defined by network model
//

#ifndef _GET_BATCH_NORM_H_
#define _GET_BATCH_NORM_H_

#include <iostream>
//#include <vector>
#include <fstream>
//#include <algorithm>
//#include "data_type.h"
//#include "config.h"
#include <string>

//template<int size>
void get_batch_norm_mean(
	    const char* src,
		float mean[]) {

    cout << "Loading batch_norm layer mean ..." << endl;
    ifstream ifs(src);
    string str;
    if (!ifs) {
        cout << "mean file not found !" << endl;
    }
    int serial_no = 0;
    while (ifs >> str) {
		float f = atof(str.c_str());
		mean[serial_no] = f;
		serial_no++;
    }
    ifs.close();
}

void get_batch_norm_denominator(
	    const char* src,
		float denominator[]) {

    cout << "Loading batch_norm layer denominator ..." << endl;
    ifstream ifs(src);
    string str;
    if (!ifs) {
        cout << "denominator file not found !" << endl;
    }
    int serial_no = 0;
    while (ifs >> str) {
		float f = atof(str.c_str());
		denominator[serial_no] = f;
		serial_no++;
    }
    ifs.close();
}

void get_batch_norm_gamma(
	    const char* src,
        float gamma[]) {

    cout << "Loading batch_norm layer gamma ..." << endl;
    ifstream ifs(src);
    string str;
    if (!ifs) {
        cout << "gamma file not found !" << endl;
    }
    int serial_no = 0;
    while (ifs >> str) {
		float f = atof(str.c_str());
		gamma[serial_no] = f;
		serial_no++;
    }
    ifs.close();
}

void get_batch_norm_beta(
	    const char* src,
		float beta[]) {

    cout << "Loading batch_norm layer beta ..." << endl;
    ifstream ifs(src);
    string str;
    if (!ifs) {
        cout << "beta file not found !" << endl;
    }
    int serial_no = 0;
    while (ifs >> str) {
		float f = atof(str.c_str());
		beta[serial_no] = f;
		serial_no++;
    }
    ifs.close();
}

#endif 
