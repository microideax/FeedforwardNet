// Baseline data type define for the entire design
// TODO:

#ifndef _ARRAY_RESET_H_
#define _ARRAY_RESET_H_

#include <iostream>

template <typename T,int size>
void array_reset(T (&x)[size]) {
	for(int i = 0; i < size; i++){
            x[i] = (T)(0);
}
}

template <typename T>
void array_reset(T (&x), int size){
    for(int i = 0; i < size; i++){
        x[i] = (T)(0);
    }
}

#endif
