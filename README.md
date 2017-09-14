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
This is the main folder contains the cnn layer accelerators.

<a name="scripts"></a>
# scripts
This folder contains all necessary scripts used for accelerator configuration and network generation.

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

1. compile caffe\_converver and convert the caffemodel file:
- cd to caffe_converter.
- run ./run.sh to compile the converter codes.
- use ./caffe_converter targeted.prototxt targeted.caffemodel [targeted.binarymodel] to convert the input caffemodel.

2. generate the targeted network with the net_config_params.txt from caffe_converter:
- navigate to codeGenerator folder.
- cp ../caffe_converter/net_config_params.txt ./
- run ./run_generator.sh and follow the instruction with correct paths,file names and data types.

3. test the generated network in C++:
- navigate to FeedforwardNet/example/, the newly generated network is located here in test_demo/.
- compile the C++ code with command make.
- run ./ff_test to test the correctness.

4. FPGA synthesis and implementation:
- uncomment _HLS_MODE_ in config.h located in inference_net/.
- navigate into hls_impl/ folder.
- run ./syn.sh to start synthesis and generate the inference_net IP.


# Bibtex:

@misc{feedforward2016,

  author = {Yao Chen, Yang Yu, Chunrong Zhong},

  title  = {{FeedforwardNet: Enabling Efficient Convolutional Neural Network Application Design on Embedded FPGAs, Version 0.1}},

  year   = {2016},

  url    = {https://github.com/microideax/FeedforwardNet}
}
