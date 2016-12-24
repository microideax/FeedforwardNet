## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2015 Xilinx Inc. All rights reserved.
############################################################
open_project hls_proj

#set_top convolution_layer_with_table
set_top pooling_layer

add_files ../fpga_cnn/average_pooling.h
add_files ../fpga_cnn/convolution.h
add_files ../fpga_cnn/activation_functions.h
add_files ../fpga_cnn/data_type.h
add_files ../fpga_cnn/config.h
#add_files ../fpga_cnn/fully_connect.h

add_files ../fpga_cnn/hls_lib/static_vector.h

add_files -tb ../fpga_cnn/image_converter.h
add_files -tb ../stb_image/stb_image.h
add_files -tb ../stb_image/stb_image_resize.h
add_files -tb ../stb_image/stb_image_write.h


add_files ff_test.cpp
add_files -tb 4.bmp
add_files -tb LeNet-weights


add_files -cflags "-std=c++0x -fpermissive -pedantic -Wall -Wextra" -tb ff_test.cpp


open_solution -reset "fpga_cnn"
set_part {xc7vx690tffg1761-2}
create_clock -period 10 -name default


csim_design -clean -argv {LeNet-weights, 4.bmp}

csynth_design

cosim_design -argv {LeNet-weights, 4.bmp} -trace_level none -rtl verilog -tool xsim



