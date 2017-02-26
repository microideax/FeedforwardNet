#coding=utf-8
#import libraries
import numpy as np
import matplotlib.pyplot as plt
import os,sys,caffe,time

matplotlib = 'incline'
caffe_root = os.getcwd() 

deploy_root = '/lenet_2conv2max_relu_deploy.prototxt'
caffe_model_root = '/lenet_2conv2max_relu_iter_10000.caffemodel'

# ************data_root :the location of data&weight output,automatic replacement*********
data_root = caffe_root+'/test_image_output/3/'
#data_root = caffe_root+'/test_image_output/1/'
# ************img_root :the location of test image****************************************
#img_root = '/4.bmp'
img_root = '/binarybmp/3.bmp'

bin_mean_root = '/mnist_mean.prototxt'
npy_mean_root = '/mnist_mean_py.npy'
labels_filename_root = '/synset_words.txt'
os.chdir(caffe_root)
sys.path.insert(0,caffe_root+'python')

plt.rcParams['figure.figsize'] = (8,8)
plt.rcParams['image.interpolation'] = 'nearest'


caffe.set_mode_cpu()
net = caffe.Net(caffe_root+deploy_root,
	caffe_root+caffe_model_root,
	caffe.TEST)
print "********************************************************"
print net
print "********************************************************"

[(k,v[0].data.shape) for k, v in net.params.items()]
net.blobs['input'].data.shape


def convert_mean(binMean, npyMean):
	blob = caffe.proto.caffe_pb2.BlobProto()
	bin_mean = open(binMean, 'rb' ).read()
	blob.ParseFromString(bin_mean)
	arr = np.array( caffe.io.blobproto_to_array(blob) )
	npy_mean = arr[0]
	np.save(npyMean, npy_mean)
binMean = caffe_root + bin_mean_root
npyMean = caffe_root + npy_mean_root
convert_mean(binMean, npyMean)

# *****************************image original data****************************
im = caffe.io.load_image(caffe_root+ img_root,color=False)
print 'image_data:',im.shape
# print 'image:',im
if(os.path.exists(data_root+'lenet_2conv2max_image.txt')):
	os.remove(data_root+'lenet_2conv2max_image.txt')
f=open(data_root+'lenet_2conv2max_image.txt','a')
for height in range(28):
	print >>f,'\n'+'height='+str(height) +'\n'
	for width in range(28):
		for chanel in range(1):
			f.write(str(im[height][width][chanel])+' ')


# *****************************image original data transformer****************************
transformer = caffe.io.Transformer({'data': net.blobs['input'].data.shape})
transformer.set_transpose('data', (2,0,1))
# print 'mean-substracted values:', zip(np.load(npyMean).mean(1).mean(1))
# transformer.set_mean('data', np.load(npyMean).mean(1).mean(1))  # lenet do not substract the mean
transformer.set_raw_scale('data', 255)
# transformer.set_raw_scale('data', -1) pad 255 or -1 
#transformer.set_channel_swap('data', (2,1,0))# lenet do not swap the channel
net.blobs['input'].data[...] = transformer.preprocess('data', im)

# *****************************net forward & the layer's params and data shape ***********
out = net.forward()
print '\ndata_shape\n'
print [(k, v.data.shape) for k, v in net.blobs.items()]
print '\nparams_shape\n'
print [(k, v[0].data.shape) for k, v in net.params.items()]

# *****************************input layer's data****************************

print '\ninput:',net.blobs['input'].data.shape
if(os.path.exists(data_root+'lenet_2conv2max_input.txt')):
	os.remove(data_root+'lenet_2conv2max_input.txt')
f=open(data_root+'lenet_2conv2max_input.txt','a')

#padsize = 2
#padding = ((0, 0), (0, padsize), (0, padsize)) + ((0, 0),) * (data.ndim - 3)
#data = np.pad(net.blobs['input'].data, padding, mode='constant', constant_values=(padval, padval))

for batch in range(1):
	for channel in range(1):
		print >>f,'\n'+'batch='+str(batch)+' '+'channel='+str(channel)+' '+'\n'
		for height in range(28):
			print >>f,'\n'
			for width in range(28):
				f.write(str(net.blobs['input'].data[batch][channel][height][width])+' ')
				#f.write(str(data[batch][channel][height][width])+' ')
f.close()

# *****************************convolutuon_1  layer's data & params****************************
print 'c1_data:',net.blobs['c1'].data.shape
print 'c1_params:',net.params['c1'][0].data.shape

if(os.path.exists(data_root+'lenet_2conv2max_c1_param.txt')):
	os.remove(data_root+'lenet_2conv2max_c1_param.txt')
f=open(data_root+'lenet_2conv2max_c1_param.txt','a')
f.write('\n bias: '+str(net.params['c1'][1].data)+' ')
for kernel in range(6):
 	for input_num in range(1):
 		print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
 		for height in range(5):
 			print >>f,'\n'
 			for width in range(5):
 				f.write(str(net.params['c1'][0].data[kernel][input_num][height][width])+' ')



if(os.path.exists(data_root+'lenet_2conv2max_c1.txt')):
	os.remove(data_root+'lenet_2conv2max_c1.txt')
f=open(data_root+'lenet_2conv2max_c1.txt','a')
for batch in range(1):
 	for output_num in range(6):
 		print >>f,'\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '+'\n'
 		for height in range(28):
 			print >>f,'\n'
 			for width in range(28):
 				f.write(str(net.blobs['c1'].data[batch][output_num][height][width])+' ')

f.close()


print 'relu_c1:',net.blobs['relu_c1'].data.shape
if(os.path.exists(data_root+'lenet_2conv2max_batch_relu_c1.txt')):
	os.remove(data_root+'lenet_2conv2max_batch_relu_c1.txt')
f=open(data_root+'lenet_2conv2max_batch_relu_c1.txt','a')
for batch in range(1):
	for output_num in range(6):
		print >>f,'\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '+'\n'
		for height in range(28):
			print >>f,'\n'
			for width in range(28):
				f.write(str(net.blobs['relu_c1'].data[batch][output_num][height][width])+' ')
f.close()

# *****************************pooling_s2  layer's data ****************************
print 's2_data',net.blobs['s2'].data.shape

if(os.path.exists(data_root+'lenet_2conv2max_s2.txt')):
	os.remove(data_root+'lenet_2conv2max_s2.txt')
f=open(data_root+'lenet_2conv2max_s2.txt','a')
for batch in range(1):
	for input_num in range(6):
		print >>f,'\n'+'batch='+str(batch)+' '+'input_num='+str(input_num)+' '+'\n'
		for height in range(14):
			print >>f,'\n'
			for width in range(14):
				f.write(str(net.blobs['s2'].data[batch][input_num][height][width])+' ')
f.close()

print 'relu_s2:',net.blobs['relu_s2'].data.shape

if(os.path.exists(data_root+'lenet_2conv2max_relu_s2.txt')):
	os.remove(data_root+'lenet_2conv2max_relu_s2.txt')
f=open(data_root+'lenet_2conv2max_relu_s2.txt','a')
for batch in range(1):
	for output_num in range(6):
		print >>f,'\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+'\n'
		for height in range(14):
			print >>f,'\n'
			for width in range(14):
				f.write(str(net.blobs['relu_s2'].data[batch][output_num][height][width])+' ')
f.close()

# *****************************convlution_c3  layer's data & params****************************
print 'c3_data:',net.blobs['c3'].data.shape
print 'c3_params:',net.params['c3'][0].data.shape

if(os.path.exists(data_root+'lenet_2conv2max_c3_param.txt')):
	os.remove(data_root+'lenet_2conv2max_c3_param.txt')
f=open(data_root+'lenet_2conv2max_c3_param.txt','a')
f.write('\n bias: '+str(net.params['c3'][1].data)+' ')
for batch in range(16):
 	for input_num in range(6):
 		print >>f,'\n\n'+'batch='+str(batch)+' '+'input_num='+str(input_num)+' '+'\n'
 		for height in range(5):
 			print >>f,'\n'
 			for width in range(5):
 				f.write(str(net.params['c3'][0].data[batch][input_num][height][width])+' ')

if(os.path.exists(data_root+'lenet_2conv2max_conv3.txt')):
	os.remove(data_root+'lenet_2conv2max_conv3.txt')
f=open(data_root+'lenet_2conv2max_conv3.txt','a')
for batch in range(1):
 	for output_num in range(16):
 		print >>f,'\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+'\n'
 		for height in range(10):
 			print >>f,'\n'
 			for width in range(10):
 				f.write(str(net.blobs['c3'].data[batch][output_num][height][width])+' ')

f.close()

print 'relu_c3:',net.blobs['relu_c3'].data.shape

if(os.path.exists(data_root+'lenet_2conv2max_relu_c3.txt')):
	os.remove(data_root+'lenet_2conv2max_relu_c3.txt')
f=open(data_root+'lenet_2conv2max_relu_c3.txt','a')
for batch in range(1):
	for output_num in range(16):
		print >>f,'\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+'\n'
		for height in range(10):
 			print >>f,'\n'
			for width in range(10):
 				f.write(str(net.blobs['relu_c3'].data[batch][output_num][height][width])+' ')
f.close()

# *****************************pooling_s4  layer's data ****************************
print 's4_data:',net.blobs['s4'].data.shape

if(os.path.exists(data_root+'lenet_2conv2max_s4.txt')):
	os.remove(data_root+'lenet_2conv2max_s4.txt')
f=open(data_root+'lenet_2conv2max_s4.txt','a')
for batch in range(1):
	for input_num in range(16):
		print >>f,'\n'+'batch='+str(batch)+' '+'input_num='+str(input_num)+'\n'
		for height in range(5):
 			print >>f,'\n'
			for width in range(5):
 				f.write(str(net.blobs['s4'].data[batch][input_num][height][width])+' ')

print 'relu_s4_data:',net.blobs['relu_s4'].data.shape

if(os.path.exists(data_root+'lenet_2conv2max_relu_s4.txt')):
	os.remove(data_root+'lenet_2conv2max_relu_s4.txt')
f=open(data_root+'lenet_2conv2max_relu_s4.txt','a')
for batch in range(1):
	for output_num in range(16):
		print >>f,'\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '+'\n'
		for height in range(5):
 			print >>f,'\n'
			for width in range(5):
 				f.write(str(net.blobs['relu_s4'].data[batch][output_num][height][width])+' ')

f.close()

# *****************************full connection layer's data & params****************************
print 'f5_data:',net.blobs['f5'].data.shape
print 'f5_params:',net.params['f5'][0].data.shape

if(os.path.exists(data_root+'lenet_2conv2max_f5_param.txt')):
	os.remove(data_root+'lenet_2conv2max_f5_param.txt')
f=open(data_root+'lenet_2conv2max_f5_param.txt','a')
f.write('\n bias: '+str(net.params['f5'][1].data)+' ')
for neturalUnit_num in range(10):
	print >>f,'\n\n neturalUnit_num='+str(neturalUnit_num)+'\n'
 	for map_num in range(400):
 		f.write(str(net.params['f5'][0].data[neturalUnit_num][map_num])+' ')


if(os.path.exists(data_root+'lenet_2conv2max_f5.txt')):
	os.remove(data_root+'lenet_2conv2max_f5.txt')
f=open(data_root+'lenet_2conv2max_f5.txt','a')
for batch in range(1):
  	for output_num in range(10):
  		f.write(str(net.blobs['f5'].data[batch][output_num])+' ')

# print 'tan_f5:',net.blobs['tan_f5'].data.shape
if(os.path.exists(data_root+'lenet_2conv2max_relu_f5.txt')):
	os.remove(data_root+'lenet_2conv2max_relu_f5.txt')
f=open(data_root+'lenet_2conv2max_relu_f5.txt','a')
for batch in range(1):
  	for output_num in range(10):
  		f.write(str(net.blobs['relu_f5'].data[batch][output_num])+' ')



 # *****************************probable layer's data & params****************************
output_prob = out['prob'][0]
print 'prob:',net.blobs['prob'].data.shape
if(os.path.exists(data_root+'lenet_2conv2max_prob.txt')):
	os.remove(data_root+'lenet_2conv2max_prob.txt')
f=open(data_root+'lenet_2conv2max_prob.txt','a')
f.write('\nprob layer:'+' \n')
for batch in range(1):
  	for channel in range(10):
  		f.write(str(net.blobs['prob'].data[batch][channel])+' ')

labels_filename = caffe_root + labels_filename_root
labels = np.loadtxt(caffe_root+labels_filename_root, str, delimiter='\t')
top_k = net.blobs['prob'].data[0].flatten().argsort()[::-1]

# ***************************************************************
# print output_prob.argmax()
# print output_prob[output_prob.argmax()]
# print out['prob'][0]
print "top_k,soft of the num: ",top_k

f.write('\n\npredicted class is:'+str(output_prob.argmax())+' '+str(output_prob[output_prob.argmax()])+'\n')
f.write('\n\nprobabilities and labels:'+str(zip(output_prob[top_k], labels[top_k]))+'\n')

print 'predicted class is:', output_prob.argmax(),output_prob[output_prob.argmax()]
print 'output label:', labels[output_prob.argmax()]
print 'probabilities and labels:'

# for i in np.arange(top_k.size):	
#  	print top_k[i], labels[top_k[i]]

print zip(output_prob[top_k], labels[top_k])

f.close()

