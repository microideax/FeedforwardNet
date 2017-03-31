// caffe_converter.cpp : ¶šÒå¿ØÖÆÌšÓŠÓÃ³ÌÐòµÄÈë¿Úµã¡£

#include <iostream>
#include <fstream>
#include <memory>
#include <ctime>
#include <limits>
#include "caffe.pb.h"
#include "layer_factory_impl.h"
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
    out.open("net_weights.txt",ios::app);
    out<<"weights: "<<endl;
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
    out<<"bias: "<<endl;
    //load bias
    for (int o = 0; o < num_output; o++) {
     out<<src.blobs(1).data(o)<<" ";
    }
    out<<""<<endl;
    out.close();
    //cout<<endl;
}

void reload_weight_from_caffe_net(const caffe::NetParameter& layer,int input_param[])
{
    caffe_layer_vector src_net(layer);
    int num_layers = src_net.size();
    int num_input=input_param[0];
    int input_size=input_param[1];
    int num_output=0;
    cout <<"channel: "<<num_input<<endl;
    cout <<"num_layers: "<<num_layers<<endl;
    for (int i = 0; i < src_net.size(); i++) {
        int pad=0;
        int kernel_size=0;
        int stride=1;
    // name，type，kernel size，pad，stride
        cout << "Layer " << i << ":" << src_net[i].name() << "\t" << src_net[i].type()<<endl;
        if(src_net[i].type()=="Convolution"){//get conv_layers' kernel_size,num_output
            ConvolutionParameter conv_param = src_net[i].convolution_param();
            num_output=conv_param.num_output();
            cout << "num_output: " << num_output<<endl;
            if (conv_param.pad_size()>0){
                pad=conv_param.pad(0);
            }
            cout<<"pad: "<<pad<<endl;
            //google::protobuf::RepeatedField<string> repeated_field;
            //cout<<"~~~~~~~"<<endl;
            //repeated_field.size();
            //cout<<"~~~~~~~"<<endl;
           //cout<<repeated_field.size()<<endl;
            //pad=conv_param.pad(0);
            kernel_size=conv_param.kernel_size(0);
            cout << "kernel size: " << kernel_size<<endl;
            if (conv_param.stride_size()>0){
                stride=conv_param.stride(0);
            }
            cout << "stride: " << stride<<endl;
            input_size = (input_size + 2 * pad - kernel_size) / stride + 1;
            num_input=num_input/conv_param.group();
            cout << "num_input: " << num_input<<endl;
            cout << "num_output: " << num_output<<endl;
            cout << "conv_output_size: " << input_size<<endl;
        }else if(src_net[i].type()=="InnerProduct"){//get fc_layers' kernel_size,num_output
            InnerProductParameter inner_product_param = src_net[i].inner_product_param();
            kernel_size=input_size;
            cout << "kernel size: " << kernel_size<<endl;
            num_output=inner_product_param.num_output();
            cout << "num_input: " << num_input<<endl;
            cout << "num_output: " << num_output<<endl;
            input_size=1;
        }else if(src_net[i].type()=="Pooling"){//get fc_layers' kernel_size,num_output
            PoolingParameter pooling_param = src_net[i].pooling_param();
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

        if(src_net[i].type()=="Convolution"||src_net[i].type()=="InnerProduct"){
            load(src_net[i],num_input,num_output,kernel_size);
        }
        if(src_net[i].type()=="Convolution"||src_net[i].type()=="InnerProduct"||src_net[i].type()=="Pooling"){
            num_input=num_output;//set each layer's num_input equals to the last layer's num_output
        }
        cout<<endl;
        
    }
}
    
void create_net_from_caffe_net(const caffe::NetParameter& layer,int input_param[])
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
            input_param[0] = static_cast<int>(layer.layer(0).input_param().shape(0).dim(1));
            input_param[1] = static_cast<int>(layer.layer(0).input_param().shape(0).dim(2));
            //int kernel_h = static_cast<int>(layer.layer(0).input_param().shape(0).dim(2));
            //int kernel_w = static_cast<int>(layer.layer(0).input_param().shape(0).dim(3));
            //cout<<"width:********************"<<kernel_w<<endl;
            //return input_param;
        }
}

void read_proto_from_text(const std::string& prototxt,
                                 google::protobuf::Message *message) {
    int fd = CNN_OPEN_TXT(prototxt.c_str());
    if (fd == -1) {
        cout<<"file not fonud: "<<prototxt<<endl;
    }

    google::protobuf::io::FileInputStream input(fd);
    input.SetCloseOnDelete(true);

    if (!google::protobuf::TextFormat::Parse(&input, message)) {
        cout<<"failed to parse"<<endl;
    }
}

void read_proto_from_binary(const std::string& protobinary,
                                   google::protobuf::Message *message) {
    int fd = CNN_OPEN_BINARY(protobinary.c_str());
    google::protobuf::io::FileInputStream rawstr(fd);
    google::protobuf::io::CodedInputStream codedstr(&rawstr);

    rawstr.SetCloseOnDelete(true);
    codedstr.SetTotalBytesLimit(std::numeric_limits<int>::max(),
                                std::numeric_limits<int>::max() / 2);

    if (!message->ParseFromCodedStream(&codedstr)) {
        cout<<"failed to parse"<<endl;
    }
}

void create_net_from_caffe_prototxt(const std::string& caffeprototxt,int input_param[])
{
    caffe::NetParameter np;

    read_proto_from_text(caffeprototxt, &np);
    create_net_from_caffe_net(np,input_param);
}

void reload_weight_from_caffe_protobinary(const std::string& caffebinary,int input_param[])
{
	caffe::NetParameter np;

	read_proto_from_binary(caffebinary, &np);
    cout <<"net_name: "<<np.name()<<endl;
    //cout<<np.layer(0).type()<<endl;
	reload_weight_from_caffe_net(np,input_param);
}

void compute_mean(const string& mean_file)
{
    caffe::BlobProto blob;
    read_proto_from_binary(mean_file, &blob);

    vector<cv::Mat> channels;
    auto data = blob.mutable_data()->mutable_data();

    for (int i = 0; i < blob.channels(); i++, data += blob.height() * blob.width())
        channels.emplace_back(blob.height(), blob.width(), CV_32FC1, data);

    ofstream out;
    out.open("net_mean.txt",ios::app);
    cv::Mat mean;
    cv::merge(channels, mean);
    out<<cv::Mat(cv::Size(1, 1), mean.type(), cv::mean(mean))<<" ";
    out.close();
}

void test_has_mean(const string& model_file,
          const string& trained_file,
          const string& mean_file) {
    int input_param[]={0,0};
    create_net_from_caffe_prototxt(model_file,input_param);
	reload_weight_from_caffe_protobinary(trained_file,input_param);
    compute_mean(mean_file);
}

void test_no_mean(const string& model_file,
          const string& trained_file) {
    int input_param[]={0,0};
    create_net_from_caffe_prototxt(model_file,input_param);
    reload_weight_from_caffe_protobinary(trained_file,input_param);
}

int main(int argc, char** argv) {
    cout<<"argc:"<<argc<<endl;
    int arg_channel = 1;
    string model_file = argv[arg_channel++];
	string trained_file = argv[arg_channel++];
    cout<<"model_file:"<<model_file<<endl;
    cout<<"trained_file:"<<trained_file<<endl;
    try  
    {  
        if(argc==4){
            string mean_file = argv[arg_channel++];
			cout << "mean_file:" << mean_file << endl;
            test_has_mean(model_file,trained_file,mean_file);
        }else if(argc==3){
            test_no_mean(model_file,trained_file);
        }
    }  
    catch (exception& e)  
    {  
        cout << "Standard exception: " << e.what() << endl;  
    }  
}
