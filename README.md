# FeedforwardNet

Bottom up designed inference network for fast mapping CNN applications on FPGA platforms.
Fully HLS friendly inference network functions together with the CAFFE compatible model converter.

# Getting Started

Please go through the user_guide.md of the basic examples.
More documents are under progress and will be hosted in Docs folder.

# File structure

Feedforward
|
|--example // folder contains example design files. execute with run.sh
|--fpga_cnn // main folder contains the cnn layer designs
|--hls_lib  // folder that has hls synthesis support vectors
|--backup   // temp folder for file storage
|--hls_script.tcl // hls synthesis and co-sim script. run with vivado_hls


# Status

FeedforwardNet is under active development and is currently un-finished.

# Help and Support

If you have any suggestions and questions, please contact yao.chen@adsc.com.sg.

# Bibtex:

@misc{feedforward2016,

  author = {Yao Chen, Yang Yu, Chunrong Zhong},

  title  = {{FeedforwardNet: Enabling Efficient Convolutional Neural Network Application Design on Embedded FPGAs, Version 0.1}},

  year   = {2016},

  url    = {https://github.com/microideax/FeedforwardNet}
}
