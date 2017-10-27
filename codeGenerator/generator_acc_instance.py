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
	layers_fun_w_bn = layers_w_bn(arr[1])
	layers_fun_fc = layers_fc(arr[1])
	prms, prms_str = helping_functions.extraction(arr)
	nn_channel_size_conv_values = prms[prms_str.index("nn_channel_size_conv")]
	nn_stride_conv_values = prms[prms_str.index("nn_stride_conv")]
	nn_in_data_size_pooling_values = prms[prms_str.index("nn_in_data_size_pooling")]  
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

		if l.lower() == "globalmaxpooling":
			strides[1].append(int(nn_in_data_size_pooling_values[pool_counter]))
			kernels[1].append(int(nn_in_data_size_pooling_values[pool_counter]))
			pool_counter = pool_counter + 1

		if l.lower() == "globalavepooling":
			strides[2].append(int(nn_in_data_size_pooling_values[pool_counter]))
			kernels[2].append(int(nn_in_data_size_pooling_values[pool_counter]))
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
	
	'''params for conv_layer & pool_layer'''
	includes = ["#include \"conv_acc_innerdf.h\"", "#include \"max_pool_acc_innerdf.h\"", "#include \"ave_pool_acc_innerdf.h\""]	
	fn_names = ["conv_layer_new", "max_pool_layer_new", "ave_pool_layer_new"]
	rn_tp = "void"
	arg_t_list = [["int", "int", "int", "int", "int","int", "int", "int", "int", "bool", "data_type_w", "data_type_w", "int", "int", "int", "int"],
		    ["int", "int", "int", "int", "int", "int", "int", "int", "bool"]]
	arg_n_list = [["N", "K", "M", "R_IN", "C_IN", "C_OUT", "R_OUT", "S", "P", "act", "*layer_weights", "*layer_bias", "weight_offset", "bias_offset", "in_offset", "out_offset"], 
		      ["R_in", "C_in", "N", "K", "R", "C", "S", "P", "act"]]
	acc_params = [[], [], []]
	
	'''params for conv_w_bn_layer'''
	includes_w_bn = ["#include \"conv_acc_innerdf_w_bn.h\""]	
	fn_names_w_bn = ["conv_layer_new_w_bn"]
	arg_t_list_w_bn = [["int", "int", "int", "int", "int","int", "int", "int", "int", "bool", "data_type_w", "data_type_w", "data_type_w", "data_type_w", "int", "data_type_w", "data_type_w", "int", "int", "int", "int", "int"]]
	arg_n_list_w_bn = [["N", "K", "M", "R_IN", "C_IN", "C_OUT", "R_OUT", "S", "P", "act", "*layer_weights", "*layer_bias", "*bn_mean", "*bn_denominator", "bn_offset", "*scale_gamma", "*scale_beta", "scale_offset", "weight_offset", "bias_offset", "in_offset", "out_offset"]]
	acc_params_w_bn = [[]]
	
	'''params for resnet fc_layer '''
	includes_fc = ["#include \"conv_acc_innerdf_fc.h\""]	
	fn_names_fc = ["conv_layer_new_fc"]
	arg_t_list_fc = [["int", "int", "int", "int", "int","int", "int", "int", "int", "bool", "data_type_w", "data_type_w", "int", "int", "int", "int"],
		    ["int", "int", "int", "int", "int", "int", "int", "int", "bool"]]
	arg_n_list_fc = [["N", "K", "M", "R_IN", "C_IN", "C_OUT", "R_OUT", "S", "P", "act", "*layer_weights", "*layer_bias", "weight_offset", "bias_offset", "in_offset", "out_offset"], 
		      ["R_in", "C_in", "N", "K", "R", "C", "S", "P", "act"]]

	Tm_1 = helping_functions.prompt("Please enter the Tm of conv_acc: ")
	acc_params[0].append(Tm_1)
	acc_params_w_bn[0].append(Tm_1)
	Tn_1 = helping_functions.prompt("Please enter the Tn of conv_acc: ")
	acc_params[0].append(Tn_1)
	acc_params_w_bn[0].append(Tn_1)
	Tr_1 = helping_functions.prompt("Please enter the Tr of conv_acc: ")
	acc_params[0].append(Tr_1)
	acc_params_w_bn[0].append(Tr_1)
	Tc_1 = helping_functions.prompt("Please enter the Tc of conv_acc: ")
	acc_params[0].append(Tc_1)
	acc_params_w_bn[0].append(Tc_1)
	Tn_2 = helping_functions.prompt("\nPlease enter the Tn of pool_acc: ")
	acc_params[1].append(Tn_2)
	acc_params[2].append(Tn_2)
	Tr_2 = helping_functions.prompt("Please enter the Tr of pool_acc: ")
	acc_params[1].append(Tr_2)
	acc_params[2].append(Tr_2)
	Tc_2 = helping_functions.prompt("Please enter the Tc of pool_acc: ")
	acc_params[1].append(Tc_2)
	acc_params[2].append(Tc_2)

	'''object & function for conv_layer & pool_layer'''
	init_nm = ["conv_acc", "max_pool_acc", "ave_pool_acc"]
	prm = "data_type, data_type_w, data_type_o"
	init_names = ["convAcc1", "maxPoolAcc1", "avePoolAcc1"]
	acc_fn_names = ["conv_layer_acc", "max_pool_layer_acc", "ave_pool_layer_acc"]

	'''object & function for conv_w_bn_layer'''
	init_nm_w_bn = ["conv_acc_w_bn"]
	prm_w_bn = "data_type, data_type_w, data_type_o"
	init_names_w_bn = ["convAcc2"]
	acc_fn_names_w_bn = ["conv_layer_acc_w_bn"]

	'''object & function for resnet fc_layer '''
	init_nm_fc = ["conv_acc_fc"]
	prm_fc = "data_type, data_type_w, data_type_o"
	init_names_fc = ["convAcc3"]
	acc_fn_names_fc = ["conv_layer_acc_fc"]

	for j in range(1,port_num + 1):
		arg_t_list[0].append("data_type")
		arg_t_list[1].append("data_type")
		arg_n_list[0].append("*in_data_" + str(j))
		arg_n_list[1].append("*in_data_" + str(j))
		arg_t_list_w_bn[0].append("data_type")
		arg_n_list_w_bn[0].append("*in_data_" + str(j))
		arg_t_list_fc[0].append("data_type")
		arg_t_list_fc[1].append("data_type")
		arg_n_list_fc[0].append("*in_data_" + str(j))
		arg_n_list_fc[1].append("*in_data_" + str(j))

	for j in range(1,port_num + 1):
		arg_t_list[0].append("data_type_o")
		arg_t_list[1].append("data_type_o")
		arg_n_list[0].append("*out_data_" + str(j))
		arg_n_list[1].append("*out_data_" + str(j))
		arg_t_list_w_bn[0].append("data_type_o")
		arg_n_list_w_bn[0].append("*out_data_" + str(j))
		arg_t_list_fc[0].append("data_type_o")
		arg_t_list_fc[1].append("data_type_o")
		arg_n_list_fc[0].append("*out_data_" + str(j))
		arg_n_list_fc[1].append("*out_data_" + str(j))

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
			if "conv_bias_size" in prms_str:
				for k, arg_nm in enumerate(arg_n_list[j]):
					s += arg_nm.replace("*", "")
					if k != len(arg_n_list[j]) - 1:
						s += ", "
			else:
				del arg_t_list[0][11]
				del arg_t_list[0][12]
				del arg_n_list[0][11]
				del arg_n_list[0][12]
				for k, arg_nm in enumerate(arg_n_list[j]):
					s += arg_nm.replace("*", "")
					if k != len(arg_n_list[j]) - 1:
						s += ", "

			body_str += generate_function(fn_names[i], rn_tp, arg_t_list[j], arg_n_list[j], [init_names[i] + CALL_SYMBOL+ acc_fn_names[i] + PARAMETER_BEGIN + s + PARAMETER_END + EOS])
			
	'''write conv_acc_w_bn if needed'''
	if "nn_batch_norm_size" in prms_str:
		for i, l in enumerate(layers_fun_w_bn):
			if l != 0:
				import_str += includes_w_bn[i] + EOL
				if i > 0:
					j = 1
				else:
					j = 0
				body_str += init_nm_w_bn[i] + CLASS_BEGIN + prm_w_bn + COMMA_SPACE
				body_str += ', '.join(acc_params_w_bn[i])
				body_str += acc_max_stride[i]
				body_str += acc_max_kernel[i]
				body_str += CLASS_END + SPACE + init_names_w_bn[i] + EOS + EOL * 2
				s = ""
				if "conv_bias_size" in prms_str:
					for k, arg_nm in enumerate(arg_n_list_w_bn[j]):
						if j == 0:
							if k == 15:
								s += EOL + "#if _SCALE_" + EOL + SEPARATER
								s += arg_nm.replace("*", "")
							elif k == 18:
								s += EOL + "#endif" + EOL + SEPARATER
								s += arg_nm.replace("*", "")
							else:
								s += arg_nm.replace("*", "") 
						else:
							s += arg_nm.replace("*", "")
						if k != len(arg_n_list_w_bn[j]) - 1:
							s += ", "
				else:
					del arg_t_list_w_bn[0][11]
					del arg_t_list_w_bn[0][18]
					del arg_n_list_w_bn[0][11]
					del arg_n_list_w_bn[0][18]
					for k, arg_nm in enumerate(arg_n_list_w_bn[j]):
						if j == 0:
							if k == 14:
								s += EOL + "#if _SCALE_" + EOL + SEPARATER
								s += arg_nm.replace("*", "")
							elif k == 17:
								s += EOL + "#endif" + EOL + SEPARATER
								s += arg_nm.replace("*", "")
							else:
								s += arg_nm.replace("*", "") 
						else:
							s += arg_nm.replace("*", "")
						if k != len(arg_n_list_w_bn[j]) - 1:
							s += ", "

				if j == 0:
					body_str += generate_function_w_bn(fn_names_w_bn[i], rn_tp, arg_t_list_w_bn[j], arg_n_list_w_bn[j], [init_names_w_bn[i] + CALL_SYMBOL+ acc_fn_names_w_bn[i] + PARAMETER_BEGIN + s + PARAMETER_END + EOS])

	'''write fc layer acc if needed'''
	if "nn_in_number_eltwise_size" in prms_str:
		for i, l in enumerate(layers_fun_fc):
			if l != 0:
				import_str += includes_fc[i] + EOL
				if i > 0:
					j = 1
				else:
					j = 0
				body_str += init_nm_fc[i] + CLASS_BEGIN + prm_fc + COMMA_SPACE
				body_str += ', '.join(acc_params[i])
				body_str += acc_max_stride[i]
				body_str += acc_max_kernel[i]
				body_str += CLASS_END + SPACE + init_names_fc[i] + EOS + EOL * 2
				s = ""
				for k, arg_nm in enumerate(arg_n_list_fc[j]):
					s += arg_nm.replace("*", "")
					if k != len(arg_n_list_fc[j]) - 1:
						s += ", "

				body_str += generate_function(fn_names_fc[i], rn_tp, arg_t_list_fc[j], arg_n_list_fc[j], [init_names_fc[i] + CALL_SYMBOL+ acc_fn_names_fc[i] + PARAMETER_BEGIN + s + PARAMETER_END + EOS])
				
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
		
		if arr[i].lower() == "avepooling" or arr[i].lower() == "globalavepooling":
			if (i != (len(arr) - 1) and arr[i+1].lower() != "relu") or (i == len(arr) - 1):
				layer_exist[2] = 1
			else:
				layer_exist[2] = 1
		if arr[i].lower() == "maxpooling" or arr[i].lower() == "globalmaxpooling":
			if (i != (len(arr) - 1) and arr[i+1].lower() != "relu") or (i == len(arr) - 1):
				layer_exist[1] = 1
			else:
				layer_exist[1] = 1

	return layer_exist

def layers_w_bn(arr):
	'''decide if layers_w_bn exist'''
	layer_exist = [0]
	arr = arr.split()
	for i, a in enumerate(arr):
		if arr[i].lower().startswith("convolution") and arr[i+1].lower().startswith("batchnorm"):
			layer_exist[0] = 1

	return layer_exist

def layers_fc(arr):
	'''decide if layers_fc exist'''
	layer_exist = [0]
	arr = arr.split()
	for i, a in enumerate(arr):
		if arr[i].lower().startswith("innerproduct"):
			layer_exist[0] = 1

	return layer_exist

'''function statement template for conv & pool & resnet fc'''
def generate_function(fn_nm, return_type, arg_types_arr, arg_names_arr, fn_body, prefix=SEPARATER):
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

'''function statement template for conv_w_bn'''
def generate_function_w_bn(fn_nm, return_type, arg_types_arr, arg_names_arr, fn_body, prefix=SEPARATER):
	fn_str = return_type + SPACE + fn_nm + PARAMETER_BEGIN + EOL

	arr = helping_functions.read_params(sys.argv[1])
	prms, prms_str = helping_functions.extraction(arr)
	for i, f in enumerate(arg_types_arr):
		if "conv_bias_size" in prms_str:
			if i == 15:
				fn_str += "#if _SCALE_" + EOL
				fn_str += prefix + arg_types_arr[i] + SPACE + arg_names_arr[i]
			elif i == 18:
				fn_str += "#endif" + EOL
				fn_str += prefix + arg_types_arr[i] + SPACE + arg_names_arr[i]
			else:
				fn_str += prefix + arg_types_arr[i] + SPACE + arg_names_arr[i] 
			if (i != len(arg_types_arr) - 1):
				fn_str += "," + EOL
		else:
			if i == 14:
				fn_str += "#if _SCALE_" + EOL
				fn_str += prefix + arg_types_arr[i] + SPACE + arg_names_arr[i]
			elif i == 17:
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

if __name__ == "__main__":
    generate()

