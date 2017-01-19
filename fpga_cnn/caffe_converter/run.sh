#compile caffe converter
g++ -O3 caffe_converter.cpp  caffe.pb.cc -o caffe_converter -lprotobuf -pthread -Wall -Wextra -pedantic -I ./ `pkg-config --cflags --libs opencv`
#run converter to converte targeted caffe model and weight files
./caffe_converter ../../example/demo/lenet/lenet_5_deploy.prototxt ../../example/demo/lenet/lenet_5_iter_10000.caffemodel 

