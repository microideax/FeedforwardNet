// Baseline data type define for the entire design
// TODO:
#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_

#include<vector>
#include<string>
#include "hls_lib/static_vector.h"
#include "hls_lib/static_unordered_map.h"
#include "hls_lib/static_unordered_set.h"

typedef unsigned int uint;
typedef uint cnn_size_t;
typedef std::vector<float, std::allocator<float> > vec_t;
using namespace std;
typedef std::vector<vec_t> tensor_t;
typedef s_vector<float, 16> hls_vec;
typedef s_vector<hls_vec, 16> hls_tensor;

#endif