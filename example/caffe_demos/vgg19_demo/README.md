Download the caffemodel of alexnet which is provided by caffe.
    sh get_vgg19_model.sh

The testing command :
	python [python file] [deploy file] [caffemodel file] [testing image] [binaryproto file] [synset words file]
Example:
    python test_output.py VGG_ILSVRC_19_layers_deploy.prototxt VGG_ILSVRC_19_layers.caffemodel cat.jpg imagenet_mean.binaryproto synset_words.txt 
         
