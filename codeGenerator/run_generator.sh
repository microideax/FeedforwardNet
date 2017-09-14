#!/bin/bash

mkdir -p ../example/test_demo
mkdir -p ../example/test_demo/hls_impl
mkdir -p ../example/test_demo/inference_net
mkdir -p ../example/test_demo/inference_net/stb_image
mkdir -p ../example/test_demo/net_inputs

copy_file(){
	if [ -e $1 ]
	then
	    if [[ $3 -eq 1 ]]
	    then
	    	cp -f  $1 $2
	    else
		cp -r  $1 $2
	    fi
	else
	    echo "No $1 file"
	fi
}

printf "Please make sure the net_weights.txt and net_config_params.txt have already generated in caffe_converter folder\n\n"

read -p "Please enter test image path: "  test_img_folder
read -p "Please enter test image name: "  test_img_name

read -p "Please enter the path to val.txt: "  val_path

prm_file_name="net_config_params.txt"

if [ -f $prm_file_name ] ; then
    rm $prm_file_name
fi

copy_file "../caffe_converter/$prm_file_name" "." 1

if grep -q "lrn" "$prm_file_name"; 
then
copy_file "../fpga_cnn/lrn_layer_one_dim.h" "../example/test_demo/inference_net/" 1
fi

#if grep -q "fc" "$prm_file_name"; 
#then
#copy_file "../fpga_cnn/fc_layer_one_dim.h" "../example/test_demo/inference_net/" 1
#fi



copy_file "../scripts/hls_script.tcl" "../example/test_demo/hls_impl/" 1
copy_file "../scripts/syn.sh" "../example/test_demo/hls_impl/" 1
copy_file "../stb_image" "../example/test_demo/inference_net/" 2
copy_file "../fpga_cnn/data_type.h" "../example/test_demo/inference_net/" 1
copy_file "../fpga_cnn/activation_functions.h" "../example/test_demo/inference_net/" 1
copy_file "../fpga_cnn/pow_function.h" "../example/test_demo/inference_net/" 1
copy_file "../fpga_cnn/softmax_one_dim.h" "../example/test_demo/inference_net/" 1
copy_file "../fpga_cnn/weight_bias_one_dim.h" "../example/test_demo/inference_net/" 1
copy_file "../fpga_cnn/image_converter.h" "../example/test_demo/inference_net/" 1

mkdir ../example/test_demo/net_inputs/test_imgs
copy_file "$test_img_folder/$test_img_name" "../example/test_demo/net_inputs/test_imgs/" 1



copy_file "../caffe_converter/net_mean.txt" "../example/test_demo/net_inputs/" 1
copy_file "../caffe_converter/net_weights.txt" "../example/test_demo/net_inputs/" 1

copy_file "$val_path" "../example/test_demo/net_inputs/val.txt" 1

copy_file "../scripts/Makefile" "../example/test_demo/" 1
copy_file "../fpga_cnn/predict_one_dim.h" "../example/test_demo/inference_net/" 1
copy_file "../fpga_cnn/accuracy_one_dim.h" "../example/test_demo/inference_net/" 1
copy_file "../fpga_cnn/resize_image.h" "../example/test_demo/inference_net/" 1


#copy_file "../fpga_cnn/conv_acc_innerdf_1.h" "../example/test_demo/inference_net/" 1
#copy_file "../fpga_cnn/conv_acc_break_noact.h" "../example/test_demo/inference_net/" 1
#copy_file "../fpga_cnn/ave_pool_acc_innerdf.h" "../example/test_demo/inference_net/" 1
#copy_file "../fpga_cnn/ave_pool_acc_noact.h" "../example/test_demo/inference_net/" 1
#copy_file "../fpga_cnn/max_pool_acc_innerdf.h" "../example/test_demo/inference_net/" 1
#copy_file "../fpga_cnn/max_pool_acc_noact.h" "../example/test_demo/inference_net/" 1



python generator_config.py $prm_file_name
python generator_ff_test.py $prm_file_name $test_img_name
python generator_acc_instance.py $prm_file_name
python generator.py $prm_file_name 
python generator_conv_acc.py $prm_file_name 
python generator_max_pool_acc.py $prm_file_name 
python generator_ave_pool_acc.py $prm_file_name 