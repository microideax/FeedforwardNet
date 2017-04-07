#coding=utf-8
#import libraries
import numpy as np
import matplotlib.pyplot as plt
import os,sys,caffe,time

matplotlib = 'incline'
model_dir = os.getcwd() +'/'
deploy_filename = 'VGG_ILSVRC_19_layers_deploy.prototxt'
caffemodel_filename = 'VGG_ILSVRC_19_layers.caffemodel'
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
print [(k, v[1].data.shape) for k, v in net.params.items()]
print '\nparams_shape\n'
print [(k, v[0].data.shape) for k, v in net.params.items()]

for k,v in net.blobs.items():
	print 'layer:',k,v.data.shape
	if(os.path.exists(output_dir+'vgg19_'+str(k)+'.txt')):
		os.remove(output_dir+'vgg19_'+str(k)+'.txt')
	f=open(output_dir+'vgg4*_'+str(k)+'.txt','a')
	if(v.data.ndim!=2):
		#print 'shape:',v.data.shape[0],v.data.shape[1],v.data.shape[2],v.data.shape[3]
		# print 'conv1_1:',net.blobs['conv1_1'].data.shape
		f.write('\nconv1_1 layer:'+' \n')
		for batch in range(v.data.shape[0]):
		  	for output_num in range(v.data.shape[1]):
		  		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
		  		for height in range(v.data.shape[2]):
		  		 	print >>f,'\n'
		  		 	for width in range(v.data.shape[3]):
		  				f.write(str(net.blobs[k].data[batch][output_num][height][width])+' ')
	else:
		for batch in range(v.data.shape[0]):
		  	for output_num in range(v.data.shape[1]):
		  		f.write(str(net.blobs[k].data[batch][output_num])+' ')
				# print 'shape:',v.data.shape[0],v.data.shape[1]
				# print 'shape:',v.data.shape[0],v.data.shape[1]

for k,v in net.params.items():
	#print 'layer:',k
	#print 'shape:',v[1].data.shape[0]
	print 'bias:',k,v[1].data.shape
	if(os.path.exists(output_dir+'vgg19_'+str(k)+'_bias.txt')):
		os.remove(output_dir+'vgg19_'+str(k)+'_bias.txt')
	f=open(output_dir+'vgg19_'+str(k)+'_bias.txt','a')
	for kernel in range(v[1].data.shape[0]):
		f.write(str(net.params[k][1].data[kernel])+'  ')

for k,v in net.params.items():
	print 'params:',k,v[0].data.shape
	if(os.path.exists(output_dir+'vgg19_'+str(k)+'_param.txt')):
		os.remove(output_dir+'vgg19_'+str(k)+'_param.txt')
	f=open(output_dir+'vgg19_'+str(k)+'_param.txt','a')
	if(v[0].data.ndim!=2):
		for kernel in range(v[0].data.shape[0]):
		 	for input_num in range(v[0].data.shape[1]):
		 		print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
		 		for height in range(v[0].data.shape[2]): 
		 			print >>f,'\n'
		 			for width in range(v[0].data.shape[3]):
		 				f.write(str(net.params[k][0].data[kernel][input_num][height][width])+' ')
		#print 'shape:',v[0].data.shape[0],v[0].data.shape[1],v[0].data.shape[2],v[0].data.shape[3]
	else:
		for neturalUnit_num in range(v[0].data.shape[0]):
			print >>f,'\n\n neturalUnit_num='+str(neturalUnit_num)+'\n'
 			for map_num in range(v[0].data.shape[1]):
 				f.write(str(net.params[k][0].data[neturalUnit_num][map_num])+' ')
		#print 'shape:',v[0].data.shape[0],v[0].data.shape[1]

print("\nDone in %.2f s.\n" % (time.time() - start))#计算时间
##如果只要visualizatyion，可把下面写文件部分注释掉

##*****************************************writing the data&params*************************#
# # image
# print 'image_data:',im.shape

# if(os.path.exists(output_dir+'vgg16_image.txt')):
# 	os.remove(output_dir+'vgg16_image.txt')
# f=open(output_dir+'vgg16_image.txt','a')
# for height in range(360):
# 	print >>f,'\n'+'height='+str(height) +'\n'
# 	for width in range(480):
# 		for chanel in range(3):
# 			f.write(str(im[height][width][chanel])+' ')

# # mean substracted values
# print 'mean-substracted values:', zip(np.load(npyMean).mean(1).mean(1))
# if(os.path.exists(output_dir+'vgg16_mean.txt')):
# 	os.remove(output_dir+'vgg16_mean.txt')
# f=open(output_dir+'vgg16_mean.txt','a')
# f.write('\n\nmean-substracted values::'+str(zip(np.load(npyMean).mean(1).mean(1)))+'\n')

# # data 
# print '\ninput:',net.blobs['data'].data.shape
# if(os.path.exists(output_dir+'vgg16_data.txt')):
# 	os.remove(output_dir+'vgg16_data.txt')
# f=open(output_dir+'vgg16_data.txt','a')
# for batch in range(10):
# 	for channel in range(3):
# 		print >>f,'\n'+'batch='+str(batch)+' '+'channel='+str(channel)+' '+'\n'
# 		for height in range(224):
# 			print >>f,'\n'
# 			for width in range(224):
# 				f.write(str(net.blobs['data'].data[batch][channel][height][width])+' ')


# #conv1_1
# print 'conv1_1:',net.blobs['conv1_1'].data.shape
# if(os.path.exists(output_dir+'vgg16_conv1_1.txt')):
# 	os.remove(output_dir+'vgg16_conv1_1.txt')
# f=open(output_dir+'vgg16_conv1_1.txt','a')
# f.write('\nconv1_1 layer:'+' \n')
# for batch in range(10):
#   	for output_num in range(64):
#   		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
#   		for height in range(224):
#   		 	print >>f,'\n'
#   		 	for width in range(224):
#   				f.write(str(net.blobs['conv1_1'].data[batch][output_num][height][width])+' ')

# print 'conv1_1_bias:',net.params['conv1_1'][1].data.shape
# if(os.path.exists(output_dir+'vgg16_conv1_1_bias.txt')):
# 	os.remove(output_dir+'vgg16_conv1_1_bias.txt')
# f=open(output_dir+'vgg16_conv1_1_bias.txt','a')
# for kernel in range(64):
# 	f.write(str(net.params['conv1_1'][1].data[kernel])+'  ')
# print 'conv1_1_params:',net.params['conv1_1'][0].data.shape
# if(os.path.exists(output_dir+'vgg16_conv1_1_param.txt')):
# 	os.remove(output_dir+'vgg16_conv1_1_param.txt')
# f=open(output_dir+'vgg16_conv1_1_param.txt','a')
# for kernel in range(64):
#  	for input_num in range(3):
#  		print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
#  		for height in range(3): 
#  			print >>f,'\n'
#  			for width in range(3):
#  				f.write(str(net.params['conv1_1'][0].data[kernel][input_num][height][width])+' ')

# #conv1_2
# print 'conv1_2:',net.blobs['conv1_2'].data.shape
# if(os.path.exists(output_dir+'vgg16_conv1_2.txt')):
# 	os.remove(output_dir+'vgg16_conv1_2.txt')
# f=open(output_dir+'vgg16_conv1_2.txt','a')
# f.write('\nconv1_2 layer:'+' \n')
# for batch in range(10):
#   	for output_num in range(64):
#   		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
#   		for height in range(224):
#   		 	print >>f,'\n'
#   		 	for width in range(224):
#   				f.write(str(net.blobs['conv1_2'].data[batch][output_num][height][width])+' ')

# print 'conv1_2_bias:',net.params['conv1_2'][1].data.shape
# if(os.path.exists(output_dir+'vgg16_conv1_2_bias.txt')):
# 	os.remove(output_dir+'vgg16_conv1_2_bias.txt')
# f=open(output_dir+'vgg16_conv1_2_bias.txt','a')
# for kernel in range(64):
# 	f.write(str(net.params['conv1_2'][1].data[kernel])+'  ')
# print 'conv1_1_params:',net.params['conv1_2'][0].data.shape
# if(os.path.exists(output_dir+'vgg16_conv1_2_param.txt')):
# 	os.remove(output_dir+'vgg16_conv1_2_param.txt')
# f=open(output_dir+'vgg16_conv1_2_param.txt','a')
# for kernel in range(64):
#  	for input_num in range(3):
#  		print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
#  		for height in range(3): 
#  			print >>f,'\n'
#  			for width in range(3):
#  				f.write(str(net.params['conv1_2'][0].data[kernel][input_num][height][width])+' ')

# print 'pool1:',net.blobs['pool1'].data.shape
# if(os.path.exists(output_dir+'vgg16_pool1.txt')):
# 	os.remove(output_dir+'vgg16_pool1.txt')
# f=open(output_dir+'vgg16_pool1.txt','a')
# f.write('\npool1 layer:'+' \n')
# for batch in range(10):
#   	for output_num in range(64):
#   		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
#   		for height in range(112):
#   		 	print >>f,'\n'
#   		 	for width in range(112):
#   				f.write(str(net.blobs['pool1'].data[batch][output_num][height][width])+' ')

# #conv2_1
# print 'conv2_1:',net.blobs['conv2_1'].data.shape
# if(os.path.exists(output_dir+'vgg16_conv2_1.txt')):
# 	os.remove(output_dir+'vgg16_conv2_1.txt')
# f=open(output_dir+'vgg16_conv2_1.txt','a')
# f.write('\nconv2_1 layer:'+' \n')
# for batch in range(10):
#   	for output_num in range(128):
#   		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
#   		for height in range(112):
#   		 	print >>f,'\n'
#   		 	for width in range(112):
#   				f.write(str(net.blobs['conv2_1'].data[batch][output_num][height][width])+' ')

# print 'conv2_1_bias:',net.params['conv2_1'][1].data.shape
# if(os.path.exists(output_dir+'vgg16_conv2_1_bias.txt')):
# 	os.remove(output_dir+'vgg16_conv2_1_bias.txt')
# f=open(output_dir+'vgg16_conv2_1_bias.txt','a')
# for kernel in range(128):
# 	f.write(str(net.params['conv2_1'][1].data[kernel])+'  ')
# print 'conv2_1_params:',net.params['conv2_1'][0].data.shape
# if(os.path.exists(output_dir+'vgg16_conv2_1_param.txt')):
# 	os.remove(output_dir+'vgg16_conv2_1_param.txt')
# f=open(output_dir+'vgg16_conv2_1_param.txt','a')
# for kernel in range(128):
#  	for input_num in range(64):
#  		print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
#  		for height in range(3):
#  			print >>f,'\n'
#  			for width in range(3):
#  				f.write(str(net.params['conv2_1'][0].data[kernel][input_num][height][width])+' ')

# #conv2_2
# print 'conv2_2:',net.blobs['conv2_2'].data.shape
# if(os.path.exists(output_dir+'vgg16_conv2_2.txt')):
# 	os.remove(output_dir+'vgg16_conv2_2.txt')
# f=open(output_dir+'vgg16_conv2_2.txt','a')
# f.write('\nconv2_2 layer:'+' \n')
# for batch in range(10):
#   	for output_num in range(128):
#   		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
#   		for height in range(112):
#   		 	print >>f,'\n'
#   		 	for width in range(112):
#   				f.write(str(net.blobs['conv2_2'].data[batch][output_num][height][width])+' ')

# print 'conv2_2_bias:',net.params['conv2_2'][1].data.shape
# if(os.path.exists(output_dir+'vgg16_conv2_2_bias.txt')):
# 	os.remove(output_dir+'vgg16_conv2_2_bias.txt')
# f=open(output_dir+'vgg16_conv2_2_bias.txt','a')
# for kernel in range(128):
# 	f.write(str(net.params['conv2_2'][1].data[kernel])+'  ')
# print 'conv2_2_params:',net.params['conv2_2'][0].data.shape
# if(os.path.exists(output_dir+'conv2_2.txt')):
# 	os.remove(output_dir+'vgg16_conv2_2_param.txt')
# f=open(output_dir+'vgg16_conv2_2_param.txt','a')
# for kernel in range(128):
#  	for input_num in range(64):
#  		print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
#  		for height in range(3):
#  			print >>f,'\n'
#  			for width in range(3):
#  				f.write(str(net.params['conv2_2'][0].data[kernel][input_num][height][width])+' ')



# print 'pool2:',net.blobs['pool2'].data.shape
# if(os.path.exists(output_dir+'vgg16_pool2.txt')):
# 	os.remove(output_dir+'vgg16_pool2.txt')
# f=open(output_dir+'vgg16_pool2.txt','a')
# f.write('\npool2 layer:'+' \n')
# for batch in range(10):
#   	for output_num in range(128):
#   		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
#   		for height in range(56):
#   		 	print >>f,'\n'
#   		 	for width in range(56):
#   				f.write(str(net.blobs['pool2'].data[batch][output_num][height][width])+' ')


# #conv3_1
# print 'conv3_1:',net.blobs['conv3_1'].data.shape
# if(os.path.exists(output_dir+'vgg16_conv3_1.txt')):
# 	os.remove(output_dir+'vgg16_conv3_1.txt')
# f=open(output_dir+'vgg16_conv3_1.txt','a')
# f.write('\nconv3_1 layer:'+' \n')
# for batch in range(10):
#   	for output_num in range(256):
#   		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
#   		for height in range(56):
#   		 	print >>f,'\n'
#   		 	for width in range(56):
#   				f.write(str(net.blobs['conv3_1'].data[batch][output_num][height][width])+' ')

# print 'conv3_1_bias:',net.params['conv3_1'][1].data.shape
# if(os.path.exists(output_dir+'vgg16_conv3_1_bias.txt')):
# 	os.remove(output_dir+'vgg16_conv3_1_bias.txt')
# f=open(output_dir+'vgg16_conv3_1_bias.txt','a')
# for kernel in range(256):
# 	f.write(str(net.params['conv3_1'][1].data[kernel])+'  ')
# print 'conv3_1_params:',net.params['conv3_1'][0].data.shape
# if(os.path.exists(output_dir+'vgg16_conv3_1_param.txt')):
# 	os.remove(output_dir+'vgg16_conv3_1_param.txt')
# f=open(output_dir+'vgg16_conv3_1_param.txt','a')
# for kernel in range(256):
#  	for input_num in range(128):
#  		print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
#  		for height in range(3):
#  			print >>f,'\n'
#  			for width in range(3):
#  				f.write(str(net.params['conv3_1'][0].data[kernel][input_num][height][width])+' ')

# #conv3_2
# print 'conv3_2:',net.blobs['conv3_2'].data.shape
# if(os.path.exists(output_dir+'vgg16_conv3_2.txt')):
# 	os.remove(output_dir+'vgg16_conv3_2.txt')
# f=open(output_dir+'vgg16_conv3_2.txt','a')
# f.write('\nconv3_2 layer:'+' \n')
# for batch in range(10):
#   	for output_num in range(256):
#   		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
#   		for height in range(56):
#   		 	print >>f,'\n'
#   		 	for width in range(56):
#   				f.write(str(net.blobs['conv3_2'].data[batch][output_num][height][width])+' ')

# print 'conv3_2_bias:',net.params['conv3_2'][1].data.shape
# if(os.path.exists(output_dir+'vgg16_conv3_2_bias.txt')):
# 	os.remove(output_dir+'vgg16_conv3_2_bias.txt')
# f=open(output_dir+'vgg16_conv3_2_bias.txt','a')
# for kernel in range(256):
# 	f.write(str(net.params['conv3_2'][1].data[kernel])+'  ')
# print 'conv3_2_params:',net.params['conv3_2'][0].data.shape
# if(os.path.exists(output_dir+'conv3_2_param.txt')):
# 	os.remove(output_dir+'vgg16_conv3_2_param.txt')
# f=open(output_dir+'vgg16_conv3_2_param.txt','a')
# for kernel in range(256):
#  	for input_num in range(256):
#  		print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
#  		for height in range(3):
#  			print >>f,'\n'
#  			for width in range(3):
#  				f.write(str(net.params['conv3_2'][0].data[kernel][input_num][height][width])+' ')
# #conv3_3
# print 'conv3_3:',net.blobs['conv3_3'].data.shape
# if(os.path.exists(output_dir+'vgg16_conv3_3.txt')):
# 	os.remove(output_dir+'vgg16_conv3_3.txt')
# f=open(output_dir+'vgg16_conv3_3.txt','a')
# f.write('\nconv3_3 layer:'+' \n')
# for batch in range(10):
#   	for output_num in range(256):
#   		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
#   		for height in range(56):
#   		 	print >>f,'\n'
#   		 	for width in range(56):
#   				f.write(str(net.blobs['conv3_3'].data[batch][output_num][height][width])+' ')

# print 'conv3_3_bias:',net.params['conv3_3'][1].data.shape
# if(os.path.exists(output_dir+'vgg16_conv3_3_bias.txt')):
# 	os.remove(output_dir+'vgg16_conv3_3_bias.txt')
# f=open(output_dir+'vgg16_conv3_3_bias.txt','a')
# for kernel in range(256):
# 	f.write(str(net.params['conv3_3'][1].data[kernel])+'  ')
# print 'conv3_3_params:',net.params['conv3_3'][0].data.shape
# if(os.path.exists(output_dir+'conv3_3_param.txt')):
# 	os.remove(output_dir+'conv3_3_param.txt')
# f=open(output_dir+'conv3_3_param.txt','a')
# for kernel in range(256):
#  	for input_num in range(256):
#  		print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
#  		for height in range(3):
#  			print >>f,'\n'
#  			for width in range(3):
#  				f.write(str(net.params['conv3_3'][0].data[kernel][input_num][height][width])+' ')

# #conv3_4
# print 'conv3_4:',net.blobs['conv3_4'].data.shape
# if(os.path.exists(output_dir+'vgg16_conv3_4.txt')):
#   os.remove(output_dir+'vgg16_conv3_4.txt')
# f=open(output_dir+'vgg16_conv3_4.txt','a')
# f.write('\nconv3_4 layer:'+' \n')
# for batch in range(10):
#     for output_num in range(256):
#       print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
#       for height in range(56):
#         print >>f,'\n'
#         for width in range(56):
#           f.write(str(net.blobs['conv3_4'].data[batch][output_num][height][width])+' ')

# print 'conv3_4_bias:',net.params['conv3_4'][1].data.shape
# if(os.path.exists(output_dir+'vgg16_conv3_4_bias.txt')):
#   os.remove(output_dir+'vgg16_conv3_4_bias.txt')
# f=open(output_dir+'vgg16_conv3_4_bias.txt','a')
# for kernel in range(256):
#   f.write(str(net.params['conv3_4'][1].data[kernel])+'  ')
# print 'conv3_4_params:',net.params['conv3_4'][0].data.shape
# if(os.path.exists(output_dir+'conv3_4_param.txt')):
#   os.remove(output_dir+'conv3_4_param.txt')
# f=open(output_dir+'conv3_4_param.txt','a')
# for kernel in range(256):
#   for input_num in range(256):
#     print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
#     for height in range(3):
#       print >>f,'\n'
#       for width in range(3):
#         f.write(str(net.params['conv3_4'][0].data[kernel][input_num][height][width])+' ')

# print 'pool3:',net.blobs['pool3'].data.shape
# if(os.path.exists(output_dir+'vgg16_pool3.txt')):
# 	os.remove(output_dir+'vgg16_pool3.txt')
# f=open(output_dir+'vgg16_pool3.txt','a')
# f.write('\npool3 layer:'+' \n')
# for batch in range(10):
#   	for output_num in range(256):
#   		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
#   		for height in range(28):
#   		 	print >>f,'\n'
#   		 	for width in range(28):
#   				f.write(str(net.blobs['pool3'].data[batch][output_num][height][width])+' ')

# #conv4_1
# print 'conv4_1:',net.blobs['conv4_1'].data.shape
# if(os.path.exists(output_dir+'vgg16_conv4_1.txt')):
# 	os.remove(output_dir+'vgg16_conv4_1.txt')
# f=open(output_dir+'vgg16_conv4_1.txt','a')
# f.write('\nconv4_1 layer:'+' \n')
# for batch in range(10):
#   	for output_num in range(512):
#   		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
#   		for height in range(28):
#   		 	print >>f,'\n'
#   		 	for width in range(28):
#   				f.write(str(net.blobs['conv4_1'].data[batch][output_num][height][width])+' ')

# print 'conv4_1_bias:',net.params['conv4_1'][1].data.shape
# if(os.path.exists(output_dir+'vgg16_conv4_1_bias.txt')):
# 	os.remove(output_dir+'vgg16_conv4_1_bias.txt')
# f=open(output_dir+'vgg16_conv4_1_bias.txt','a')
# for kernel in range(512):
# 	f.write(str(net.params['conv4_1'][1].data[kernel])+'  ')
# print 'conv4_1_params:',net.params['conv4_1'][0].data.shape
# if(os.path.exists(output_dir+'vgg16_conv4_1_param.txt')):
# 	os.remove(output_dir+'vgg16_conv4_1_param.txt')
# f=open(output_dir+'vgg16_conv4_1_param.txt','a')
# for kernel in range(512):
#  	for input_num in range(256):
#  		print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
#  		for height in range(3):
#  			print >>f,'\n'
#  			for width in range(3):
#  				f.write(str(net.params['conv4_1'][0].data[kernel][input_num][height][width])+' ')


# #conv4_2
# print 'conv4_2:',net.blobs['conv4_2'].data.shape
# if(os.path.exists(output_dir+'vgg16_conv4_2.txt')):
# 	os.remove(output_dir+'vgg16_conv4_2.txt')
# f=open(output_dir+'vgg16_conv4_2.txt','a')
# f.write('\nconv4_2 layer:'+' \n')
# for batch in range(10):
#   	for output_num in range(512):
#   		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
#   		for height in range(28):
#   		 	print >>f,'\n'
#   		 	for width in range(28):
#   				f.write(str(net.blobs['conv4_2'].data[batch][output_num][height][width])+' ')

# print 'conv4_2_bias:',net.params['conv4_2'][1].data.shape
# if(os.path.exists(output_dir+'vgg16_conv4_2_bias.txt')):
# 	os.remove(output_dir+'vgg16_conv4_2_bias.txt')
# f=open(output_dir+'vgg16_conv4_2_bias.txt','a')
# for kernel in range(512):
# 	f.write(str(net.params['conv4_2'][1].data[kernel])+'  ')
# print 'conv4_2_params:',net.params['conv4_2'][0].data.shape
# if(os.path.exists(output_dir+'conv4_2_param.txt')):
# 	os.remove(output_dir+'vgg16_conv4_2_param.txt')
# f=open(output_dir+'vgg16_conv4_2_param.txt','a')
# for kernel in range(512):
#  	for input_num in range(512):
#  		print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
#  		for height in range(3):
#  			print >>f,'\n'
#  			for width in range(3):
#  				f.write(str(net.params['conv4_2'][0].data[kernel][input_num][height][width])+' ')
# #conv4_3
# print 'conv4_3:',net.blobs['conv4_3'].data.shape
# if(os.path.exists(output_dir+'vgg16_conv4_3.txt')):
# 	os.remove(output_dir+'vgg16_conv4_3.txt')
# f=open(output_dir+'vgg16_conv4_3.txt','a')
# f.write('\nconv4_3 layer:'+' \n')
# for batch in range(10):
#   	for output_num in range(512):
#   		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
#   		for height in range(28):
#   		 	print >>f,'\n'
#   		 	for width in range(28):
#   				f.write(str(net.blobs['conv4_3'].data[batch][output_num][height][width])+' ')

# print 'conv4_3_bias:',net.params['conv4_3'][1].data.shape
# if(os.path.exists(output_dir+'vgg16_conv4_3_bias.txt')):
# 	os.remove(output_dir+'vgg16_conv4_3_bias.txt')
# f=open(output_dir+'vgg16_conv4_3_bias.txt','a')
# for kernel in range(512):
# 	f.write(str(net.params['conv4_3'][1].data[kernel])+'  ')
# print 'conv4_3_params:',net.params['conv4_3'][0].data.shape
# if(os.path.exists(output_dir+'conv4_3_param.txt')):
# 	os.remove(output_dir+'conv4_3_param.txt')
# f=open(output_dir+'conv4_3_param.txt','a')
# for kernel in range(512):
#  	for input_num in range(512):
#  		print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
#  		for height in range(3):
#  			print >>f,'\n'
#  			for width in range(3):
#  				f.write(str(net.params['conv4_3'][0].data[kernel][input_num][height][width])+' ')
# #conv4_4
# print 'conv4_4:',net.blobs['conv4_4'].data.shape
# if(os.path.exists(output_dir+'vgg16_conv4_4.txt')):
#   os.remove(output_dir+'vgg16_conv4_4.txt')
# f=open(output_dir+'vgg16_conv4_4.txt','a')
# f.write('\nconv4_4 layer:'+' \n')
# for batch in range(10):
#     for output_num in range(512):
#       print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
#       for height in range(28):
#         print >>f,'\n'
#         for width in range(28):
#           f.write(str(net.blobs['conv4_4'].data[batch][output_num][height][width])+' ')

# print 'conv4_4_bias:',net.params['conv4_4'][1].data.shape
# if(os.path.exists(output_dir+'vgg16_conv4_4_bias.txt')):
#   os.remove(output_dir+'vgg16_conv4_4_bias.txt')
# f=open(output_dir+'vgg16_conv4_4_bias.txt','a')
# for kernel in range(512):
#   f.write(str(net.params['conv4_4'][1].data[kernel])+'  ')
# print 'conv4_4_params:',net.params['conv4_4'][0].data.shape
# if(os.path.exists(output_dir+'conv4_4_param.txt')):
#   os.remove(output_dir+'conv4_4_param.txt')
# f=open(output_dir+'conv4_4_param.txt','a')
# for kernel in range(512):
#   for input_num in range(512):
#     print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
#     for height in range(3):
#       print >>f,'\n'
#       for width in range(3):
#         f.write(str(net.params['conv4_4'][0].data[kernel][input_num][height][width])+' ')


# print 'pool4:',net.blobs['pool4'].data.shape
# if(os.path.exists(output_dir+'vgg16_pool4.txt')):
# 	os.remove(output_dir+'vgg16_pool4.txt')
# f=open(output_dir+'vgg16_pool4.txt','a')
# f.write('\npool4 layer:'+' \n')
# for batch in range(10):
#   	for output_num in range(512):
#   		print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
#   		for height in range(14):
#   		 	print >>f,'\n'
#   		 	for width in range(14):
#   				f.write(str(net.blobs['pool4'].data[batch][output_num][height][width])+' ')

# #conv5_1
# print 'conv5_1:',net.blobs['conv5_1'].data.shape
# if(os.path.exists(output_dir+'vgg16_conv5_1.txt')):
#   os.remove(output_dir+'vgg16_conv5_1.txt')
# f=open(output_dir+'vgg16_conv5_1.txt','a')
# f.write('\nconv5_1 layer:'+' \n')
# for batch in range(10):
#     for output_num in range(512):
#       print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
#       for height in range(14):
#         print >>f,'\n'
#         for width in range(14):
#           f.write(str(net.blobs['conv5_1'].data[batch][output_num][height][width])+' ')

# print 'conv5_1_bias:',net.params['conv5_1'][1].data.shape
# if(os.path.exists(output_dir+'vgg16_conv5_1_bias.txt')):
#   os.remove(output_dir+'vgg16_conv5_1_bias.txt')
# f=open(output_dir+'vgg16_conv5_1_bias.txt','a')
# for kernel in range(512):
#   f.write(str(net.params['conv5_1'][1].data[kernel])+'  ')
# print 'conv5_1_params:',net.params['conv5_1'][0].data.shape
# if(os.path.exists(output_dir+'vgg16_conv5_1_param.txt')):
#   os.remove(output_dir+'vgg16_conv5_1_param.txt')
# f=open(output_dir+'vgg16_conv5_1_param.txt','a')
# for kernel in range(512):
#   for input_num in range(512):
#     print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
#     for height in range(3):
#       print >>f,'\n'
#       for width in range(3):
#         f.write(str(net.params['conv5_1'][0].data[kernel][input_num][height][width])+' ')


# #conv5_2
# print 'conv5_2:',net.blobs['conv5_2'].data.shape
# if(os.path.exists(output_dir+'vgg16_conv5_2.txt')):
#   os.remove(output_dir+'vgg16_conv5_2.txt')
# f=open(output_dir+'vgg16_conv5_2.txt','a')
# f.write('\nconv5_2 layer:'+' \n')
# for batch in range(10):
#     for output_num in range(512):
#       print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
#       for height in range(14):
#         print >>f,'\n'
#         for width in range(14):
#           f.write(str(net.blobs['conv5_2'].data[batch][output_num][height][width])+' ')

# print 'conv5_2_bias:',net.params['conv5_2'][1].data.shape
# if(os.path.exists(output_dir+'vgg16_conv5_2_bias.txt')):
#   os.remove(output_dir+'vgg16_conv5_2_bias.txt')
# f=open(output_dir+'vgg16_conv5_2_bias.txt','a')
# for kernel in range(512):
#   f.write(str(net.params['conv5_2'][1].data[kernel])+'  ')
# print 'conv5_2_params:',net.params['conv5_2'][0].data.shape
# if(os.path.exists(output_dir+'conv5_2_param.txt')):
#   os.remove(output_dir+'vgg16_conv5_2_param.txt')
# f=open(output_dir+'vgg16_conv5_2_param.txt','a')
# for kernel in range(512):
#   for input_num in range(512):
#     print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
#     for height in range(3):
#       print >>f,'\n'
#       for width in range(3):
#         f.write(str(net.params['conv5_2'][0].data[kernel][input_num][height][width])+' ')
# #conv5_3
# print 'conv5_3:',net.blobs['conv5_3'].data.shape
# if(os.path.exists(output_dir+'vgg16_conv5_3.txt')):
#   os.remove(output_dir+'vgg16_conv5_3.txt')
# f=open(output_dir+'vgg16_conv5_3.txt','a')
# f.write('\nconv5_3 layer:'+' \n')
# for batch in range(10):
#     for output_num in range(512):
#       print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
#       for height in range(14):
#         print >>f,'\n'
#         for width in range(14):
#           f.write(str(net.blobs['conv5_3'].data[batch][output_num][height][width])+' ')

# print 'conv5_3_bias:',net.params['conv5_3'][1].data.shape
# if(os.path.exists(output_dir+'vgg16_conv5_3_bias.txt')):
#   os.remove(output_dir+'vgg16_conv5_3_bias.txt')
# f=open(output_dir+'vgg16_conv5_3_bias.txt','a')
# for kernel in range(512):
#   f.write(str(net.params['conv5_3'][1].data[kernel])+'  ')
# print 'conv5_3_params:',net.params['conv5_3'][0].data.shape
# if(os.path.exists(output_dir+'conv5_3_param.txt')):
#   os.remove(output_dir+'conv5_3_param.txt')
# f=open(output_dir+'conv5_3_param.txt','a')
# for kernel in range(512):
#   for input_num in range(512):
#     print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
#     for height in range(3):
#       print >>f,'\n'
#       for width in range(3):
#         f.write(str(net.params['conv5_3'][0].data[kernel][input_num][height][width])+' ')
# #conv5_4
# print 'conv5_4:',net.blobs['conv5_4'].data.shape
# if(os.path.exists(output_dir+'vgg16_conv5_4.txt')):
#   os.remove(output_dir+'vgg16_conv5_4.txt')
# f=open(output_dir+'vgg16_conv5_4.txt','a')
# f.write('\nconv5_4 layer:'+' \n')
# for batch in range(10):
#     for output_num in range(512):
#       print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
#       for height in range(14):
#         print >>f,'\n'
#         for width in range(14):
#           f.write(str(net.blobs['conv5_4'].data[batch][output_num][height][width])+' ')

# print 'conv5_4_bias:',net.params['conv5_4'][1].data.shape
# if(os.path.exists(output_dir+'vgg16_conv5_4_bias.txt')):
#   os.remove(output_dir+'vgg16_conv5_4_bias.txt')
# f=open(output_dir+'vgg16_conv5_4_bias.txt','a')
# for kernel in range(512):
#   f.write(str(net.params['conv5_4'][1].data[kernel])+'  ')
# print 'conv5_4_params:',net.params['conv5_4'][0].data.shape
# if(os.path.exists(output_dir+'conv5_4_param.txt')):
#   os.remove(output_dir+'conv5_4_param.txt')
# f=open(output_dir+'conv5_4_param.txt','a')
# for kernel in range(512):
#   for input_num in range(512):
#     print >>f,'\n\n'+'kernel='+str(kernel)+' '+'input_num='+str(input_num)+' '+'\n'
#     for height in range(3):
#       print >>f,'\n'
#       for width in range(3):
#         f.write(str(net.params['conv5_4'][0].data[kernel][input_num][height][width])+' ')

# print 'pool5:',net.blobs['pool5'].data.shape
# if(os.path.exists(output_dir+'vgg16_pool5.txt')):
#   os.remove(output_dir+'vgg16_pool5.txt')
# f=open(output_dir+'vgg16_pool5.txt','a')
# f.write('\npool5 layer:'+' \n')
# for batch in range(10):
#     for output_num in range(512):
#       print >>f,'\n\n'+'batch='+str(batch)+' '+'output_num='+str(output_num)+' '
#       for height in range(7):
#         print >>f,'\n'
#         for width in range(7):
#           f.write(str(net.blobs['pool5'].data[batch][output_num][height][width])+' ')

# #fc6
# print 'fc6:',net.blobs['fc6'].data.shape

# if(os.path.exists(output_dir+'vgg16_fc6.txt')):
# 	os.remove(output_dir+'vgg16_fc6.txt')
# f=open(output_dir+'vgg16_fc6.txt','a')
# for batch in range(10):
#   	for output_num in range(4096):
#   		f.write(str(net.blobs['fc6'].data[batch][output_num])+' ')


# print 'fc6_bias:',net.params['fc6'][1].data.shape
# if(os.path.exists(output_dir+'vgg16_fc6_bias.txt')):
# 	os.remove(output_dir+'vgg16_fc6_bias.txt')
# f=open(output_dir+'vgg16_fc6_bias.txt','a')
# for neturalUnit_num in range(4096):
#  	f.write(str(net.params['fc6'][1].data[neturalUnit_num])+' ')
# print 'fc6_params:',net.params['fc6'][0].data.shape
# if(os.path.exists(output_dir+'vgg16_fc6_param.txt')):
# 	os.remove(output_dir+'vgg16_fc6_param.txt')
# f=open(output_dir+'vgg16_fc6_param.txt','a')
# for neturalUnit_num in range(4096):
# 	print >>f,'\n\n neturalUnit_num='+str(neturalUnit_num)+'\n'
#  	for map_num in range(25088):
#  		f.write(str(net.params['fc6'][0].data[neturalUnit_num][map_num])+' ')

# #fc7
# print 'fc7:',net.blobs['fc7'].data.shape

# if(os.path.exists(output_dir+'vgg16_fc7.txt')):
# 	os.remove(output_dir+'vgg16_fc7.txt')
# f=open(output_dir+'vgg16_fc7.txt','a')
# for batch in range(10):
#   	for output_num in range(4096):
#   		f.write(str(net.blobs['fc7'].data[batch][output_num])+' ')


# print 'fc7_bias:',net.params['fc7'][1].data.shape
# if(os.path.exists(output_dir+'vgg16_fc7_bias.txt')):
# 	os.remove(output_dir+'vgg16_fc7_bias.txt')
# f=open(output_dir+'vgg16_fc7_bias.txt','a')
# for neturalUnit_num in range(4096):
#  	f.write(str(net.params['fc7'][1].data[neturalUnit_num])+' ')

# print 'fc7_params:',net.params['fc7'][0].data.shape
# if(os.path.exists(output_dir+'vgg16_fc7_param.txt')):
# 	os.remove(output_dir+'vgg16_fc7_param.txt')
# f=open(output_dir+'vgg16_fc7_param.txt','a')
# for neturalUnit_num in range(4096):
# 	print >>f,'\n\n neturalUnit_num='+str(neturalUnit_num)+'\n'
#  	for map_num in range(4096):
#  		f.write(str(net.params['fc7'][0].data[neturalUnit_num][map_num])+' ')

# #fc8
# print 'fc8:',net.blobs['fc8'].data.shape

# if(os.path.exists(output_dir+'vgg16_fc8.txt')):
# 	os.remove(output_dir+'vgg16_fc8.txt')
# f=open(output_dir+'vgg16_fc8.txt','a')
# for batch in range(10):
#   	for output_num in range(1000):
#   		f.write(str(net.blobs['fc8'].data[batch][output_num])+' ')


# print 'fc8_bias:',net.params['fc8'][1].data.shape
# if(os.path.exists(output_dir+'vgg16_fc8_bias.txt')):
# 	os.remove(output_dir+'vgg16_fc8_bias.txt')
# f=open(output_dir+'vgg16_fc8_bias.txt','a')
# for neturalUnit_num in range(1000):
#  	f.write(str(net.params['fc8'][1].data[neturalUnit_num])+' ')
# print 'fc8_params:',net.params['fc8'][0].data.shape
# if(os.path.exists(output_dir+'vgg16_fc8_param.txt')):
# 	os.remove(output_dir+'vgg16_fc8_param.txt')
# f=open(output_dir+'vgg16_fc8_param.txt','a')
# for neturalUnit_num in range(1000):
# 	print >>f,'\n\n neturalUnit_num='+str(neturalUnit_num)+'\n'
#  	for map_num in range(4096):
#  		f.write(str(net.params['fc8'][0].data[neturalUnit_num][map_num])+' ')



#  # prob
# print 'prob:',net.blobs['prob'].data.shape
# if(os.path.exists(output_dir+'vgg16_prob.txt')):
# 	os.remove(output_dir+'vgg16_prob.txt')
# f=open(output_dir+'vgg16_prob.txt','a')
# f.write('\nprob layer:'+' \n')
# for batch in range(10):
#   	for channel in range(1000):
#   		f.write(str(net.blobs['prob'].data[batch][channel])+' ')



# output_prob = out['prob'][0]

# labels = np.loadtxt(model_dir+labels_filename, str, delimiter='\t')
# top_k = net.blobs['prob'].data[0].flatten().argsort()[::-1]

# # predict 
# print 'predict:'
# if(os.path.exists(output_dir+'vgg16_predict.txt')):
# 	os.remove(output_dir+'vgg16_predict.txt')
# f=open(output_dir+'vgg16_predict.txt','a')
# f.write('\nprob layer:'+' \n')
# #print "top_k,soft of the num: ",top_k

# f.write('\n\npredicted class is:'+str(output_prob.argmax())+' '+str(output_prob[output_prob.argmax()])+'\n')
# f.write('\n\nprobabilities and labels:'+str(zip(output_prob[top_k], labels[top_k]))+'\n')

# # print 'predicted class is:', output_prob.argmax(),output_prob[output_prob.argmax()]
# # print 'output label:', labels[output_prob.argmax()]
# # print 'probabilities and labels:'

# ##*****************************************writing the data&params*************************##


# print("\nDone in %.2f s.\n" % (time.time() - start))#计算时间



# ####*************visualization *****************##
# show_data(net.blobs['pool5'].data[0],"pool5")
# show_data(net.params['conv5_1'][0].data[0],"conv5_1_params")
# show_data(net.blobs['conv5_1'].data[0],"conv5_1")
# show_data(net.params['conv5_2'][0].data[0],"conv5_2_params")
# show_data(net.blobs['conv5_2'].data[0],"conv5_2")
# show_data(net.params['conv5_3'][0].data[0],"conv5_3_params")
# show_data(net.blobs['conv5_3'].data[0],"conv5_3")
# show_data(net.params['conv5_4'][0].data[0],"conv5_4_params")
# show_data(net.blobs['conv5_4'].data[0],"conv5_4")

# show_data(net.blobs['pool4'].data[0],"pool4")
# show_data(net.params['conv4_1'][0].data[0],"conv4_1_params")
# show_data(net.blobs['conv4_1'].data[0],"conv4_1")
# show_data(net.params['conv4_2'][0].data[0],"conv4_2_params")
# show_data(net.blobs['conv4_2'].data[0],"conv4_2")
# show_data(net.params['conv4_3'][0].data[0],"conv4_3_params")
# show_data(net.blobs['conv4_3'].data[0],"conv4_3")
# show_data(net.params['conv4_4'][0].data[0],"conv4_4_params")
# show_data(net.blobs['conv4_4'].data[0],"conv4_4")

# show_data(net.blobs['pool3'].data[0],"pool3")
# show_data(net.params['conv3_1'][0].data[0],"conv3_1_params")
# show_data(net.blobs['conv3_1'].data[0],"conv3_1")
# show_data(net.params['conv3_2'][0].data[0],"conv3_2_params")
# show_data(net.blobs['conv3_2'].data[0],"conv3_2")
# show_data(net.params['conv3_3'][0].data[0],"conv3_3_params")
# show_data(net.blobs['conv3_3'].data[0],"conv3_3")
# show_data(net.params['conv3_4'][0].data[0],"conv3_4_params")
# show_data(net.blobs['conv3_4'].data[0],"conv3_4")

# show_data(net.blobs['pool2'].data[0],"pool2")
# show_data(net.params['conv2_1'][0].data[0],"conv2_1_params")
# show_data(net.blobs['conv2_1'].data[0],"conv2_1")
# show_data(net.params['conv2_2'][0].data[0],"conv2_2_params")
# show_data(net.blobs['conv2_2'].data[0],"conv2_2")

# show_data(net.blobs['pool1'].data[0],"pool1")
# show_data(net.params['conv1_2'][0].data[0],"conv1_2_params")
# show_data(net.blobs['conv1_2'].data[0],"conv1_2")
# show_data(net.params['conv1_1'][0].data.transpose(0,2,3,1).squeeze(),'conv1_1_params')
# show_data(net.blobs['conv1_1'].data[0], "conv1_1")

# show_data(net.blobs['data'].data[0], "data")

# plt.imshow(im)
# plt.axis('off')



# plt.show()
