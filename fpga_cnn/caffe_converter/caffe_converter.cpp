// caffe_converter.cpp : ¶šÒå¿ØÖÆÌšÓŠÓÃ³ÌÐòµÄÈë¿Úµã¡£
//

#include <iostream>
#include <fstream>
#include <memory>
#include <ctime>
#include "caffe.pb.h"
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
#define CNN_OPEN_TXT(filename) open(filename, O_RDONLY)
//#endif
using namespace std;
using namespace caffe;
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/repeated_field.h>

void load(const caffe::LayerParameter& src,int num_input,int num_output,int kernel_size) {
    int src_idx = 0;
    ofstream out;
    out.open("weights_batch_1.txt",ios::app);
    out<<"weights:"<<endl;
    //load weight
    for (int o = 0; o < num_output; o++) {
        for (int i = 0; i < num_input; i++) {
            //cout<<"num_input:"<<num_input<<endl;
            //cout<<"weights:"<<endl;
            for (int x = 0; x < kernel_size * kernel_size; x++) {
                out<<src.blobs(0).data(src_idx++)<<" ";
            }
            //cout<<endl;
        }
    }
    out<<""<<endl;
    //cout<<"bias:"<<endl;
    out<<"bias:"<<endl;
    //load bias
    for (int o = 0; o < num_output; o++) {
     out<<src.blobs(1).data(o)<<" ";
    }
    out<<""<<endl;
    out.close();
    //cout<<endl;
}
void reload_weight_from_caffe_net(const caffe::NetParameter& layer,int& input_size)
{
    int num_layers = layer.layer_size();
    int num_input=1;
    int num_output=0;
    int pad=0;
    int kernel_size=0;
    int stride=0;
    for (int i = 0; i < num_layers; ++i)
    {
    cout <<num_layers<<endl;
    // name，type，kernel size，pad，stride
    cout << "Layer " << i << ":" << layer.layer(i).name() << "\t" << layer.layer(i).type()<<endl;
        if(layer.layer(i).type()=="Convolution"){//get conv_layers' kernel_size,num_output
            ConvolutionParameter conv_param = layer.layer(i).convolution_param();
            num_output=conv_param.num_output();
            if (conv_param.pad_size()>0){
                pad=conv_param.pad(0);
            }
            //cout<<"~~~~~~~"<<endl;
            //google::protobuf::RepeatedField<string> repeated_field;
            //cout<<"~~~~~~~"<<endl;
            //repeated_field.size();
            //cout<<"~~~~~~~"<<endl;
           //cout<<repeated_field.size()<<endl;
            //pad=conv_param.pad(0);
            kernel_size=conv_param.kernel_size(0);
            stride=conv_param.stride(0);
            input_size = (input_size + 2 * pad - kernel_size) / stride + 1;
            cout << "kernel size: " << kernel_size<<endl;
            cout << "num_input: " << num_input<<endl;
            cout << "num_output: " << num_output<<endl;
            cout << "conv_output_size: " << input_size<<endl;
        }else if(layer.layer(i).type()=="InnerProduct"){//get fc_layers' kernel_size,num_output
            InnerProductParameter inner_product_param = layer.layer(i).inner_product_param();
            kernel_size=input_size;
            cout << "kernel size: " << kernel_size<<endl;
            num_output=inner_product_param.num_output();
            cout << "num_input: " << num_input<<endl;
            cout << "num_output: " << num_output<<endl;
        }else if(layer.layer(i).type()=="Pooling"){//get fc_layers' kernel_size,num_output
            PoolingParameter pooling_param = layer.layer(i).pooling_param();
            pad=pooling_param.pad();
            kernel_size=pooling_param.kernel_size();
            stride=pooling_param.stride();
            input_size = (input_size + 2 * pad - kernel_size) / stride + 1;
            cout << "kernel size: " << kernel_size<<endl;
            cout << "pooling_output_size: " << input_size<<endl;
            //num_output=input_param.crop_size();
            //cout << "num_input_2: " << num_input<<endl;
            //cout << "shape: " << input_param.crop_size()<<endl;
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
int create_net_from_caffe_net(const caffe::NetParameter& layer)
{
        if (layer.input_shape_size() > 0) {
            // input_shape is deprecated in Caffe
            // blob dimensions are ordered by number N x channel K x height H x width W
            int depth  = static_cast<int>(layer.input_shape(0).dim(1));
            int height = static_cast<int>(layer.input_shape(0).dim(2));
            int width  = static_cast<int>(layer.input_shape(0).dim(3));
            //cout<<"depth:********************"<<depth<<endl;
        }else if (layer.layer(0).has_input_param()) {
            // blob dimensions are ordered by number N x channel K x height H x width W
            //int depth  = static_cast<int>(layer.layer(0).input_param().shape(0).dim(1));
            int kernel_size = static_cast<int>(layer.layer(0).input_param().shape(0).dim(2));
            //int kernel_h = static_cast<int>(layer.layer(0).input_param().shape(0).dim(2));
            //int kernel_w = static_cast<int>(layer.layer(0).input_param().shape(0).dim(3));
            //cout<<"width:********************"<<kernel_w<<endl;
            return kernel_size;
        }
}
void read_proto_from_text(const std::string& prototxt,
                                 google::protobuf::Message *message) {
    int fd = CNN_OPEN_TXT(prototxt.c_str());
    //if (fd == -1) {
    //    throw nn_error("file not fonud: " + prototxt);
    //}

    google::protobuf::io::FileInputStream input(fd);
    input.SetCloseOnDelete(true);

    if (!google::protobuf::TextFormat::Parse(&input, message)) {
        //throw nn_error("failed to parse");
    }
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
int create_net_from_caffe_prototxt(const std::string& caffeprototxt)
{
    caffe::NetParameter np;

    read_proto_from_text(caffeprototxt, &np);
    return create_net_from_caffe_net(np);
}
void reload_weight_from_caffe_protobinary(const std::string& caffebinary,int& input_size)
{
	caffe::NetParameter np;

	read_proto_from_binary(caffebinary, &np);
	reload_weight_from_caffe_net(np,input_size);
}

void test(const string& model_file,
          const std::string& trained_file) {
    int fc_kernel_size=create_net_from_caffe_prototxt(model_file);
	reload_weight_from_caffe_protobinary(trained_file,fc_kernel_size);
}

int main(int argc, char** argv) {
    string model_file = argv[1];
	string trained_file = argv[2];
    cout<<"model_file:"<<model_file<<endl;
    cout<<"trained_file:"<<trained_file<<endl;
	test(model_file,trained_file);
}
