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

def generate(generated_file_name="acc_instance.h"):

	arr = helping_functions.read_params(sys.argv[1])
	layers_fun = layers(arr[1])
	str = "#ifndef _ACC_INSTANCE_H_" + EOL + "#define _ACC_INSTANCE_H_" + EOL * 2
	import_str = ""
	body_str = ""
	conv_layer_new_body = ""
	
	includes = ["#include \"conv_acc_dataflow.h\"", "#include \"conv_acc_break_noact.h\"", "#include \"max_pool_acc.h\"", "#include \"ave_pool_acc.h\"", "#include \"max_pool_acc_noact.h\"", "#include \"ave_pool_acc_noact.h\""]	
	fn_names = ["conv_layer_new", "conv_layer_new_noact", "max_pool_layer_new", "ave_pool_layer_new", "max_pool_layer_new_noact", "ave_pool_layer_new_noact"]
	rn_tp = "void"
	arg_t_list = [["int", "int", "int", "int", "int", "int", "int", "data_type", "data_type_w", "data_type_w", "data_type_o", "int", "int", "int", "int"],
		    ["int", "int", "int", "int", "int", "int", "int", "int", "data_type", "data_type_o"]]
	arg_n_list = [["N", "K", "M", "R", "C", "S", "P", "*in_data", "*layer_weights", "*layer_bias", "*out_data", "weight_offset", "bias_offset", "in_offset", "out_offset"], 
		      ["R_in", "C_in", "N", "K", "R", "C", "S", "P", "*in_data", "*out_data"]]
	acc_params = [["16", "4", "13", "13"], ["32", "3", "32", "32"], ["16", "16", "16"], ["16", "16", "16"], ["16", "16", "16"], ["16", "16", "16"]]
	init_nm = ["conv_acc", "conv_acc_noact", "max_pool_acc", "ave_pool_acc", "max_pool_acc_noact", "ave_pool_acc_noact"]
	prm = "data_type, data_type_w, data_type_o"
	init_names = ["convAcc2", "convAcc1", "maxPoolAcc1", "avePoolAcc1", "maxPoolAccNoact1", "avePoolAccNoact1"]
	acc_fn_names = ["conv_layer_acc", "conv_layer_acc_noact", "max_pool_layer_acc", "ave_pool_layer_acc", "max_pool_layer_acc_noact", "ave_pool_layer_acc_noact"]

	for i, l in enumerate(layers_fun):
		if l != 0:
			import_str += includes[i] + EOL
			if i > 1:
				j = 1
			else:
				j = 0
			body_str += init_nm[i] + CLASS_BEGIN + prm + COMMA_SPACE
			body_str += ', '.join(acc_params[i])
			body_str += CLASS_END + SPACE + init_names[i] + EOS + EOL * 2
			s = ""
			for k, arg_nm in enumerate(arg_n_list[j]):
				s += arg_nm.replace("*", "") 
				if k != len(arg_n_list[j]) - 1:
					s += ", "
			body_str += generate_function(fn_names[i], rn_tp, arg_t_list[j], arg_n_list[j], [init_names[i] + CALL_SYMBOL+ acc_fn_names[i] + PARAMETER_BEGIN + s + PARAMETER_END + EOS])
			
	import_str += "#include \"config.h\"" + EOL * 2


	str += import_str + body_str + EOL*2 + ENDIF

	with open("../example/test_demo/inference_net/" + generated_file_name, "w") as generated_file:
        	generated_file.write(str)


	return str


def layers(arr):
	""""layer_exist = [conv_act, conv_no_act, pool_max_act, pool_ave_act, pool_max_no_act, pool_ave_no_act]"""
	layer_exist = [0, 0, 0, 0, 0, 0]
	arr = arr.split()
	for i, a in enumerate(arr):
		if arr[i].lower().startswith("convolution") or arr[i].lower().startswith("innerproduct"):
			if i != len(arr) and arr[i+1].lower() == "relu":
				layer_exist[0] = 1
			else:
				layer_exist[1] = 1

		
		if arr[i].lower() == "avepooling":
			if (i != (len(arr) - 1) and arr[i+1].lower() != "relu") or (i == len(arr) - 1):
				layer_exist[5] = 1
			else:
				layer_exist[3] = 1
		if arr[i].lower() == "maxpooling":
			if (i != (len(arr) - 1) and arr[i+1].lower() != "relu") or (i == len(arr) - 1):
				layer_exist[4] = 1
			else:
				layer_exist[2] = 1

	return layer_exist

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

if __name__ == "__main__":
    generate()
