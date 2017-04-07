Download the caffemodel of alexnet which is provided by caffe.
    sh get_vgg16_model.sh

The testing command :
	python [python file] [deploy file] [caffemodel file] [testing image] [binaryproto file] [synset words file]
Example:
    python test_output.py VGG_ILSVRC_16_layers_deploy.prototxt VGG_ILSVRC_16_layers.caffemodel cat.jpg imagenet_mean.binaryproto synset_words.txt 
         
