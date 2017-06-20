## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2015 Xilinx Inc. All rights reserved.
############################################################
open_project hls_conv_pool

set_top inference_net

add_files ../inference_net/activation_functions.h
add_files ../inference_net/config.h
add_files ../inference_net/construct_net.h
add_files ../inference_net/conv_acc_break.h
add_files ../inference_net/max_pool_acc.h
add_files ../inference_net/pool_layer_one_dim.h
add_files ../inference_net/fc_layer_one_dim.h
add_files ../inference_net/data_type.h
add_files ../inference_net/pow_function.h

add_files -tb ../inference_net/accuracy_one_dim.h
add_files -tb ../inference_net/predict_one_dim.h
add_files -tb ../inference_net/softmax_one_dim.h
add_files -tb ../inference_net/weight_bias_one_dim.h
add_files -tb ../inference_net/stb_image/stb_image.h
add_files -tb ../inference_net/stb_image/stb_image_resize.h
add_files -tb ../inference_net/stb_image/stb_image_write.h

add_files ../ff_test.cpp
add_files -tb ../net_inputs/input_1.txt
add_files -tb ../net_inputs/net_weights.txt

add_files -cflags "-I/nfs/app/Xilinx/Vivado_HLS/2016.4/include -std=c++0x -fpermissive -pedantic -Wall -Wextra" -tb ../ff_test.cpp


open_solution -reset "lenet"
#vc709
#set_part {xc7vx690tffg1761-2}
#zynq-7000 board
set_part {xc7z045ffg900-1} 
# zedboard
#set_part {xc7z020clg484-1}  
create_clock -period 10 -name default

csim_design -clean -argv {net_weights.txt, input_1.txt}
#config_schedule -effort high
csynth_design

cosim_design -argv {net_weights.txt, input_1.txt} -trace_level none -rtl verilog -tool xsim

export_design -flow impl -format ip_catalog
exit
