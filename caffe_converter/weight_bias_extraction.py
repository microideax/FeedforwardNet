#coding=utf-8
import numpy as np
import os,sys,caffe,time,math

matplotlib = 'incline'
model_dir = os.getcwd() +'/'

#**************************system argument*************************#
deploy_filename = sys.argv[1]
caffemodel_filename = sys.argv[2]
output_dir = './'

caffe.set_mode_cpu()
net = caffe.Net(model_dir+deploy_filename,
	model_dir+caffemodel_filename,
	caffe.TEST)

start = time.time()

#*********************************************************#
if(os.path.exists(output_dir)==False):
	os.mkdir(output_dir)
	print 'mkdir:',output_dir
if(os.path.exists(output_dir+'net_weights.txt')):
	os.remove(output_dir+'net_weights.txt')
f1=open(output_dir+'net_weights.txt','a+')
if(os.path.exists(output_dir+'batch_norm_mean.txt')):
	os.remove(output_dir+'batch_norm_mean.txt')
f2=open(output_dir+'batch_norm_mean.txt','a+')
if(os.path.exists(output_dir+'batch_norm_denominator.txt')):
	os.remove(output_dir+'batch_norm_denominator.txt')
f3=open(output_dir+'batch_norm_denominator.txt','a+')
if(os.path.exists(output_dir+'scale_gamma.txt')):
	os.remove(output_dir+'scale_gamma.txt')
f4=open(output_dir+'scale_gamma.txt','a+')
if(os.path.exists(output_dir+'scale_beta.txt')):
	os.remove(output_dir+'scale_beta.txt')
f5=open(output_dir+'scale_beta.txt','a+')
bn_record = 0
scale_record = 0
batch_size = 0
break_bool = False
#'''get net batch size'''
for k,v in net.params.items():
	for i in v :
		if (i.data.ndim==1):
 			if "bn" in k:
 				bn_record += 1
 			if bn_record == 3:
 				break_bool = True
 				for neturalUnit_num in range(i.data.shape[0]):
 					batch_size = i.data[neturalUnit_num]
 				break
 	if break_bool:
 		break

#'''get net params'''
bn_record = 0
for k,v in net.params.items():
	for i in v :
		#'''conv layer weights'''
		if (i.data.ndim!=1 and i.data.ndim!=2):
			print 'layer:',k,i.data.shape,'dim:',i.data.ndim
			print >>f1,'\n'+'weights:'
			for kernel in range(i.data.shape[0]):
				for input_num in range(i.data.shape[1]):
					for height in range(i.data.shape[2]): 
						for width in range(i.data.shape[3]):
							f1.write(str(i.data[kernel][input_num][height][width])+' ')
		#'''fc layer weights'''
		elif (i.data.ndim!=1):
			print 'layer:',k,i.data.shape,'dim:',i.data.ndim
			print >>f1,'\n'+'weights:'
			for neturalUnit_num in range(i.data.shape[0]):
 				for map_num in range(i.data.shape[1]):
 					f1.write(str(i.data[neturalUnit_num][map_num])+' ')
 		#'''bn & scale layer & conv,fc layer bias'''
 		else:
 			print 'layer:',k,i.data.shape,'dim:',i.data.ndim
 			if "bn" not in k and "scale" not in k:
 				print >>f1,'\n'+'bias:'
 				for neturalUnit_num in range(i.data.shape[0]):
 					f1.write(str(i.data[neturalUnit_num])+' ')
 			if "bn" in k:
 				bn_record += 1
 				scale_record = 0
 			if "scale" in k:
 				scale_record += 1
 				bn_record = 0
 			if bn_record == 1:
 				for neturalUnit_num in range(i.data.shape[0]):
 					f2.write(str(i.data[neturalUnit_num])+' ')
 			if bn_record == 2:
 				for neturalUnit_num in range(i.data.shape[0]):
 					f3.write(str(1/math.pow(i.data[neturalUnit_num] / batch_size + 1e-05, 0.5))+' ')

 			if scale_record == 1:
 				for neturalUnit_num in range(i.data.shape[0]):
 					f4.write(str(i.data[neturalUnit_num])+' ')
 			if scale_record == 2 and "scale" in k:#'''except softmax layer'''
 				for neturalUnit_num in range(i.data.shape[0]):
 					f5.write(str(i.data[neturalUnit_num])+' ')

f1.close()
f2.close()
f3.close()
f4.close()
f5.close()

print("\nDone in %.2f s.\n" % (time.time() - start))
