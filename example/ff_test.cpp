//This is the main function of a LeNet-5 model based application.
//Application description: LeNet-5 image recognition with std image.
//Using stb_image instead of OpenCV to eliminate library dependency.

//#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb_image/stb_image.h"
#include "../stb_image/stb_image_resize.h"
#include "../stb_image/stb_image_write.h"
#include "../fpga_cnn/hls_lib/static_vector.h"
#include "../fpga_cnn/convolution.h"
#include "../fpga_cnn/average_pooling.h"
#include "../fpga_cnn/fully_connect.h"
#include "../fpga_cnn/image_converter.h"
#include <iterator>
using namespace std;

int nn_in_data_size_conv[3] = { 32,14,5 };
int nn_out_data_size_conv[3] = { 28,10,1 };
int nn_in_number_conv[4] = { 1,6,16,120 };
int nn_channel_size_conv = 5;
int nn_channel_number_conv[4] = { 1, 6,16,120 };
bool has_connection_table[3] = { false,true,false };
int in_number_conv = 0;//第几个卷积输入层
int weight_bias_count_1 = 0;//前n层的权重偏置数
int weight_bias_count_2 = 0;//前n-1层的权重偏置数
int weight_count = 0;
int convolution_layer_count = 0;

int nn_in_data_size_pooling[2] = { 28,10 };
int nn_out_data_size_pooling[2] = { 14,5 };
int nn_in_number_pooling[2] = { 6,16 };
int nn_channel_number_pooling[2] = { 6,16 };
int nn_channel_size_pooling = 2;
int in_number_pooling = 0;//第几个池化输入层

int nn_in_data_size_fc[1] = { 1 };
int nn_out_data_size_fc[1] = { 1 };
int nn_in_number_fc[2] = { 120,10 };
int nn_channel_size_fc = 1;
int nn_channel_number_fc[2] = { 120,10 };
int in_number_fc = 0;//第几个全连接输入层

typedef s_vector<float, 16> hls_vec;
typedef s_vector<hls_vec, 16> hls_tensor;

// rescale output to 0-100
template <typename Activation>
double rescale(double x) {
	Activation a;
	return 100.0 * (x - a.scale().first) / (a.scale().second - a.scale().first);
}

void convert_image(const std::string& imagefilename,
	double minv,
	double maxv,
	int w,
	int h,
	vec_t& data) {
	// load
	int input_w, input_h, comp;
	stbi_uc* input_pixels = stbi_load(imagefilename.c_str(), &input_w, &input_h, &comp, 1);

	if (!input_pixels) {
		// cannot open, or it's not an image
		cout << "stbi_load failed";
		return;
	}

	// resize
	std::vector<uint8_t> resized(w * h);
	uint8_t* resized_pixels = &(resized[0]);
	int input_stride_in_bytes = input_w;
	if (!stbir_resize_uint8(input_pixels, input_w, input_h, input_stride_in_bytes, resized_pixels, w, h, w, 1)) {
		cout << "stbir_resize_uint8 failed";
		stbi_image_free(input_pixels);
		return;
	}
	stbi_image_free(input_pixels);
	// mnist dataset is "white on black", so negate required

	std::transform(resized.begin(), resized.end(), std::back_inserter(data),
		[=](uint8_t c) { return (255 - c) * (maxv - minv) / 255.0 + minv; });

	/*
	for (unsigned int i = 0; i < resized.size(); i++){
	data.push_back((255 - resized[i]) * (maxv - minv) / 255.0 + minv);
	}
	*/
	cout << data.size() << endl;

}

/*
bool save_image(const std::string& imagefilename, const image<>& img) {
// no scaling, save at original size
int stride_bytes = img.width();
int ret = stbi_write_png(
imagefilename.c_str(),
img.width(),
img.height(),
1,
&(img.at(0, 0)),
stride_bytes);
return (ret != 0);
}
*/
/*
//void construct_net(network<sequential>& nn) {
void construct_net(network<sequential>& nn) {
// connection table [Y.Lecun, 1998 Table.1]
#define O true
#define X false
static const bool tbl[] = {
O, X, X, X, O, O, O, X, X, O, O, O, O, X, O, O,
O, O, X, X, X, O, O, O, X, X, O, O, O, O, X, O,
O, O, O, X, X, X, O, O, O, X, X, O, X, O, O, O,
X, O, O, O, X, X, O, O, O, O, X, X, O, X, O, O,
X, X, O, O, O, X, X, O, O, O, O, X, O, O, X, O,
X, X, X, O, O, O, X, X, O, O, O, O, X, O, O, O
};
#undef O
#undef X

// construct nets
nn << convolutional_layer<tan_h>(32, 32, 5, 1, 6)  // C1, 1@32x32-in, 6@28x28-out
<< average_pooling_layer<tan_h>(28, 28, 6, 2)   // S2, 6@28x28-in, 6@14x14-out
<< convolutional_layer<tan_h>(14, 14, 5, 6, 16,
connection_table(tbl, 6, 16))              // C3, 6@14x14-in, 16@10x10-out
<< average_pooling_layer<tan_h>(10, 10, 16, 2)  // S4, 16@10x10-in, 16@5x5-out
<< convolutional_layer<tan_h>(5, 5, 5, 16, 120) // C5, 16@5x5-in, 120@1x1-out
<< fully_connected_layer<tan_h>(120, 10);       // F6, 120-in, 10-out
}

*/

std::vector<tensor_t> load_weight_conv() {
	ifstream ifs("LeNet-weights");
	string str;
	tensor_t weight_v;//权重矩阵
	std::vector<tensor_t> weight2D;
	vec_t vec3;//权重向量
			   //前n-1层权重&偏置+第n层权重数
	int weight_count = weight_bias_count_1 + nn_channel_size_conv*nn_channel_size_conv*nn_in_number_conv[in_number_conv] * nn_in_number_conv[in_number_conv + 1];
	int weight_bias_count_3_conv = 0;
	while (ifs >> str&&weight_bias_count_3_conv<weight_count)
	{
		if (weight_bias_count_3_conv >= weight_bias_count_2) {
			//连续n个数据是weight
			if (weight_bias_count_1 == 0 || (weight_bias_count_1 - weight_bias_count_2 - 1) / nn_channel_size_conv == (weight_bias_count_1 - weight_bias_count_2) / nn_channel_size_conv) {//将原本的一维权重按kernel大小转换成二维
				float f = atof(str.c_str());
				vec3.push_back(f);//放入kernel每个像素的权重值构成输入行向量
				if (weight_bias_count_1 == weight_count - 1) {//最后一个输入行向量
					weight_v.push_back(vec3);//放入输入行向量构成二维输入tensor
					vec3.clear();
				}
				if (weight_v.size() == nn_channel_size_conv) {
					weight2D.push_back(weight_v);
					weight_v.clear();
				}
			}
			else {
				weight_v.push_back(vec3);
				vec3.clear();
				float f = atof(str.c_str());
				vec3.push_back(f);//放入输入行向量第一个元素
			}
			weight_bias_count_1++;
		}
		weight_bias_count_3_conv++;
	}
	cout << "conv weights number in total is = " << weight_bias_count_1 << endl;
	ifs.close();
	return weight2D;
}

vec_t load_weight_pooling() {
	ifstream ifs("LeNet-weights");
	string str;
	vec_t weight_v;//权重向量
				   //前n-1层权重&偏置+第n层权重数
	int weight_count = weight_bias_count_1 + nn_channel_size_pooling*nn_channel_size_pooling*nn_in_number_pooling[in_number_pooling];
	int weight_bias_count_3_pooling = 0;
	while (ifs >> str&&weight_bias_count_3_pooling<weight_count)
	{
		if (weight_bias_count_3_pooling >= weight_bias_count_2) {
			//连续n个数据是weight
			float f = atof(str.c_str());
			weight_v.push_back(f);//放入kernel每个像素的权重值构成输入行向量
			weight_bias_count_1++;
		}
		weight_bias_count_3_pooling++;
	}
	cout << "weights number in total is = " << weight_bias_count_1 << endl;
	ifs.close();
	return weight_v;
}

std::vector<tensor_t> load_weight_fc() {
	ifstream ifs("LeNet-weights");
	string str;
	tensor_t weight_v;//权重矩阵
	std::vector<tensor_t> weight2D;
	vec_t vec3;//权重向量
			   //前n-1层权重&偏置+第n层权重数
	int weight_count = weight_bias_count_1 + nn_channel_size_fc*nn_channel_size_fc*nn_in_number_fc[in_number_fc] * nn_in_number_fc[in_number_fc + 1];
	int weight_bias_count_3_fc = 0;
	while (ifs >> str&&weight_bias_count_3_fc<weight_count)
	{
		if (weight_bias_count_3_fc >= weight_bias_count_2) {
			//连续n个数据是weight
			if (nn_channel_size_fc == 1) {//如果全连接kernel大小为1
				float f = atof(str.c_str());
				vec3.push_back(f);//放入kernel每个像素的权重值构成输入行向量
				weight_v.push_back(vec3);//放入输入行向量构成二维输入tensor
				weight2D.push_back(weight_v);
				vec3.clear();
				weight_v.clear();
			}
			else {
				if (weight_bias_count_1 == 0 || (weight_bias_count_1 - weight_bias_count_2 - 1) / nn_channel_size_fc == (weight_bias_count_1 - weight_bias_count_2) / nn_channel_size_fc) {//将原本的一维权重按kernel大小转换成二维
					float f = atof(str.c_str());
					vec3.push_back(f);//放入kernel每个像素的权重值构成输入行向量
					if (weight_bias_count_1 == weight_count - 1) {//最后一个输入行向量
						weight_v.push_back(vec3);//放入输入行向量构成二维输入tensor
						vec3.clear();
					}
					if (weight_v.size() == nn_channel_size_fc) {
						weight2D.push_back(weight_v);
						weight_v.clear();
					}
				}
				else {
					weight_v.push_back(vec3);
					vec3.clear();
					float f = atof(str.c_str());
					vec3.push_back(f);//放入输入行向量第一个元素
				}
			}
			weight_bias_count_1++;
		}
		weight_bias_count_3_fc++;
	}
	cout << "conv weights number in total is = " << weight_bias_count_1 << endl;
	ifs.close();
	return weight2D;
}

vec_t load_bias_conv() {
	ifstream ifs("LeNet-weights");
	string str;
	vec_t bias2D;
	int weight_count = weight_bias_count_2 + nn_channel_size_conv*nn_channel_size_conv*nn_in_number_conv[in_number_conv] * nn_in_number_conv[in_number_conv + 1];
	int weight_bias_count = weight_bias_count_2 + nn_channel_size_conv*nn_channel_size_conv*nn_in_number_conv[in_number_conv] * nn_in_number_conv[in_number_conv + 1] + nn_channel_number_conv[in_number_conv + 1];
	int weight_bias_count_3_conv = 0;
	while (ifs >> str&&weight_bias_count_3_conv<weight_bias_count)
	{
		if (weight_bias_count_3_conv >= weight_bias_count_1&&weight_bias_count_1 >= weight_count&&weight_bias_count_1 <= weight_bias_count) {
			//连续n个数据是bias
			float f = atof(str.c_str());
			bias2D.push_back(f);//放入偏置
			weight_bias_count_1++;
		}
		weight_bias_count_3_conv++;
	}
	weight_bias_count_2 = weight_bias_count_1;
	cout << "bias weights number in total is = " << weight_bias_count_1 << endl;
	ifs.close();
	return bias2D;
}

vec_t load_bias_pooling() {
	ifstream ifs("LeNet-weights");
	string str;
	vec_t bias2D;
	int weight_count = weight_bias_count_2 + nn_channel_size_pooling*nn_channel_size_pooling*nn_in_number_pooling[in_number_pooling];
	int weight_bias_count = weight_bias_count_2 + nn_channel_size_pooling*nn_channel_size_pooling*nn_in_number_pooling[in_number_pooling] + nn_channel_number_pooling[in_number_pooling];
	int weight_bias_count_3_pooling = 0;
	while (ifs >> str&&weight_bias_count_3_pooling<weight_bias_count)
	{
		if (weight_bias_count_3_pooling >= weight_bias_count_1&&weight_bias_count_1 >= weight_count&&weight_bias_count_1 <= weight_bias_count) {
			//连续n个数据是bias
			float f = atof(str.c_str());
			bias2D.push_back(f);//放入偏置
			weight_bias_count_1++;
		}
		weight_bias_count_3_pooling++;
	}
	weight_bias_count_2 = weight_bias_count_1;
	cout << "weights number in total is = " << weight_bias_count_1 << endl;
	ifs.close();
	return bias2D;
}

vec_t load_bias_fc() {
	ifstream ifs("LeNet-weights");
	string str;
	vec_t bias2D;
	int weight_count = weight_bias_count_2 + nn_channel_size_fc*nn_channel_size_fc*nn_in_number_fc[in_number_fc] * nn_in_number_fc[in_number_fc + 1];
	int weight_bias_count = weight_bias_count_2 + nn_channel_size_fc*nn_channel_size_fc*nn_in_number_fc[in_number_fc] * nn_in_number_fc[in_number_fc + 1] + nn_channel_number_fc[in_number_fc + 1];
	int weight_bias_count_3_fc = 0;
	while (ifs >> str&&weight_bias_count_3_fc<weight_bias_count)
	{
		if (weight_bias_count_3_fc >= weight_bias_count_1&&weight_bias_count_1 >= weight_count&&weight_bias_count_1 <= weight_bias_count) {
			//连续n个数据是bias
			float f = atof(str.c_str());
			bias2D.push_back(f);//放入偏置
			weight_bias_count_1++;
		}
		weight_bias_count_3_fc++;
	}
	weight_bias_count_2 = weight_bias_count_1;
	cout << "bias weights number in total is = " << weight_bias_count_1 << endl;
	ifs.close();
	return bias2D;
}

/*
//输出结果：数字，概率
void print_score(vec_t res)
{
vector<pair<double, int> > scores;
// sort & print top-3
for (int i = 0; i < 10; i++)
scores.emplace_back(rescale<tan_h>(res[i]), i);
//按概率从大到小排列
sort(scores.begin(), scores.end(), greater<pair<double, int>>());
for (int i = 0; i < 10; i++)
cout << scores[i].second << "," << scores[i].first << endl;
getchar();
}
*/

int main(int argc, char** argv) {

	float c = 3;
	//network<sequential> nn;
	vec_t res;
	vec_t data_in;  //image data storage
	vec_t data_out;
	vec_t data;
	tensor_t in_data;
	tensor_t out_data;

	//convert image to data matrix
	const std::string filename = "4.bmp";
	convert_image(filename, -1.0, 1.0, 32, 32, data_in);

	//construct net and load weights to net
	//construct_net(nn);	
	//nn_load_weight("LeNet-weights", nn);

	std::vector<tensor_t> conv_1_weight2D;//卷积层权重矩阵
	vec_t 		      conv_1_bias2D;//偏置矩阵
	conv_1_weight2D = load_weight_conv();
	conv_1_bias2D = load_bias_conv();
	in_data = in_2_3D(data_in);//vector的输入图片转成tensor

	std::vector<tensor_t> in_data2D;
	in_data2D = in_2_2D_conv(nn_in_data_size_conv[0], in_data);//in转换成二维表示
	std::vector<tensor_t> conv_1_out_data;

	cout << "Finished network weights and data space preparation" << endl;
	cout << "starting convolution layer 1" << endl;

	//convolution_1
	convolution_layer_with_table(
		"tan_h",
		nn_in_data_size_conv[0],
		nn_channel_size_conv,
		in_data2D,
		has_connection_table[0],
		conv_1_weight2D,
		conv_1_bias2D,
		conv_1_out_data,
		nn_channel_number_conv[0],
		nn_channel_number_conv[1]);

	in_number_conv++;

	cout << "Finished convolution layer 1" << endl;
	cout << "Starting pooling layer 1" << endl;

	vec_t pooling_1_weight;//权重矩阵
	vec_t pooling_1_bias2D;//偏置矩阵
	pooling_1_weight = load_weight_pooling();
	pooling_1_bias2D = load_bias_pooling();
	std::vector<tensor_t> pooling_1_out_data;

	//pooling_1
	pooling_layer(
		"tan_h",
		nn_in_data_size_pooling[0],
		nn_channel_size_pooling,
		conv_1_out_data,
		pooling_1_weight,
		pooling_1_bias2D,
		pooling_1_out_data,
		nn_in_number_pooling[0],
		nn_channel_number_pooling[0]);

	in_number_pooling++;

	cout << "Finished pooling layer 1" << endl;
	cout << "Starting convolution layer 2" << endl;

	std::vector<tensor_t> conv_2_weight2D;//卷积层权重矩阵
	vec_t                 conv_2_bias2D;//偏置矩阵
	conv_2_weight2D = load_weight_conv();
	conv_2_bias2D = load_bias_conv();
	std::vector<tensor_t> conv_2_out_data;

	//convolution_2
	convolution_layer_with_table(
		"tan_h",
		nn_in_data_size_conv[1],
		nn_channel_size_conv,
		pooling_1_out_data,
		has_connection_table[1],
		conv_2_weight2D,
		conv_2_bias2D,
		conv_2_out_data,
		nn_channel_number_conv[1],
		nn_channel_number_conv[2]);

	in_number_conv++;

	cout << "Finished convolution layer 2" << endl;
	cout << "Starting pooling layer 2" << endl;

	vec_t pooling_2_weight;//权重矩阵
	vec_t pooling_2_bias2D;//偏置矩阵
	pooling_2_weight = load_weight_pooling();
	pooling_2_bias2D = load_bias_pooling();
	std::vector<tensor_t> pooling_2_out_data;

	//pooling_2
	pooling_layer(
		"tan_h",
		nn_in_data_size_pooling[1],
		nn_channel_size_pooling,
		conv_2_out_data,
		pooling_2_weight,
		pooling_2_bias2D,
		pooling_2_out_data,
		nn_in_number_pooling[1],
		nn_channel_number_pooling[1]);

	in_number_pooling++;

	cout << "Finished pooling layer 2" << endl;
	cout << "Starting convolution layer 3" << endl;

	std::vector<tensor_t> conv_3_weight2D;//卷积层权重矩阵
	vec_t                 conv_3_bias2D;//偏置矩阵
	conv_3_weight2D = load_weight_conv();
	conv_3_bias2D = load_bias_conv();
	std::vector<tensor_t> conv_3_out_data;

	//convolution_3
	convolution_layer_with_table(
		"tan_h",
		nn_in_data_size_conv[2],
		nn_channel_size_conv,
		pooling_2_out_data,
		has_connection_table[2],
		conv_3_weight2D,
		conv_3_bias2D,
		conv_3_out_data,
		nn_channel_number_conv[2],
		nn_channel_number_conv[3]);

	in_number_conv++;

	cout << "Finished convolution layer 3" << endl;
	cout << "Starting fully connected layer 1" << endl;

	std::vector<tensor_t> fc_1_weight2D;//全连接层权重矩阵
	vec_t                 fc_1_bias2D;//偏置矩阵
	fc_1_weight2D = load_weight_fc();
	fc_1_bias2D = load_bias_fc();
	std::vector<tensor_t> fc_1_out_data;

	//fully_connect
	fully_connected_layer(
		"tan_h",
		nn_in_data_size_fc[0],
		conv_3_out_data,
		fc_1_weight2D,
		fc_1_bias2D,
		fc_1_out_data,
		nn_channel_number_fc[0],
		nn_channel_number_fc[1]);

	in_number_fc++;

	cout << "End of network" << endl;

	return 0;
}
