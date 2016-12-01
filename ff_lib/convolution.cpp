#include "convolution.h"
#include "../tiny_dnn/activation.h"


std::vector<tensor_t> forward(const std::vector<tensor_t>& in, network<sequential> nn) {
	tensor_t in_data = in[0];//Ô­±¾µÄÊäÈëÒ»Î¬±íÊ¾
	tensor_t in_data2D;//×ª»»³É¶þÎ¬±íÊ¾
	fill_tensor(in_data2D, float(0));
	vec_t vec1;//ÊäÈëµÄÐÐÏòÁ¿
	vec_t vec2;//Êä³öµÄÐÐÏòÁ¿
	for (int i = 0; i < in_data[0].size(); i++)
	{
		//if (i==0||i % 32!=0) {ÕâÑù²»ÐÐ£¬ÒòÎªÃ¿ÐÐµÄµÚÒ»¸öÔªËØ¶ªÁË
		if(i==0||(i-1)/32==i/32){//½«Ô­±¾µÄÒ»Î¬ÊäÈë°´Í¼Æ¬´óÐ¡×ª»»³É¶þÎ¬
			vec1.push_back(in_data[0][i]);//·ÅÈëÊäÈëmapÃ¿¸öÏñËØµÄÊäÈëÖµ¹¹³ÉÊäÈëÐÐÏòÁ¿
			if (i == in_data[0].size() - 1) {//×îºóÒ»¸öÊäÈëÐÐÏòÁ¿
				in_data2D.push_back(vec1);//·ÅÈëÊäÈëÐÐÏòÁ¿¹¹³É¶þÎ¬ÊäÈëtensor
				vec1.clear();
			}
		}
		else {
			in_data2D.push_back(vec1);
			vec1.clear();
			vec1.push_back(in_data[0][i]);//·ÅÈëÊäÈëÐÐÏòÁ¿µÚÒ»¸öÔªËØ
		}
	}
	tensor_t out_data2D;
	vector<tensor_t> out;
	fill_tensor(out_data2D, float(0));
	cout << "ÊäÈëmapÊýÁ¿" << in_data.size()<<endl;//1
	cout << "ÊäÈëmapÏñËØÊý" << in_data[0].size() << endl;//1024
	/*for(int i=0;i< in_data.size(); i++) {
		const vec_t& in = in_data[i];
		vec_t& a = out_data[i];*/
		int K = 5;//kernel³¤¿í
		int m = 32;//Í¼Æ¬m¡Án
		int n = 32;
		ifstream ifs("LeNet-weights");
		string str;
		int count = 0;
		int bias_num=0;
		tensor_t weight2D;//È¨ÖØ&Æ«ÖÃ¾ØÕó£ºweight2D[0]´æÈ¨ÖØ£»weight2D[1]´æÆ«ÖÃ
		vec_t vec3;//È¨ÖØÏòÁ¿
		vec_t vec4;//Æ«ÖÃÏòÁ¿
		while (ifs >> str&&count<156)
		{
			if (count<150) {//Ç°150¸öÊý¾ÝÊÇweight
				float f = atof(str.c_str());
				vec3.push_back(f);//·ÅÈëÈ¨ÖØ
				cout << vec3[count] << " ";
			}
			else {//ºó6¸öÊÇbias
				float f = atof(str.c_str());
				vec4.push_back(f);//·ÅÈëÆ«ÖÃ
				cout << vec4[bias_num] << " ";
				bias_num++;
			}
			count++;
		}
		weight2D.push_back(vec3);
		weight2D.push_back(vec4);
		cout << "È¨ÖØ×ÜÊýÎª£º" << count << endl;
		ifs.close();
		for (int a = 0; a < (*nn[0]->weights()[1]).size(); a++) {
			for (int i = K / 2; i < m - K / 2; ++i) //±éÀúÊäÈëmap
			{
				for (int j = K / 2; j < n - K / 2; ++j)
				{
					float sum = 0; //
					for (int ii = -K / 2; ii <= K / 2; ++ii) //±éÀúkernel
					{
						for (int jj = -K / 2; jj <= K / 2; ++jj)
						{
							float data = in_data2D[i + ii][j + jj];
							//float weight = (*nn[0]->weights()[0])[a*K*K+(ii + K / 2) * 5 + (jj + K / 2)];
							float weight = weight2D[0][a*K*K+(ii + K / 2) * 5 + (jj + K / 2)];
							sum += data * weight;// Êä³ömapÃ¿¸öÏñËØµÄÊä³öÖµ
						}
					}
					//sum += (*nn[0]->weights()[1])[a];//¼ÓÉÏÆ«ÖÃbias
					sum += weight2D[1][a];
					const float ep = exp(sum);
					const float em = exp(-sum);
					sum= (ep - em) / (ep + em);//tan_h¼¤»î
					vec2.push_back(sum);//·ÅÈësum¹¹³ÉÊä³öÐÐÏòÁ¿
				}
				out_data2D.push_back(vec2);//·ÅÈëÊä³öÐÐÏòÁ¿¹¹³ÉÊä³ömap
				vec2.clear();
			}
			out.push_back(out_data2D);//·ÅÈëÊä³ömap¹¹³ÉÈýÎ¬Êä³ö
			out_data2D.clear();
		}
		//for (int i = 0; i < out.size(); i++) {//±éÀúÈ«²¿Êä³ömap
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