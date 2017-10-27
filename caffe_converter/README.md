<span style="display: inline-block;">

Before auto generation the user should  
 - Debug caffe_converter.cpp with command in run_cpp.sh
   For general networks,there will be net_config_params.txt,net_mean.txt and net_weights.txt
   But for Resnet,only generate net_config_params.txt,net_mean.txt and 3 files else needed for auto generation     
 - For Resnet,debug weight_bias_extraction.py with command in run_py.sh
   There will be net_weights.txt and 4 files else for batch_norm_scale_layer parameters
  
