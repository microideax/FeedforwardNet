// Initially designed by Li Wang(wangli1426@gmail.com)

#ifndef __STATIC_UNORDERED_SET_H__
#define __STATIC_UNORDERED_SET_H__

#include <iostream>
#include "static_vector.h"

template <class T, int L >

class static_unordered_set {

private:
	s_vector<T, L> vec;
	
public:
	void insert(T e) {
	  int i = 0;
          for ( ; i < vec.size(); i++) {
            if(e == vec[i])
		return;
          }
          vec.push_back(e);
	}

    bool contains(T e) {
	   for (int i = 0; i < vec.size(); i++) {
             if(e == vec[i])
               return true ;
           }
           return false;
       }

       

};

#endif
