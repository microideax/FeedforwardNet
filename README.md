<span style="display: inline-block;">

# Table of Contents

FeedforwardNet is a toolkit that simplifies the development and deployment of deep learning systems 
for FPGA based devices, either embedded or resource rich. 
The toolkit enables fast mapping pre-trained network models or applications on FPGA platforms and accelerate
with pure logical hardware.

This toolkit contains a bottom up designed inference network library together with the pre-trained CAFFE model
converter and network construction flow.
This version of code is still under active development.
Future version of well integrated toolkit with a web based GUI will be released soon.

1. [caffe converter](#caffeconverter) -- converting caffemodels to weight, mean, val and network parameter files.
2. [codeGenerator](#codegenerator) -- generating the targeted network model in C++.
3. [example](#exampledesigns) -- several example caffemodels and generated projects.
4. [fpga\_cnn](#fpgacnn) -- original accelerator template lib.
5. [scripts](#scripts) -- scripts used to organize the generation flow.
6. [stb\_image](#stbimage) -- stb image lib with non-preinstallation required.


# Getting Started

1. compile caffe\_converver and convert the caffemodel file:
- cd to caffe_converter.
- run ./run.sh to compile the converter codes.
- use ./caffe_converter targeted.prototxt targeted.caffemodel [targeted.binarymodel] to convert the input caffemodel.
The detailed explanation and instructions are located in [caffe_converter](./caffe_converter).

2. generate the targeted network with the net_config_params.txt from caffe_converter:
- navigate to codeGenerator folder.
- cp ../caffe_converter/net_config_params.txt ./
- run ./run_generator.sh and follow the instruction with correct paths,file names and data types. Make sure the test_demo folder in example/ is deleted before starting this step.
The detailed explanation and instructions are located in [codeGenerator](./codeGenerator).

3. test the generated network in C++:
- navigate to FeedforwardNet/example/, the newly generated network is located here in test_demo/.
- compile the C++ code with command make.
- run ./ff_test to test the correctness.

4. FPGA synthesis and implementation:
- uncomment _HLS_MODE_ in config.h located in inference_net/.
- navigate into hls_impl/ folder.
- run ./syn.sh to start synthesis and generate the inference_net IP.


# Help and Support
If you have any suggestions and questions, please contact yao.chen@adsc.com.sg.


# Bibtex:

@misc{feedforward2016,

  author = {Yao Chen, Yang Yu, Chunrong Zhong},

  title  = {{FeedforwardNet: Enabling Efficient Convolutional Neural Network Application Design on Embedded FPGAs, Version 0.1}},

  year   = {2016},

  url    = {https://github.com/microideax/FeedforwardNet}
}
