// caffe_converter.cpp : ¶šÒå¿ØÖÆÌšÓŠÓÃ³ÌÐòµÄÈë¿Úµã¡£
//

#include <iostream>
#include <fstream>
#include <memory>
#include <ctime>
#include "tiny_dnn/io/caffe/caffe.pb.h"
//#ifdef _MSC_VER
//#define _NOMINMAX
//#include <io.h>
//#include <fcntl.h>
//#define CNN_OPEN_BINARY(filename) open(filename, _O_RDONLY|_O_BINARY)
//#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define CNN_OPEN_BINARY(filename) open(filename, O_RDONLY)
//#endif
using namespace std;
using namespace caffe;
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/coded_stream.h>

void load(const caffe::LayerParameter& src,int num_input,int num_output,int kernel_size) {
    int src_idx = 0;
    ofstream out;
    out.open("weights.txt",ios::app);
    out<<"weights:"<<endl;
    //load weight
    for (int o = 0; o < num_output; o++) {
        for (int i = 0; i < num_input; i++) {
            cout<<"num_input:"<<num_input<<endl;
            cout<<"weights:"<<endl;
            for (int x = 0; x < kernel_size * kernel_size; x++) {
                out<<src.blobs(0).data(src_idx++)<<" ";
            }
            cout<<endl;
        }
    }
    out<<""<<endl;
    cout<<"bias:"<<endl;
    out<<"bias:"<<endl;
    //load bias
    for (int o = 0; o < num_output; o++) {
     out<<src.blobs(1).data(o)<<" ";
    }
    out<<""<<endl;
    out.close();
    cout<<endl;
}
void reload_weight_from_caffe_net(const caffe::NetParameter& layer)
{
    int num_layers = layer.layer_size();
        int num_input=1;//original num_input
        int num_output=0;
        int kernel_size=0;
    for (int i = 0; i < num_layers; ++i)
    {
    cout <<num_layers<<endl;
    // name，type，kernel size，pad，stride
    cout << "Layer " << i << ":" << layer.layer(i).name() << "\t" << layer.layer(i).type()<<endl;
        if(layer.layer(i).type()=="Convolution"){//get conv_layers' kernel_size,num_output
            ConvolutionParameter conv_param = layer.layer(i).convolution_param();
            num_output=conv_param.num_output();
            kernel_size=conv_param.kernel_size(0);
            cout << "kernel size: " << kernel_size<<endl;
            cout << "num_input_1: " << num_input<<endl;
            cout << "num_output: " << num_output<<endl;
            
        }
        else if(layer.layer(i).type()=="InnerProduct"){//get fc_layers' kernel_size,num_output
            InnerProductParameter inner_product_param = layer.layer(i).inner_product_param();
            kernel_size=1;
            cout << "kernel size: " << kernel_size<<endl;
            num_output=inner_product_param.num_output();
            cout << "num_input_2: " << num_input<<endl;
            cout << "num_output: " << num_output<<endl;
        }
        if(layer.layer(i).type()=="Convolution"||layer.layer(i).type()=="InnerProduct"){
            load(layer.layer(i),num_input,num_output,kernel_size);
        }
        if(layer.layer(i).type()=="Convolution"||layer.layer(i).type()=="InnerProduct"||layer.layer(i).type()=="Pooling"){
            num_input=num_output;//set each layer's num_input equals to the last layer's num_output
        }
	}
        cout<<endl;
}
void read_proto_from_binary(const std::string& protobinary,
                                   google::protobuf::Message *message) {
    int fd = CNN_OPEN_BINARY(protobinary.c_str());
    google::protobuf::io::FileInputStream rawstr(fd);
    google::protobuf::io::CodedInputStream codedstr(&rawstr);

    rawstr.SetCloseOnDelete(true);
    //codedstr.SetTotalBytesLimit(std::numeric_limits<int>::max(),
    //                            std::numeric_limits<int>::max() / 2);

    if (!message->ParseFromCodedStream(&codedstr)) {
 //       //throw nn_error("failed to parse");
    }
}
void reload_weight_from_caffe_protobinary(const std::string& caffebinary)
{
	caffe::NetParameter np;

	read_proto_from_binary(caffebinary, &np);
	reload_weight_from_caffe_net(np);
}

void test(const std::string& trained_file) {
	reload_weight_from_caffe_protobinary(trained_file);
}

int main(int argc, char** argv) {
	string trained_file = argv[2];
    cout<<"trained_file:"<<trained_file<<endl;
	test( trained_file);
}