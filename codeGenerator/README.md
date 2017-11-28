<span style="display: inline-block;">

For auto generation the user should  
 - make sure the net_mean.txt,net_weights.txt and net_config_params.txt (and batch_norm_mean.txt, batch_norm_denominator.txt, scale_gamma.txt, scale_beta.txt and so on) have already generated and located at FeedforwardNet/fpga_cn/caffe_converter/ folder  
 - Navigate to codeGenerator folder    
 - Run generator with command ./run_generator.sh    
 - Follow the steps with the instructions to generate the project folders for the targeted network model.      
  
Network generation example:   
             (1) Please enter test image path: (a path to the test image input)    
                 ../example/caffe_demos/lenet/lenet_2conv2max_relu/img
	     (2) Please enter test image name:
		 3.bmp
             (3) Please enter the type of input: float   
                 Please enter the type of weights: float   
                 Please enter the type of output: float       
             (4) Please enter color specification input (color, grayscale): grayscale   
		If the image is colored, you will be asked to enter image dimensions:       
                   - Please enter the height of the image: 375          
                   - Please enter the width of the image: 500          
             (5) Please enter the Tm of conv_acc: 16
                 Please enter the Tn of conv_acc: 16
                 Please enter the Tr of conv_acc: 16
                 Please enter the Tc of conv_acc: 16

 		 Please enter the Tn of pool_acc: 16
		 Please enter the Tr of pool_acc: 16
		 Please enter the Tc of pool_acc: 16

As the result, the test_demo project for the targeted network model input is generated. The folder contains all necessary libraries and files.

The corresponding parameter settings for the accelerators will be calculated and output like this:

Accelerators:
conv_act - max_kernel: 5, max_stride: 5
pool_max_act - max_kernel: 2, max_stride: 2

You've generated the network successfully!
