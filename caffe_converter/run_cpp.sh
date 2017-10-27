

#compile caffe converter
#g++ -std=c++11 -O3 caffe_converter.cpp  caffe.pb.cc -o caffe_converter -lprotobuf -pthread -Wall -Wextra -pedantic -I ./ `pkg-config --cflags --libs opencv`

#run converter to converte targeted caffe model and weight files
./caffe_converter ../example/caffe_demos/lenet/lenet_2conv2max_relu/lenet_2conv2max_relu_deploy.prototxt ../example/caffe_demos/lenet/lenet_2conv2max_relu/lenet_2conv2max_relu_iter_10000.caffemodel

#./caffe_converter ../example/caffe_demos/vgg16_demo/VGG_ILSVRC_16_layers_deploy.prototxt ../example/caffe_demos/vgg16_demo/VGG_ILSVRC_16_layers.caffemodel ../example/caffe_demos/vgg16_demo/imagenet_mean.binaryproto 
