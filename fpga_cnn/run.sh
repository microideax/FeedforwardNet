g++ -O3 examples/caffe_converter/caffe_converter_1.cpp  tiny_dnn/io/caffe/caffe.pb.cc -o caffe_converter_1 -lprotobuf -pthread -Wall -Wextra -pedantic -I ./ `pkg-config --cflags --libs opencv`


./caffe_converter_1 ./lenet/lenet_5_deploy.prototxt ./lenet/lenet_5_iter_10000.caffemodel 

