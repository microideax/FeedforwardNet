# FeedforwardNet

FeedforwardNet is a toolkit that simplifies the development and deployment of deep learning systems 
for FPGA based devices, either embedded or resource rich. 
The toolkit enables fast mapping pre-trained network models or applications on FPGA platforms and accelerate
with pure logical hardware.

The toolkit contains a bottom up designed inference network library together with the pre-trained CAFFE model
converter and network construction flow.
This version of code is under active development.
Future version of well integrated toolkit will be released soon.


# File structure

Feedforward
|
|--example   // folder contains C++ and HLS example design files and CAFFE demos.
|--fpga_cnn  // main folder contains the cnn layer designs and CAFFE converter.
|--stb_image // stb based image processing lib.
|--README.md // This README file.


# Status

FeedforwardNet is under active development and is currently un-finished.


# Help and Support

If you have any suggestions and questions, please contact yao.chen@adsc.com.sg.


# Getting Started

1. cd to FeedforwardNet/fpga_cnn/caffe_converter
2. run ./run.sh to generate the executable converter file. Pre-installed caffe and openCV is required in order to compile caffe converter.
3. cp ../../example/demo/lenet/lenet-1.prototxt ../../example/demo/lenet/lenet-1_1000_itr.caffemodel ./
4. run ./caffe_converter lenet-1.prototxt lenet-1_iter_10000.caffemodel to generate the weights.txt file.
5. Recently the automation of network construction is still under progress, so we have to construct the network model in fpga_cnn/construct_net.h as the provided example in it.
6. cp the weights.txt to example folder and rename it as weights_bias.txt.
7. cp the input.txt from example/demo/lenet to example, it is the input image file.
8. cd to example folder and run ./run.sh to generate the executable file to test the correctness of the network model.
9. run ./fftest to predict the input.txt file.
10. change the setting of _HLS_MODE_ in config.h file to enable hardware synthesis.
10. run vivado_hls -f hls_script.tcl to generate the HLS project.


# Bibtex:

@misc{feedforward2016,

  author = {Yao Chen, Yang Yu, Chunrong Zhong},

  title  = {{FeedforwardNet: Enabling Efficient Convolutional Neural Network Application Design on Embedded FPGAs, Version 0.1}},

  year   = {2016},

  url    = {https://github.com/microideax/FeedforwardNet}
}
