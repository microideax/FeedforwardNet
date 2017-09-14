<span style="display: inline-block;">

# Table of Contents
1. [caffe converter](#caffeconverter)
2. [codeGenerator](#codegenerator)
3. [example](#exampledesigns)
4. [fpga\_cnn](#fpgacnn)
5. [scripts](#scripts)
6. [stb\_image](#stbimage)
7. [README.md](#readme)

# FeedforwardNet

FeedforwardNet is a toolkit that simplifies the development and deployment of deep learning systems 
for FPGA based devices, either embedded or resource rich. 
The toolkit enables fast mapping pre-trained network models or applications on FPGA platforms and accelerate
with pure logical hardware.

This toolkit contains a bottom up designed inference network library together with the pre-trained CAFFE model
converter and network construction flow.
This version of code is still under active development.
Future version of well integrated toolkit with a web based GUI will be released soon.

<a name="caffeconverter"></a>
# caffe_converter
This tool is used to converte caffemodel file to weight, mean and val data files.

<a name="codegenerator"></a>
# codeGenerator
This is the automated tool that generates the targeted network model.

<a name="exampledesigns"></a>
# example   
This folder contains demoe example designs and original CAFFE demos.

<a name="fpgacnn"></a>
# fpga\_cnn  
This is the main folder contains the cnn layer designs and CAFFE converter.

<a name="stb\_image"></a>
# stb\_image 
stb based image processing lib.

<a name="readme"></a>
# README.md 
This README file.


# Status

FeedforwardNet is under active development and is currently un-finished.


# Help and Support

If you have any suggestions and questions, please contact yao.chen@adsc.com.sg.


# Getting Started

1. cd to FeedforwardNet/fpga_cnn/caffe_converter
2. run ./run.sh to generate the executable converter file. Pre-installed caffe and openCV is required in order to compile caffe converter.
3. cp ../../example/caffe_demos/lenet/lenet_2conv2max/lenet-1.prototxt ../../example/caffe_demos/lenet/lenet_2conv2max/lenet-1_1000_itr.caffemodel ./
4. run ./caffe_converter lenet-1.prototxt lenet-1_iter_10000.caffemodel to generate the weights.txt file.
5. Recently the automation of network construction is still under progress, so we have to construct the network model in fpga_cnn/construct_net.h as the provided example in it.
6. cp the weights.txt to example folder and rename it as weights_lenet.txt.
7. cp the input.txt(or example picture) from example/demo/lenet to example and rename as input_3.txt, it is the input image file.
8. change the compilation mode in config.h, _HLS_MODE_ enables HLS process.
    for C compilation, comment _HLS_MODE_. 
    If test with BATCH data, then comment _KERNEL_MODE_, otherwise the default is processing single picture instead of dataset.
9. cd to example folder and run ./run.sh to generate the executable file of the C++ design to test the correctness of the network model.
10. run ./fftest to predict the input file (or run the test dataset).
11. change the setting of _HLS_MODE_ in config.h file to enable hardware synthesis.
12. cd to hls_impl folder and run ./syn.sh to generate and start the HLS project.


# Bibtex:

@misc{feedforward2016,

  author = {Yao Chen, Yang Yu, Chunrong Zhong},

  title  = {{FeedforwardNet: Enabling Efficient Convolutional Neural Network Application Design on Embedded FPGAs, Version 0.1}},

  year   = {2016},

  url    = {https://github.com/microideax/FeedforwardNet}
}
