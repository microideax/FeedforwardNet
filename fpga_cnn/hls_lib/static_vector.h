// Initially designed by Li Wang(wangli1426@gmail.com)
// Static vector define for HLS compatiblility.

#ifndef __STATIC_VECTOR_H__
#define __STATIC_VECTOR_H__

#include <iostream>

template <class T, int L >
class s_vector {
  private:
    T elems[L];
    int cur;
    const int capacity;
  public:
    s_vector():cur(0), capacity(L){};

    s_vector(const s_vector& e, const int cap) {
      capacity = cap;
      cur = e.cur;
      for(int i = 0; i < e.cur; i++) {
        elems[i] = e[i];
      }
    }

    void push_back(const T& e) {
      elems[cur++] = e;
    }
 
    void pop_back() {
      cur--;
    }
    
    T& front() {
      return elems[0];
    }
    
    T& back() {
      return elems[cur - 1];
    }

    int size() const {
      return cur;
    }

    void clear() {
      cur = 0;
    }

    T & operator[](unsigned int index) {
//TODO: enable the HLS_COMPATIBLE optition
//#ifndef HLS_COMPATIBLE
//      assert(index < cur);
//#endif
      return elems[index];  
    }
    
    const T & operator[](unsigned int index) const {
//TODO: enable the HLS_COMPATIBLE optition
//#ifndef HLS_COMPATIBLE
//      assert(index < cur);
//#endif
      return elems[index];
    }    

    s_vector& operator= (const s_vector& o) {
      cur = o.cur;
      for(int i = 0; i < cur; i++) {
        elems[i] = o[i];
      }
      return *this;
    }
            
};

#endif
