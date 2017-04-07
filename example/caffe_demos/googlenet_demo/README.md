Download the caffemodel of alexnet which is provided by caffe.
    sh get_googlenet_model.sh

The testing command :
	python [python file] [deploy file] [caffemodel file] [testing image] [binaryproto file] [synset words file]
Example:
    python test_output.py deploy.prototxt bvlc_googlenet.caffemodel cat.jpg imagenet_mean.binaryproto synset_words.txt 
         
