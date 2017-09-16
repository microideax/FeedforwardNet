<span style="display: inline-block;">

For auto generation the user should  
 - make sure the net_config_params.txt and net_weights.txt have been generated and located at FeedforwardNet/fpga_cn/caffe_converter/ folder  
 - Navigate to codeGenerator folder    
 - Run generator with command ./run_generator.sh    
 - Follow the steps with the instructions to generate the project folders for the targeted network model.      
  
Network generation example:   
             (1) Please enter test image path: (a path to the test image input)    
                 ../example/caffe_demos/lenet/lenet_2conv2max_relu/img
	     (2) Please enter test image name:
		 input_3_28.txt  
	     (3) Please enter the path to val.txt: (the path to val.txt and net_mean.txt, optional)
		 ../example/caffe_demos/lenet/lenet_2conv2max_relu/val.txt             
             (4) Please enter the type of input: float   
                 Please enter the type of weights: float   
                 Please enter the type of output: float       
             (5) Please enter color specification input (color, grayscale): grayscale   
		If the image is colored, you will be asked to enter image dimensions:       
                   - Please enter the height of the image: 375          
                   - Please enter the width of the image: 500          
           
As the result, the test_demo project for the targeted network model input is generated. The folder contains all necessary libraries and files.

The corresponding parameter settings for the accelerators will be calculated and output like this:

Accelerators:
conv_act - max_kernel: 5, max_stride: 5
pool_max_act - max_kernel: 2, max_stride: 2
