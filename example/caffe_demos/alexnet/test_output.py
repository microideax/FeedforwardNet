#coding=utf-8
#import libraries
import numpy as np
import matplotlib.pyplot as plt
import os,sys,caffe,time

matplotlib = 'incline'
model_dir = os.getcwd() +'/'
deploy_filename = 'deploy.prototxt'
caffemodel_filename = 'bvlc_alexnet.caffemodel'
img_root = 'cat.jpg'
bin_mean_filename = 'imagenet_mean.binaryproto'
npy_mean_filename = 'imagenet_mean.npy'
labels_filename = 'synset_words.txt'
output_dir = 'output/'
plt.rcParams['figure.figsize'] = (8,8)
plt.rcParams['image.interpolation'] = 'nearest'

caffe.set_mode_cpu()
# print "**********************************************************************************"
net = caffe.Net(model_dir+deploy_filename,
	model_dir+caffemodel_filename,
	caffe.TEST)

[(k,v[0].data.shape) for k, v in net.params.items()]
net.blobs['data'].data.shape

def show_data(data,title,padsize=1,padval=0):
	data -= data.min()
	data /= data.max()
	n = int(np.ceil(np.sqrt(data.shape[0])))
	padding = ((0, n ** 2 - data.shape[0]), (0, padsize), (0, padsize)) + ((0, 0),) * (data.ndim - 3)
	data = np.pad(data,padding,mode='constant',constant_values=(padval, padval))
	# tile the filters into an image
	data = data.reshape((n, n) + data.shape[1:]).transpose((0, 2, 1, 3) + tuple(range(4, data.ndim + 1)))
	data = data.reshape((n * data.shape[1], n * data.shape[3]) + data.shape[4:])
	plt.figure(), plt.title(title)
	plt.imshow(data, cmap='gray')
	# plt.imshow(data)
	plt.axis('off')

def convert_mean(binMean, npyMean):
	blob = caffe.proto.caffe_pb2.BlobProto()
	bin_mean = open(binMean, 'rb' ).read()
	blob.ParseFromString(bin_mean)
	arr = np.array( caffe.io.blobproto_to_array(blob) )
	npy_mean = arr[0]
	np.save(npyMean, npy_mean)
binMean = model_dir + bin_mean_filename
npyMean = model_dir + npy_mean_filename
convert_mean(binMean, npyMean)

start = time.time()

im = caffe.io.load_image(model_dir + img_root)



transformer = caffe.io.Transformer({'data': net.blobs['data'].data.shape})
transformer.set_transpose('data', (2,0,1))
transformer.set_mean('data', np.load(npyMean).mean(1).mean(1))
transformer.set_raw_scale('data', 255)
transformer.set_channel_swap('data', (2,1,0))
net.blobs['data'].data[...] = transformer.preprocess('data', im)



out = net.forward()
print '\ndata_shape\n'
print [(k, v.data.shape) for k, v in net.blobs.items()]
print '\nbias_shape\n'
print [(k, v[0].data.shape) for k, v in net.params.items()]
print '\nparams_shape\n'
print [(k, v[0].data.shape) for k, v in net.params.items()]

##如果只要visualizatyion，可把下面写文件部分注释掉

##*****************************************writing the data&params*************************#
# image
print 'image_data:',im.shape

if(os.path.exists(output_dir+'alexnet_image.txt')):
	os.remove(output_dir+'alexnet_image.txt')
f=open(output_dir+'alexnet_image.txt','a')
for height in range(260):
	print >>f,'\n'+'height='+str(height) +'\n'
	for width in range(480):
		for chanel in range(3):
			f.write(str(im[height][width][chanel])+' ')

# mean substracted values
print 'mean-substracted values:', zip(np.load(npyMean).mean(1).mean(1))
if(os.path.exists(output_dir+'alexnet_mean.txt')):
	os.remove(output_dir+'alexnet_mean.txt')
f=open(output_dir+'alexnet_mean.txt','a')
f.write('\n\nmean-substracted values::'+str(zip(np.load(npyMean).mean(1).mean(1)))+'\n')

# data 
print '\ninput:',net.blobs['data'].data.shape
if(os.path.exists(output_dir+'alexnet_data.txt')):
	os.remove(output_dir+'alexnet_data.txt')
f=open(output_dir+'alexnet_data.txt','a')
for batch in range(10):
	for channel in range(3):
		print >>f,'\n'+'batch='+str(batch)+' '+'channel='+str(channel)+' '+'\n'
		for height in range(227):
			print >>f,'\n'
			for width in range(227):
				f.write(str(net.blobs['data'].data[batch][channel][height][width])+' ')

#conv1
print 'conv1:',net.blobs['conv1'].data.shape
if(os.path.exists(output_dir+'alexnet_conv1.txt')):
	os.remove(output_dir+'alexnet_conv1.txt')
f=open(output_dir+'alexnet_conv1.txt','a')
f.write('\nconv1 layer:'+' \n')
for batch in range(10):
  	for output_num in range(96):
  		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
  		for height in range(55):
  		 	print >>f,'\n'
  		 	for width in range(55):
  				f.write(str(net.blobs['conv1'].data[batch][output_num][height][width])+' ')

print 'conv1_bias:',net.params['conv1'][1].data.shape
if(os.path.exists(output_dir+'alexnet_conv1_bias.txt')):
	os.remove(output_dir+'alexnet_conv1_bias.txt')
f=open(output_dir+'alexnet_conv1_bias.txt','a')
for kernel in range(96):
	f.write(str(net.params['conv1'][1].data[kernel])+'  ')
print 'conv1_params:',net.params['conv1'][0].data.shape
if(os.path.exists(output_dir+'alexnet_conv1_param.txt')):
	os.remove(output_dir+'alexnet_conv1_param.txt')
f=open(output_dir+'alexnet_conv1_param.txt','a')
for kernel in range(96):
 	for input_num in range(3):
 		print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
 		for height in range(11):
 			print >>f,'\n'
 			for width in range(11):
 				f.write(str(net.params['conv1'][0].data[kernel][input_num][height][width])+' ')

print 'norm1:',net.blobs['norm1'].data.shape
if(os.path.exists(output_dir+'alexnet_norm1.txt')):
	os.remove(output_dir+'alexnet_norm1.txt')
f=open(output_dir+'alexnet_norm1.txt','a')
f.write('\nnorm1 layer:'+' \n')
for batch in range(10):
  	for output_num in range(96):
  		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
  		for height in range(55):
  		 	print >>f,'\n'
  		 	for width in range(55):
  				f.write(str(net.blobs['norm1'].data[batch][output_num][height][width])+' ')

print 'pool1:',net.blobs['pool1'].data.shape
if(os.path.exists(output_dir+'alexnet_pool1.txt')):
	os.remove(output_dir+'alexnet_pool1.txt')
f=open(output_dir+'alexnet_pool1.txt','a')
f.write('\npool1 layer:'+' \n')
for batch in range(10):
  	for output_num in range(96):
  		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
  		for height in range(27):
  		 	print >>f,'\n'
  		 	for width in range(27):
  				f.write(str(net.blobs['pool1'].data[batch][output_num][height][width])+' ')

#conv2
print 'conv2:',net.blobs['conv2'].data.shape
if(os.path.exists(output_dir+'alexnet_conv2.txt')):
	os.remove(output_dir+'alexnet_conv2.txt')
f=open(output_dir+'alexnet_conv2.txt','a')
f.write('\nconv2 layer:'+' \n')
for batch in range(10):
  	for output_num in range(256):
  		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
  		for height in range(27):
  		 	print >>f,'\n'
  		 	for width in range(27):
  				f.write(str(net.blobs['conv2'].data[batch][output_num][height][width])+' ')

print 'conv2_bias:',net.params['conv2'][1].data.shape
if(os.path.exists(output_dir+'alexnet_conv2_bias.txt')):
	os.remove(output_dir+'alexnet_conv2_bias.txt')
f=open(output_dir+'alexnet_conv2_bias.txt','a')
for kernel in range(256):
	f.write(str(net.params['conv2'][1].data[kernel])+'  ')
print 'conv2_params:',net.params['conv2'][0].data.shape
if(os.path.exists(output_dir+'alexnet_conv2_param.txt')):
	os.remove(output_dir+'alexnet_conv2_param.txt')
f=open(output_dir+'alexnet_conv2_param.txt','a')
for kernel in range(256):
 	for input_num in range(48):
 		print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
 		for height in range(5):
 			print >>f,'\n'
 			for width in range(5):
 				f.write(str(net.params['conv2'][0].data[kernel][input_num][height][width])+' ')

print 'norm2:',net.blobs['norm2'].data.shape
if(os.path.exists(output_dir+'alexnet_norm2.txt')):
	os.remove(output_dir+'alexnet_norm2.txt')
f=open(output_dir+'alexnet_norm2.txt','a')
f.write('\nnorm2 layer:'+' \n')
for batch in range(10):
  	for output_num in range(256):
  		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
  		for height in range(27):
  		 	print >>f,'\n'
  		 	for width in range(27):
  				f.write(str(net.blobs['norm2'].data[batch][output_num][height][width])+' ')

print 'pool2:',net.blobs['pool2'].data.shape
if(os.path.exists(output_dir+'alexnet_pool2.txt')):
	os.remove(output_dir+'alexnet_pool2.txt')
f=open(output_dir+'alexnet_pool2.txt','a')
f.write('\npool2 layer:'+' \n')
for batch in range(10):
  	for output_num in range(256):
  		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
  		for height in range(13):
  		 	print >>f,'\n'
  		 	for width in range(13):
  				f.write(str(net.blobs['pool2'].data[batch][output_num][height][width])+' ')

#conv3
print 'conv3:',net.blobs['conv3'].data.shape
if(os.path.exists(output_dir+'alexnet_conv3.txt')):
	os.remove(output_dir+'alexnet_conv3.txt')
f=open(output_dir+'alexnet_conv3.txt','a')
f.write('\nconv3 layer:'+' \n')
for batch in range(10):
  	for output_num in range(384):
  		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
  		for height in range(13):
  		 	print >>f,'\n'
  		 	for width in range(13):
  				f.write(str(net.blobs['conv3'].data[batch][output_num][height][width])+' ')

print 'conv3_bias:',net.params['conv3'][1].data.shape
if(os.path.exists(output_dir+'alexnet_conv3_bias.txt')):
	os.remove(output_dir+'alexnet_conv3_bias.txt')
f=open(output_dir+'alexnet_conv3_bias.txt','a')
for kernel in range(384):
	f.write(str(net.params['conv3'][1].data[kernel])+'  ')
print 'conv3_params:',net.params['conv3'][0].data.shape
if(os.path.exists(output_dir+'alexnet_conv3_param.txt')):
	os.remove(output_dir+'alexnet_conv3_param.txt')
f=open(output_dir+'alexnet_conv3_param.txt','a')
for kernel in range(384):
 	for input_num in range(256):
 		print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
 		for height in range(3):
 			print >>f,'\n'
 			for width in range(3):
 				f.write(str(net.params['conv3'][0].data[kernel][input_num][height][width])+' ')

#conv4
print 'conv4:',net.blobs['conv4'].data.shape
if(os.path.exists(output_dir+'alexnet_conv4.txt')):
	os.remove(output_dir+'alexnet_conv4.txt')
f=open(output_dir+'alexnet_conv4.txt','a')
f.write('\nconv4 layer:'+' \n')
for batch in range(10):
  	for output_num in range(384):
  		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
  		for height in range(13):
  		 	print >>f,'\n'
  		 	for width in range(13):
  				f.write(str(net.blobs['conv4'].data[batch][output_num][height][width])+' ')

print 'conv4_bias:',net.params['conv4'][1].data.shape
if(os.path.exists(output_dir+'alexnet_conv4_bias.txt')):
	os.remove(output_dir+'alexnet_conv4_bias.txt')
f=open(output_dir+'alexnet_conv4_bias.txt','a')
for kernel in range(384):
	f.write(str(net.params['conv4'][1].data[kernel])+'  ')
print 'conv4_params:',net.params['conv4'][0].data.shape
if(os.path.exists(output_dir+'alexnet_conv4_param.txt')):
	os.remove(output_dir+'alexnet_conv4_param.txt')
f=open(output_dir+'alexnet_conv4_param.txt','a')
for kernel in range(384):
 	for input_num in range(192):
 		print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
 		for height in range(3):
 			print >>f,'\n'
 			for width in range(3):
 				f.write(str(net.params['conv4'][0].data[kernel][input_num][height][width])+' ')

#conv5
print 'conv5:',net.blobs['conv5'].data.shape
if(os.path.exists(output_dir+'alexnet_conv5.txt')):
	os.remove(output_dir+'alexnet_conv5.txt')
f=open(output_dir+'alexnet_conv5.txt','a')
f.write('\nconv5 layer:'+' \n')
for batch in range(10):
  	for output_num in range(256):
  		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
  		for height in range(13):
  		 	print >>f,'\n'
  		 	for width in range(13):
  				f.write(str(net.blobs['conv5'].data[batch][output_num][height][width])+' ')

print 'conv5_bias:',net.params['conv5'][1].data.shape
if(os.path.exists(output_dir+'alexnet_conv5_bias.txt')):
	os.remove(output_dir+'alexnet_conv5_bias.txt')
f=open(output_dir+'alexnet_conv5_bias.txt','a')
for kernel in range(256):
	f.write(str(net.params['conv5'][1].data[kernel])+'  ')
print 'conv5_params:',net.params['conv5'][0].data.shape
if(os.path.exists(output_dir+'alexnet_conv5_param.txt')):
	os.remove(output_dir+'alexnet_conv5_param.txt')
f=open(output_dir+'alexnet_conv5_param.txt','a')
for kernel in range(256):
 	for input_num in range(192):
 		print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
 		for height in range(3):
 			print >>f,'\n'
 			for width in range(3):
 				f.write(str(net.params['conv5'][0].data[kernel][input_num][height][width])+' ')


print 'pool5:',net.blobs['pool5'].data.shape
if(os.path.exists(output_dir+'alexnet_pool5.txt')):
	os.remove(output_dir+'alexnet_pool5.txt')
f=open(output_dir+'alexnet_pool5.txt','a')
f.write('\npool5 layer:'+' \n')
for batch in range(10):
  	for output_num in range(256):
  		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
  		for height in range(6):
  		 	print >>f,'\n'
  		 	for width in range(6):
  				f.write(str(net.blobs['pool5'].data[batch][output_num][height][width])+' ')

#fc6
print 'fc6:',net.blobs['fc6'].data.shape

if(os.path.exists(output_dir+'alexnet_fc6.txt')):
	os.remove(output_dir+'alexnet_fc6.txt')
f=open(output_dir+'alexnet_fc6.txt','a')
for batch in range(10):
  	for output_num in range(4096):
  		f.write(str(net.blobs['fc6'].data[batch][output_num])+' ')


print 'fc6_bias:',net.params['fc6'][1].data.shape
if(os.path.exists(output_dir+'alexnet_fc6_bias.txt')):
	os.remove(output_dir+'alexnet_fc6_bias.txt')
f=open(output_dir+'alexnet_fc6_bias.txt','a')
for neturalUnit_num in range(4096):
 	f.write(str(net.params['fc6'][1].data[neturalUnit_num])+' ')
print 'fc6_params:',net.params['fc6'][0].data.shape
if(os.path.exists(output_dir+'alexnet_fc6_param.txt')):
	os.remove(output_dir+'alexnet_fc6_param.txt')
f=open(output_dir+'alexnet_fc6_param.txt','a')
for neturalUnit_num in range(4096):
	print >>f,'\n\n neturalUnit_num='+str(neturalUnit_num)+'\n'
 	for map_num in range(9216):
 		f.write(str(net.params['fc6'][0].data[neturalUnit_num][map_num])+' ')

#fc7
print 'fc7:',net.blobs['fc7'].data.shape

if(os.path.exists(output_dir+'alexnet_fc7.txt')):
	os.remove(output_dir+'alexnet_fc7.txt')
f=open(output_dir+'alexnet_fc7.txt','a')
for batch in range(10):
  	for output_num in range(4096):
  		f.write(str(net.blobs['fc7'].data[batch][output_num])+' ')


print 'fc7_bias:',net.params['fc7'][1].data.shape
if(os.path.exists(output_dir+'alexnet_fc7_bias.txt')):
	os.remove(output_dir+'alexnet_fc7_bias.txt')
f=open(output_dir+'alexnet_fc7_bias.txt','a')
for neturalUnit_num in range(4096):
 	f.write(str(net.params['fc7'][1].data[neturalUnit_num])+' ')

print 'fc7_params:',net.params['fc7'][0].data.shape
if(os.path.exists(output_dir+'alexnet_fc7_param.txt')):
	os.remove(output_dir+'alexnet_fc7_param.txt')
f=open(output_dir+'alexnet_fc7_param.txt','a')
for neturalUnit_num in range(4096):
	print >>f,'\n\n neturalUnit_num='+str(neturalUnit_num)+'\n'
 	for map_num in range(4096):
 		f.write(str(net.params['fc7'][0].data[neturalUnit_num][map_num])+' ')

#fc8
print 'fc8:',net.blobs['fc8'].data.shape

if(os.path.exists(output_dir+'alexnet_fc8.txt')):
	os.remove(output_dir+'alexnet_fc8.txt')
f=open(output_dir+'alexnet_fc8.txt','a')
for batch in range(10):
  	for output_num in range(1000):
  		f.write(str(net.blobs['fc8'].data[batch][output_num])+' ')


print 'fc8_bias:',net.params['fc8'][1].data.shape
if(os.path.exists(output_dir+'alexnet_fc8_bias.txt')):
	os.remove(output_dir+'alexnet_fc8_bias.txt')
f=open(output_dir+'alexnet_fc8_bias.txt','a')
for neturalUnit_num in range(1000):
 	f.write(str(net.params['fc8'][1].data[neturalUnit_num])+' ')
print 'fc8_params:',net.params['fc8'][0].data.shape
if(os.path.exists(output_dir+'alexnet_fc8_param.txt')):
	os.remove(output_dir+'alexnet_fc8_param.txt')
f=open(output_dir+'alexnet_fc8_param.txt','a')
for neturalUnit_num in range(1000):
	print >>f,'\n\n neturalUnit_num='+str(neturalUnit_num)+'\n'
 	for map_num in range(4096):
 		f.write(str(net.params['fc8'][0].data[neturalUnit_num][map_num])+' ')



 # prob
print 'prob:',net.blobs['prob'].data.shape
if(os.path.exists(output_dir+'alexnet_prob.txt')):
	os.remove(output_dir+'alexnet_prob.txt')
f=open(output_dir+'alexnet_prob.txt','a')
f.write('\nprob layer:'+' \n')
for batch in range(10):
  	for channel in range(1000):
  		f.write(str(net.blobs['prob'].data[batch][channel])+' ')



output_prob = out['prob'][0]

labels = np.loadtxt(model_dir+labels_filename, str, delimiter='\t')
top_k = net.blobs['prob'].data[0].flatten().argsort()[::-1]

# predict 
print 'predict:'
if(os.path.exists(output_dir+'alexnet_predict.txt')):
	os.remove(output_dir+'alexnet_predict.txt')
f=open(output_dir+'alexnet_predict.txt','a')
f.write('\nprob layer:'+' \n')
#print "top_k,soft of the num: ",top_k

f.write('\n\npredicted class is:'+str(output_prob.argmax())+' '+str(output_prob[output_prob.argmax()])+'\n')
f.write('\n\nprobabilities and labels:'+str(zip(output_prob[top_k], labels[top_k]))+'\n')

# print 'predicted class is:', output_prob.argmax(),output_prob[output_prob.argmax()]
# print 'output label:', labels[output_prob.argmax()]
# print 'probabilities and labels:'

##*****************************************writing the data&params*************************##


print("\nDone in %.2f s.\n" % (time.time() - start))#计算时间



####*************visualization *****************##
show_data(net.params['conv5'][0].data[0],"conv5_params")
show_data(net.blobs['conv5'].data[0],"conv5")

show_data(net.params['conv4'][0].data[0],"conv4_params")
show_data(net.blobs['conv4'].data[0],"conv4")

show_data(net.params['conv3'][0].data[0],"conv3_params")
show_data(net.blobs['conv3'].data[0],"conv3")

show_data(net.blobs['pool2'].data[0],"pool2")
show_data(net.blobs['norm2'].data[0], "norm2")
show_data(net.params['conv2'][0].data[0],"conv2_params")
show_data(net.blobs['conv2'].data[0],"conv2")


show_data(net.blobs['pool1'].data[0],"pool1")
show_data(net.blobs['norm1'].data[0], "norm1")
show_data(net.params['conv1'][0].data.transpose(0,2,3,1).squeeze(),'conv1_params')
show_data(net.blobs['conv1'].data[0], "conv1")

show_data(net.blobs['data'].data[0], "data")

plt.imshow(im)
plt.axis('off')



plt.show()
