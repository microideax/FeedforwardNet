//#include "stdafx.h"
//LeNet-5 image recognition with std image
//eliminating OpenCV lib dependency
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include "stb_image/stb_image_resize.h"
#include "stb_image/stb_image_write.h"
#include "../tiny_dnn/tiny_dnn.h"
#include "../tiny_dnn/hls_lib/static_vector.h"
#include "../tiny_dnn/util/image.h"

using namespace tiny_dnn;
using namespace tiny_dnn::activation;
using namespace std;

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
	//cout<<data.size()<<endl;
	//cout << input_pixels << endl;
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
	cout << data.size() << endl;
	
}

bool save_image(const std::string& imagefilename,
	const image<>& img
)
{
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
			connection_table(tbl, 6, 16))              // C3, 6@14x14-in, 16@10x10-in
		<< average_pooling_layer<tan_h>(10, 10, 16, 2)  // S4, 16@10x10-in, 16@5x5-out
		<< convolutional_layer<tan_h>(5, 5, 5, 16, 120) // C5, 16@5x5-in, 120@1x1-out
		<< fully_connected_layer<tan_h>(120, 10);       // F6, 120-in, 10-out

	//int n = 0;
	//for (int i = 0; i < nn.depth(); i++) { 
	//	cout << "#layer:" << i << "\n"; 
	//	cout << "layer type:" << nn[i]->layer_type() << "\n"; 
	//	/*for (int j = 0; j < nn[i]->weights().size();j++) {
	//		for (int k = 0;k < (*nn[i]->weights()[j]).size(); k++) {
	//			cout << "input:" << (*nn[i]->weights()[j])[k] << "\n";
	//		}
	//	}*/
	//	cout << "input:" << nn[i]->in_data_size() << "(" << nn[i]->in_data_shape() << ")\n";
	//	cout << "output:" << nn[i]->out_data_size() << "(" << nn[i]->out_data_shape() << ")\n";
	//	cout << "维数：权重&偏置" << nn[i]->weights().size() << endl;//2
	//	for (int j = 0; j < nn[i]->weights().size(); j++) {
	//		cout << "权重像素数&偏置数" << (*nn[i]->weights()[j]).size() << endl;//150：权重像素数5×5×6;6：偏置个数
	//		for (int k = 0; k < (*nn[i]->weights()[j]).size(); k++) {
	//			cout << (*nn[i]->weights()[j])[k] << " ";//输出权重map像素&偏置
	//			n++;
	//		}
	//		cout <<"权重偏置总数为："<<n<< endl;
	//	}
	//}
}

void nn_load_weight(const std::string& dic, network<sequential>& nn) {

	ifstream weights(dic);
	nn.load(weights);
}


void recognize(network<sequential>& nn, vec_t& data, vec_t& res) {

	// recognize
	// res = predict(data,nn);
	res = nn.predict(data);
	//return res;
}

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


/*
// save outputs of each layer
for (size_t i = 0; i < nn.depth(); i++) {
auto out_img = nn[i]->output_to_image();
auto filename = "layer_" + std::to_string(i) + ".png";
if (!save_image(filename, out_img)) {
cout << "failed to save " << filename << endl;
}
}
// save filter shape of first convolutional layer
{
auto weight = nn.at<convolutional_layer<tan_h>>(0).weight_to_image();
auto filename = "weights.png";
if (!save_image(filename, weight)) {
cout << "failed to save " << filename << endl;
}
}
*/

int main(int argc, char** argv) {
	cnn_size_t in_channels_;   // number of input vectors
	cnn_size_t out_channels_;  // number of output vectors
	//std::vector<vector_type> in_type_;
	//std::vector<vector_type> out_type_;
	float c = 3;

	network<sequential> nn;
	vec_t res;
	vec_t data;

	//convert image to data matrix
	const std::string filename = "4.bmp";
	convert_image(filename, -1.0, 1.0, 32, 32, data);

	//construct net and load weights to net
	construct_net(nn);
	/**/
	nn_load_weight("LeNet-weights", nn);

	//image<unsigned char> img = nn[0]->output_to_image(); // visualize activations of recent input 
	//img.write("layer0.bmp");
	//image<unsigned char> img1 = nn.at< convolutional_layer<tan_h>>(0).weight_to_image();
	//img1.write("kernel0.bmp");

	//cout <<"源代码执行输出结果:"<< endl;
	//vector<tensor_t> source_out=nn[0]->output();//源代码执行输出结果
	//for (int i = 0; i < source_out.size(); i++) {
	//	for (int j = 0; j < source_out[i].size(); j++) {
	//		for (int k = 0; k < source_out[i][j].size(); k++) {
	//			cout << source_out[i][j][k] << " ";
	//		}
	//		cout << endl;
	//	}
	//	cout << endl;
	//}

	//prediction stage    
	recognize(nn, data, res);

	//print out ranked scores 
	print_score(res);
	/**/
	return 0;
}
