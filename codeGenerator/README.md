For auto generation the user should  
 -  make sure the net_config_params.txt and net_weights.txt has been generated and they are located at FeedforwardNet/fpga_cn/caffe_converter folder  
 -  Navigate codeGenerator folder    
 -  Run generator with ./run_generator.sh    
 - Follow the steps as this example.      
  
For example:   
             (1)   Please enter test image path:    
                 ../example/caffe_demos/lenet/lenet_2conv2max_relu/img                               
	     (2)   Please enter the path to val.txt:                          
		 ../example/caffe_demos/lenet/lenet_2conv2max_relu/val.txt             
             (3)   Please enter the type of input: float   
                   Please enter the type of weights: float   
                   Please enter the type of output: float   
             (4) Please enter the path to test image: img/lenet_2conv2max_input.txt     
             (5) Please enter the image name: lenet_2conv2max_input.txt   
             (6) Please enter color specification input (color, grayscale): grayscale   
		If the image is colored, you will be asked to enter image dimensions:       
                   - Please enter the height of the image: 375          
                   - Please enter the width of the image: 500          
           
As the result, the test_demo folder is generated. The folder contains all necessary libraries and files
with 3 generated files, construct_net.h, config.h and ff_test.cpp.  
