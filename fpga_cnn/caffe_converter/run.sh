#compile caffe converter
g++ -std=c++11 -O3 caffe_converter.cpp  caffe.pb.cc -o caffe_converter -lprotobuf -pthread -Wall -Wextra -pedantic -I ./ `pkg-config --cflags --libs opencv`
#run converter to converte targeted caffe model and weight files
#./caffe_converter ../../example/demo/lenet/lenet_2conv2max_deploy.prototxt ../../example/demo/lenet/lenet_2conv2max_iter_10000.caffemodel
./caffe_converter ../../example/demo/alexnet/deploy.prototxt ../../example/demo/alexnet/bvlc_alexnet.caffemodel 

