#!/bin/bash

rm -rf fftest conv_layer_a.txt pool_layer_a.txt fc_layer_a.txt

max=60
for (( i=2; i <= max; ++i))
do
	echo -e "\n"
done


g++ -g -Wall -Wextra -pedantic -fpermissive -I /nfs/app/Xilinx/Vivado_HLS/2016.4/include -std=c++0x -I inference_net -I /home/yaochen/work/caffe/include/ -O3 ff_test.cpp -o fftest
#clang++ -I ../ -I /home/yaochen/work/caffe/include/ -o0 ff_test.cpp -o fftest
