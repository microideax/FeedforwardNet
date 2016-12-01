// ConsoleApplication3.cpp : 定义控制台应用程序的入口点。
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
//#include "hls_video.h"
//#include "hls_opencv.h"

//#include "../tiny_dnn/util/util.h"
//#define _HAS_ITERATOR_DEBUGGING 0
//#define _SECURE_SCL_ 0

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

std::vector<tensor_t> forward(const std::vector<tensor_t>& in, network<sequential> nn) {
	tensor_t in_data = in[0];//原本的输入一维表示
	tensor_t in_data2D;//转换成二维表示
	fill_tensor(in_data2D, float(0));
	vec_t vec1;//输入的行向量
	vec_t vec2;//输出的行向量
	for (int i = 0; i < in_data[0].size(); i++)
	{
		//if (i==0||i % 32!=0) {这样不行，因为每行的第一个元素丢了
		if(i==0||(i-1)/32==i/32){//将原本的一维输入按图片大小转换成二维
			vec1.push_back(in_data[0][i]);//放入输入map每个像素的输入值构成输入行向量
			if (i == in_data[0].size() - 1) {//最后一个输入行向量
				in_data2D.push_back(vec1);//放入输入行向量构成二维输入tensor
				vec1.clear();
			}
		}
		else {
			in_data2D.push_back(vec1);
			vec1.clear();
			vec1.push_back(in_data[0][i]);//放入输入行向量第一个元素
		}
	}
	tensor_t out_data2D;
	vector<tensor_t> out;
	fill_tensor(out_data2D, float(0));
	cout << "输入map数量" << in_data.size()<<endl;//1
	cout << "输入map像素数" << in_data[0].size() << endl;//1024
	/*for(int i=0;i< in_data.size(); i++) {
		const vec_t& in = in_data[i];
		vec_t& a = out_data[i];*/
		int K = 5;//kernel长宽
		int m = 32;//图片m×n
		int n = 32;
		ifstream ifs("LeNet-weights");
		string str;
		int count = 0;
		int bias_num=0;
		tensor_t weight2D;//权重&偏置矩阵：weight2D[0]存权重；weight2D[1]存偏置
		vec_t vec3;//权重向量
		vec_t vec4;//偏置向量
		while (ifs >> str&&count<156)
		{
			if (count<150) {//前150个数据是weight
				float f = atof(str.c_str());
				vec3.push_back(f);//放入权重
				cout << vec3[count] << " ";
			}
			else {//后6个是bias
				float f = atof(str.c_str());
				vec4.push_back(f);//放入偏置
				cout << vec4[bias_num] << " ";
				bias_num++;
			}
			count++;
		}
		weight2D.push_back(vec3);
		weight2D.push_back(vec4);
		cout << "权重总数为：" << count << endl;
		ifs.close();
		for (int a = 0; a < (*nn[0]->weights()[1]).size(); a++) {
			for (int i = K / 2; i < m - K / 2; ++i) //遍历输入map
			{
				for (int j = K / 2; j < n - K / 2; ++j)
				{
					float sum = 0; //
					for (int ii = -K / 2; ii <= K / 2; ++ii) //遍历kernel
					{
						for (int jj = -K / 2; jj <= K / 2; ++jj)
						{
							float data = in_data2D[i + ii][j + jj];
							//float weight = (*nn[0]->weights()[0])[a*K*K+(ii + K / 2) * 5 + (jj + K / 2)];
							float weight = weight2D[0][a*K*K+(ii + K / 2) * 5 + (jj + K / 2)];
							sum += data * weight;// 输出map每个像素的输出值
						}
					}
					//sum += (*nn[0]->weights()[1])[a];//加上偏置bias
					sum += weight2D[1][a];
					const float ep = exp(sum);
					const float em = exp(-sum);
					sum= (ep - em) / (ep + em);//tan_h激活
					vec2.push_back(sum);//放入sum构成输出行向量
				}
				out_data2D.push_back(vec2);//放入输出行向量构成输出map
				vec2.clear();
			}
			out.push_back(out_data2D);//放入输出map构成三维输出
			out_data2D.clear();
		}
		//for (int i = 0; i < out.size(); i++) {//遍历全部输出map
			for (int j = 0; j < out[0].size(); j++) {
				for (int k = 0; k < out[0][j].size(); k++) {
					cout << out[0][j][k]<<" ";
				}
				cout << endl;
			}
			cout << endl;
				//if (!params.tbl.is_connected(o, inc)) continue;
	return out;
}

std::vector<tensor_t> fprop(const std::vector<tensor_t>& in, network<sequential> nn) {
	for (int i = 0; i < in.size(); i++) {
		for (int j = 0; j < in[i].size(); j++) {
			for (int k = 0; k < in[i][j].size(); k++) {
				cout << in[i][j][k]<<" ";
			}
			cout << endl;
		}
		cout << endl;
	}
	return forward(in,nn);
}

std::vector<vec_t> fprop(const std::vector<vec_t>& in, network<sequential> nn) {
	/*for (int i = 0; i < in.size(); i++) {
		for (int j = 0; j < in[i].size(); j++) {
				cout << in[i][j];
		}
		cout << endl;
	}*/
	return fprop(std::vector<tensor_t>{ in },nn)[0];
}

vec_t fprop(const vec_t& in, network<sequential> nn) {
	//if (in.size() != (size_t)in_data_size())
	//	data_mismatch(**net_.begin(), in);
	return fprop(std::vector<vec_t>{ in },nn)[0];
}

vec_t predict(const vec_t& in, network<sequential> nn) { 
	return fprop(in,nn); 
}

void recognize(network<sequential>& nn, vec_t& data, vec_t& res) {

	// recognize
	res = predict(data,nn);
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
