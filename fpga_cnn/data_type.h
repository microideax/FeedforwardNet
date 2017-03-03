// Baseline data type define for the entire design
// TODO:

#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_

#include<vector>
#include<string>

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

//typedef s_vector<float, 120> vec_t;//avg_pooling_2要存2*2*16个weight,convolution_3要存120个bias
//typedef s_vector<vec_t, 32> tensor_t;
//typedef s_vector<tensor_t, 1920> tensor_t_3d;//convolution_3要存16*120个权重矩阵


//typedef s_vector<float, 32> vec_t_32;
//typedef s_vector<vec_t_32, 32> tensor_t_32;
//typedef s_vector<tensor_t_32, 1> tensor_3d_32_1;

/*
typedef s_vector<float, 28> vec_t_28;
typedef s_vector<vec_t_28, 28> tensor_t_28;
typedef s_vector<tensor_t_28, 6> tensor_3d_28_6;

typedef s_vector<float, 14> vec_t_14;
typedef s_vector<vec_t_14, 14> tensor_t_14;
typedef s_vector<tensor_t_14, 6> tensor_3d_14_6;

typedef s_vector<float, 10> vec_t_10;
typedef s_vector<vec_t_10, 10> tensor_t_10;
typedef s_vector<tensor_t_10, 16> tensor_3d_10_16;

typedef s_vector<float, 5> vec_t_5;
typedef s_vector<vec_t_5, 5> tensor_t_5;
typedef s_vector<tensor_t_5, 16>  tensor_3d_5_16;

typedef s_vector<float, 1> vec_t_1;
typedef s_vector<vec_t_1, 1> tensor_t_1;
typedef s_vector<tensor_t_1, 120> tensor_3d_1_120;
*/

#endif
