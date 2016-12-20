// Baseline data type define for the entire design
// TODO:

#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_

#include<vector>
#include<string>
#include "hls_lib/static_vector.h"
#include "hls_lib/static_unordered_map.h"
#include "hls_lib/static_unordered_set.h"

using namespace std;

typedef unsigned int uint;
typedef uint cnn_size_t;

typedef std::vector<float, std::allocator<float> > std_vec_t;
typedef std::vector<std_vec_t> std_tensor_t;
typedef std::vector<std_tensor_t> std_tensor_t_3d;

/*
typedef std::vector<float, std::allocator<float> > vec_t;
typedef std::vector<vec_t> tensor_t;
typedef std::vector<tensor_t> tensor_t_3d;
*/

typedef s_vector<float, 32> vec_t;
typedef s_vector<vec_t, 32> tensor_t;
typedef s_vector<tensor_t, 32> tensor_t_3d;


#endif