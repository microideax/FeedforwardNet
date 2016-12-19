//Move all the image pre-processing functions here to simplify the main function.
//re-construct the array data of images into 3d
tensor_t in_2_3D(vec_t& data_in) {
	tensor_t data_out;
	data_out.push_back(data_in);
	return data_out;
}

//re-construct input array to 2d matrix
std::vector<tensor_t> in_2_2D_conv(int& input_size, tensor_t in) {
	vec_t in_data; //original input data
	vec_t vec1;    //input row vector
	tensor_t vec2;
	std::vector<tensor_t> in_data2D;
	for (int j = 0; j < in.size(); j++) {
		in_data = in[j];
		for (int i = 0; i < in_data.size(); i++)//输入一维转二维
		{
			if (in_data.size() < input_size*input_size) {//有多个输入
				if (i == 0 || (i - 1) / input_size == i / input_size) {//将原本的一维输入按图片大小转换成二维
					vec1.push_back(in_data[i]);//放入输入map每个像素的输入值构成输入行向量
					if (i == in_data.size() - 1) {//最后一个输入行向量
						vec2.push_back(vec1);//放入输入行向量构成二维输入tensor
						vec1.clear();
					}
					if (i != 0 && i % (input_size*input_size) == 0) {
						in_data2D.push_back(vec2);
						vec2.clear();
					}
				}
				else {
					vec2.push_back(vec1);
					vec1.clear();
					vec1.push_back(in_data[i]);//放入输入行向量第一个元素
					if (i != 0 && i % (input_size*input_size) == 0) {
						in_data2D.push_back(vec2);
						vec2.clear();
					}
				}
			}
			else {//只有一个输入
				if (i == 0 || (i - 1) / input_size == i / input_size) {//将原本的一维输入按图片大小转换成二维
					vec1.push_back(in_data[i]);//放入输入map每个像素的输入值构成输入行向量
					if (i == in_data.size() - 1) {//最后一个输入行向量
						vec2.push_back(vec1);//放入输入行向量构成二维输入tensor
						vec1.clear();
						in_data2D.push_back(vec2);
						vec2.clear();
					}
				}
				else {
					vec2.push_back(vec1);
					vec1.clear();
					vec1.push_back(in_data[i]);//放入输入行向量第一个元素
				}
			}
		}
	}
	return in_data2D;
}