import json
import sys
import helping_functions

EOL = "\n"
SPACE = " "
PARAMETER_BEGIN = "("
PARAMETER_END = ")"
BODY_BEGIN = "{"
BODY_END = "}"
SEPARATER = "   "
CLASS_BEGIN = "<"
CLASS_END = ">"
COMMA_SPACE = ", "
EOS = ";"
CALL_SYMBOL = "."
ENDIF = "#endif"
para = open("parameter3.json", "r")
port_num = int(json.load(para))

def generate(generated_file_name="acc_instance.h"):

	arr = helping_functions.read_params(sys.argv[1])
	layers_fun = layers(arr[1])
	prms, prms_str = helping_functions.extraction(arr)
	nn_channel_size_conv_values = prms[prms_str.index("nn_channel_size_conv")]
	nn_stride_conv_values = prms[prms_str.index("nn_stride_conv")]
	nn_channel_size_pooling_values = prms[prms_str.index("nn_channel_size_pooling")]
	nn_stride_pooling_values = prms[prms_str.index("nn_stride_pooling")]
	nn_channel_size_fc_values = prms[prms_str.index("nn_channel_size_fc")]
	layers_order = prms[prms_str.index("layers_order")]
	str1 = "#ifndef _ACC_INSTANCE_H_" + EOL + "#define _ACC_INSTANCE_H_" + EOL * 2
	import_str = ""
	body_str = ""
	conv_layer_new_body = ""
	strides = [[], [], []]
	kernels = [[], [], []]
	acc_max_kernel = [[], [], []]
	acc_max_stride = [[], [], []]
	conv_counter = 0
	pool_counter = 0
	fc_counter = 0

	'''get stride&kernel of each layer'''
	for i, l in enumerate(layers_order):
		if l.lower().startswith("convolution"):
			strides[0].append(int(nn_stride_conv_values[conv_counter]))
			kernels[0].append(int(nn_channel_size_conv_values[conv_counter]))
			conv_counter = conv_counter + 1

		if l.lower() == "maxpooling":
			strides[1].append(int(nn_stride_pooling_values[pool_counter]))
			kernels[1].append(int(nn_channel_size_pooling_values[pool_counter]))
			pool_counter = pool_counter + 1

		if l.lower() == "avepooling":
			strides[2].append(int(nn_stride_pooling_values[pool_counter]))
			kernels[2].append(int(nn_channel_size_pooling_values[pool_counter]))
			pool_counter = pool_counter + 1

		if l.lower() == "innerproduct":
			strides[0].append(int(nn_channel_size_fc_values[fc_counter]))
			kernels[0].append(int(nn_channel_size_fc_values[fc_counter]))
			fc_counter = fc_counter + 1

	'''select the biggest stride&kernel of each kind of layer'''
	for k1 in range(len(kernels)):
		if len(kernels[k1]) != 0:
			acc_max_kernel[k1] = ", " + str(max(kernels[k1]))
   		if len(strides[k1]) != 0:	
   			acc_max_stride[k1] = ", " + str(max(strides[k1]))
	
	includes = ["#include \"conv_acc_innerdf_1.h\"", "#include \"max_pool_acc_innerdf.h\"", "#include \"ave_pool_acc_innerdf.h\""]	
	fn_names = ["conv_layer_new", "max_pool_layer_new", "ave_pool_layer_new"]
	rn_tp = "void"
	arg_t_list = [["int", "int", "int", "int", "int","int", "int", "int", "int", "bool", "data_type_w", "data_type_w", "data_type_w", "data_type_w", "data_type_w", "data_type_w", "int", "int", "int", "int"],
		    ["int", "int", "int", "int", "int", "int", "int", "int", "bool"]]
	arg_n_list = [["N", "K", "M", "R_IN", "C_IN", "C_OUT", "R_OUT", "S", "P", "act", "*layer_weights", "*layer_bias", "*bn_mean", "*bn_denominator", "*scale_gamma", "*scale_beta", "weight_offset", "bias_offset", "in_offset", "out_offset"], 
		      ["R_in", "C_in", "N", "K", "R", "C", "S", "P", "act"]]
	acc_params = [["16", "16", "16", "16"], ["16", "16", "16"], ["16", "16", "16"]]
	init_nm = ["conv_acc", "max_pool_acc", "ave_pool_acc"]
	prm = "data_type, data_type_w, data_type_o"
	init_names = ["convAcc1", "maxPoolAcc1", "avePoolAcc1"]
	acc_fn_names = ["conv_layer_acc", "max_pool_layer_acc", "ave_pool_layer_acc"]

	for j in range(1,port_num + 1):
		arg_t_list[0].append("data_type")
		arg_t_list[1].append("data_type")
		arg_n_list[0].append("*in_data_" + str(j))
		arg_n_list[1].append("*in_data_" + str(j))

	for j in range(1,port_num + 1):
		arg_t_list[0].append("data_type_o")
		arg_t_list[1].append("data_type_o")
		arg_n_list[0].append("*out_data_" + str(j))
		arg_n_list[1].append("*out_data_" + str(j))

	'''write layer acc needed'''
	for i, l in enumerate(layers_fun):
		if l != 0:
			import_str += includes[i] + EOL
			if i > 0:
				j = 1
			else:
				j = 0
			body_str += init_nm[i] + CLASS_BEGIN + prm + COMMA_SPACE
			body_str += ', '.join(acc_params[i])
			body_str += acc_max_stride[i]
			body_str += acc_max_kernel[i]
			body_str += CLASS_END + SPACE + init_names[i] + EOS + EOL * 2
			s = ""
			for k, arg_nm in enumerate(arg_n_list[j]):
				if j == 0:
					if k == 12:
						s += EOL + "#if _BATCH_NORM_" + EOL + SEPARATER
						s += arg_nm.replace("*", "")
					elif k == 14:
						s += EOL + "#endif" + EOL + "#if _SCALE_" + EOL + SEPARATER
						s += arg_nm.replace("*", "")
					elif k == 16:
						s += EOL + "#endif" + EOL + SEPARATER
						s += arg_nm.replace("*", "")
					else:
						s += arg_nm.replace("*", "") 
				else:
					s += arg_nm.replace("*", "")
				if k != len(arg_n_list[j]) - 1:
					s += ", "
			if j == 0:
				body_str += generate_function0(fn_names[i], rn_tp, arg_t_list[j], arg_n_list[j], [init_names[i] + CALL_SYMBOL+ acc_fn_names[i] + PARAMETER_BEGIN + s + PARAMETER_END + EOS])
			else:
				body_str += generate_function1(fn_names[i], rn_tp, arg_t_list[j], arg_n_list[j], [init_names[i] + CALL_SYMBOL+ acc_fn_names[i] + PARAMETER_BEGIN + s + PARAMETER_END + EOS])
			
	import_str += "#include \"config.h\"" + EOL * 2


	str1 += import_str + body_str + EOL*2 + ENDIF

	with open("../example/test_demo/inference_net/" + generated_file_name, "w") as generated_file:
        	generated_file.write(str1)


	return str1


def layers(arr):
	'''decide which kind of layers exist'''
	""""eg:layer_exist = [conv_act, pool_max_act, pool_ave_act]"""
	layer_exist = [0, 0, 0]
	arr = arr.split()
	for i, a in enumerate(arr):
		if arr[i].lower().startswith("convolution") or arr[i].lower().startswith("innerproduct"):
			if i != len(arr) and arr[i+1].lower() == "relu":
				layer_exist[0] = 1
			else:
				layer_exist[0] = 1
		
		if arr[i].lower() == "avepooling":
			if (i != (len(arr) - 1) and arr[i+1].lower() != "relu") or (i == len(arr) - 1):
				layer_exist[2] = 1
			else:
				layer_exist[2] = 1
		if arr[i].lower() == "maxpooling":
			if (i != (len(arr) - 1) and arr[i+1].lower() != "relu") or (i == len(arr) - 1):
				layer_exist[1] = 1
			else:
				layer_exist[1] = 1

	return layer_exist

'''function statement template'''
def generate_function0(fn_nm, return_type, arg_types_arr, arg_names_arr, fn_body, prefix=SEPARATER):
	fn_str = return_type + SPACE + fn_nm + PARAMETER_BEGIN + EOL

	for i, f in enumerate(arg_types_arr):
		if i == 12:
			fn_str += "#if _BATCH_NORM_" + EOL
			fn_str += prefix + arg_types_arr[i] + SPACE + arg_names_arr[i]
		elif i == 14:
			fn_str += "#endif" + EOL + "#if _SCALE_" + EOL
			fn_str += prefix + arg_types_arr[i] + SPACE + arg_names_arr[i]
		elif i == 16:
			fn_str += "#endif" + EOL
			fn_str += prefix + arg_types_arr[i] + SPACE + arg_names_arr[i]
		else:
			fn_str += prefix + arg_types_arr[i] + SPACE + arg_names_arr[i] 
		if (i != len(arg_types_arr) - 1):
			fn_str += "," + EOL
	fn_str += PARAMETER_END + SPACE + BODY_BEGIN + EOL * 2 
	for fb in fn_body:
		fn_str += prefix + fb + EOL
	fn_str += EOL
	fn_str += BODY_END + EOL*2
		
	return fn_str

def generate_function1(fn_nm, return_type, arg_types_arr, arg_names_arr, fn_body, prefix=SEPARATER):
	fn_str = return_type + SPACE + fn_nm + PARAMETER_BEGIN + EOL

	for i, f in enumerate(arg_types_arr):
		fn_str += prefix + arg_types_arr[i] + SPACE + arg_names_arr[i] 
		if (i != len(arg_types_arr) - 1):
			fn_str += "," + EOL
	fn_str += PARAMETER_END + SPACE + BODY_BEGIN + EOL * 2 
	for fb in fn_body:
		fn_str += prefix + fb + EOL
	fn_str += EOL
	fn_str += BODY_END + EOL*2
		
	return fn_str

if __name__ == "__main__":
    generate()

