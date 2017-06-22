For auto generation the user should
- Please make sure thr net_config_params.txt and net_weights.txt has been generated and they are
located at FeedforwardNet/fpga_cnn/caffe_converter folder
- Change the directory to codeGenerator folder
- Run generator:
./run_generator.sh
It will run python scripts for generation of construct_net.h, config.h and ff_test.cpp files
- You will be prompted to enter the path to the folder with test images:
“Please enter test image path: ”
- Then you will be prompted to enter the types of input data, weights and output data.
“Please enter the type of input: ”
“Please enter the type of weights: ”
“Please enter the type of output: ”
- Next the path to test img is asked:
“Please enter the path to test image”
- Then the color specification input is asked:
“Please the color specification input:”
You are asked to specify if your image is colored or grayscaled one. So you should enter “color” or
“grayscale” respectively.
- Then the height and width of the image are asked (if the image is colored one):
“height:”
“width:”
As the result, the test_demo folder is generated. The folder contains all necessary libraries and files
with 3 generated files, construct_net.h, config.h and ff_test.cpp are generated in
