## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2015 Xilinx Inc. All rights reserved.
############################################################
open_project hls_proj_lenet

#set_top convolution_layer_with_table
#set_top pooling_layer
set_top inference_net

add_files ../../fpga_cnn/activation_functions.h
add_files ../../fpga_cnn/conv_pool_layer.h
add_files ../../fpga_cnn/pool_layer.h
add_files ../../fpga_cnn/config.h
add_files ../../fpga_cnn/construct_net.h
add_files ../../fpga_cnn/conv_layer.h
add_files ../../fpga_cnn/data_type.h
add_files ../../fpga_cnn/fc_layer.h

add_files -tb ../../fpga_cnn/accuracy.h
#add_files -tb ../../read_mnist.h
#add_files -tb ../../fpga_cnn/image_converter.h
add_files -tb ../../fpga_cnn/data_quantize.h
#add_files -tb ../../fpga_cnn/predict.h
add_files -tb ../../fpga_cnn/weight_bias.h
add_files -tb ../../stb_image/stb_image.h
add_files -tb ../../stb_image/stb_image_resize.h
add_files -tb ../../stb_image/stb_image_write.h

add_files ../ff_test.cpp
add_files -tb ../input_3.txt
add_files -tb ../weights_bias_relu.txt


add_files -cflags "-std=c++0x -fpermissive -pedantic -Wall -Wextra" -tb ff_test.cpp


open_solution -reset "fpga_cnn"
#zynq-7000 board
#set_part {xc7z045ffg900-1} 
# zedboard
set_part {xc7z020clg484-1}  
create_clock -period 10 -name default


csim_design -clean -argv {weights_bias_relu, input_3.txt}

csynth_design

cosim_design -argv {weights_bias_relu, input_3.txt} -trace_level none -rtl verilog -tool xsim
