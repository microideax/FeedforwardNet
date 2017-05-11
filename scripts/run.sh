#!/bin/bash

rm -rf fftest out_conv.txt out_pool.txt out_fc.txt

max=60
for (( i=2; i <= max; ++i))
do
	echo -e "\n"
done


g++ -g -Wall -Wextra -pedantic -fpermissive -I /nfs/app/Xilinx/Vivado_HLS/2015.4/include -std=c++0x -I ../../fpga_cnn -I /home/yaochen/work/caffe/include/ -O3 ff_test.cpp -o fftest
#clang++ -I ../ -I /home/yaochen/work/caffe/include/ -o0 ff_test.cpp -o fftest
