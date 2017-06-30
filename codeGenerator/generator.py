import json
import sys
import helping_functions

architecture_list = ["import", "head", "body"]
EOL = "\n"
SEPARATER = "   "
SPACE = " "
PARAMETER_BEGIN = "("
PARAMETER_END = ")"
BODY_BEGIN = "{"
BODY_END = "}"
ARRAY_BEGIN = "["
ARRAY_END = "]"
CLASS_BEGIN = "<"
CLASS_END = ">"
COMMA = ","
COMMA_SPACE = ", "
EOS = ";"
CALL_SYMBOL = "."
FOR = "for"
EQUAL = " = "
INCREMENT = "++"
LESS = " < "


def generate(generated_file_name="construct_net.h"):
    paraJS = open("parameter.json", "r")
    json_data = json.load(paraJS)

    arr = helping_functions.read_params(sys.argv[1])   
 
    body_s, count = generate_body(json_data["body"], arr)
    import_s = generate_import(json_data["import"], count)
    header_s = generate_header(json_data["head"], arr)
    pragma_s = generate_pragma(json_data["pragma"])
    end_s = generate_end(json_data["end"])

    function_str = import_s + header_s + pragma_s + body_s + end_s
  


    with open("../example/test_demo/inference_net/" + generated_file_name, "w") as generated_file:
        generated_file.write(function_str)


def generate_body(body_json, arr2, prefix=SEPARATER):
    body_str = EOL*2
    body_str += prefix + "cout << " + "\"starting forward network process...........................\" << endl;" + EOL +\
	        prefix + "cout << \"...........................................................\" << endl;" + EOL*2
    layers_order = []	
    conv_counter = 0
    conv_pool_counter = 0
    conv_lrn_pool_counter = 0
    lrn_counter = 0
    pool_counter = 0
    fc_counter = 0
    
    """extraction of parameters from 'net_config_params.txt' file"""
    prms, prms_str = helping_functions.extraction(arr2)
    
    n = prms[prms_str.index("n")]	
    nn_in_data_size_conv_values = prms[prms_str.index("nn_in_data_size_conv")]		
    nn_channel_size_conv_values = prms[prms_str.index("nn_channel_size_conv")]     
    nn_padding_conv_values = prms[prms_str.index("nn_padding_conv")]   
    nn_stride_conv_values = prms[prms_str.index("nn_stride_conv")]    
    nn_in_number_conv_values = prms[prms_str.index("nn_in_number_conv")]    
    nn_out_number_conv_values = prms[prms_str.index("nn_out_number_conv")]  
    nn_group_conv_values = prms[prms_str.index("nn_group_conv")]  
    nn_local_size_lrn_values = prms[prms_str.index("nn_local_size_lrn")]  
    nn_in_data_size_pooling_values = prms[prms_str.index("nn_in_data_size_pooling")]  
    nn_channel_size_pooling_values = prms[prms_str.index("nn_channel_size_pooling")]  
    nn_padding_pooling_values = prms[prms_str.index("nn_padding_pooling")] 
    nn_stride_pooling_values = prms[prms_str.index("nn_stride_pooling")]  
    nn_in_number_pooling_values = prms[prms_str.index("nn_in_number_pooling")]  
    nn_in_number_fc_values = prms[prms_str.index("nn_in_number_fc")]  
    nn_channel_size_fc_values = prms[prms_str.index("nn_channel_size_fc")]   
    nn_out_number_fc_values = prms[prms_str.index("nn_out_number_fc")]    
    layers_order = prms[prms_str.index("layers_order")]

    function_calls = ""
    w_port = "conv_weight_port"
    b_port = "conv_bias_port"
    fc_w_port = "fc_weight_port"
    fc_b_port = "fc_bias_port"
    out_data1 = "output_temp_1"
    out_data2 = "output_temp_2"
    in_data = "in_data_buf"
    out_data = out_data1
    alpha = "nn_alpha_lrn"
    beta = "nn_beta_lrn"
    act_type = "activation_type"
    shift_w = "shift_weight"
    shift_b = "shift_bias"
    shifts = ""
    conv_weight = 0
    conv_bias = 0
    fc_weight = 0
    fc_bias = 0
    cw = ""
    cb = ""
    
    for i, l in enumerate(layers_order):
	if l.lower().startswith("convolution"):
		body_str += generate_layer_init("conv_layer", [nn_in_data_size_conv_values[conv_counter], nn_channel_size_conv_values[conv_counter], 
                            nn_padding_conv_values[conv_counter], nn_stride_conv_values[conv_counter], nn_in_number_conv_values [conv_counter], 
                            nn_out_number_conv_values[conv_counter], nn_group_conv_values[conv_counter]])
		body_str += " C" + str(conv_counter+1) + EOS + EOL
		last = nn_out_number_conv_values[conv_counter]
		last1 = (int(nn_in_data_size_conv_values[conv_counter]) + int(nn_padding_conv_values[conv_counter]) * 2 -\
                        int(nn_channel_size_conv_values[conv_counter]))/int(nn_stride_conv_values[conv_counter]) + 1;
		shw = ""
		shb = ""
		if conv_counter != 0:
			conv_weight += int(nn_in_number_conv_values[conv_counter-1])*int(nn_in_number_conv_values[conv_counter])/\
			int(nn_group_conv_values[conv_counter-1])*int(nn_channel_size_conv_values[conv_counter-1])*int(nn_channel_size_conv_values[conv_counter-1])
			conv_bias += int(nn_out_number_conv_values[conv_counter-1])
			cw = str(conv_weight)
			cb = str(conv_bias)
			shifts += prefix + "int " + shift_w + "_conv" + str(conv_counter + 1) + EQUAL + cw + EOS + EOL
			shifts += prefix + "int " + shift_b + "_conv" + str(conv_counter + 1) + EQUAL + cb + EOS + EOL
			shw = " + " + shift_w + "_conv" + str(conv_counter + 1)
			shb = " + " + shift_b + "_conv" + str(conv_counter + 1)
		function_calls += generate_function_calls("C", "conv", layers_order[i+1], str(conv_counter + 1), [in_data,
				  w_port + shw,  b_port + shb, out_data])
		conv_counter = conv_counter + 1
		
        elif l.lower() == "lrn":
		body_str += generate_layer_init("lrn_layer", [last, nn_local_size_lrn_values[lrn_counter], str(last1)])
		body_str += " L" + str(lrn_counter+1) + EOS + EOL
		alpha1 = alpha + ARRAY_BEGIN + str(lrn_counter) + ARRAY_END
		beta1 = beta + ARRAY_BEGIN + str(lrn_counter) + ARRAY_END
		function_calls += generate_function_calls("L", "lrn", layers_order[i+1], str(lrn_counter+1), [alpha1, beta1, in_data, out_data])
                lrn_counter = lrn_counter + 1
		
        elif l.lower() == "pooling": 
		body_str += generate_layer_init("pool_layer", [nn_in_data_size_pooling_values[pool_counter], nn_channel_size_pooling_values[pool_counter], 
                            nn_padding_pooling_values[pool_counter], nn_stride_pooling_values[pool_counter], nn_in_number_pooling_values[pool_counter]])
		body_str += " P" + str(pool_counter+1) + EOS + EOL
 		last =  nn_in_number_pooling_values[pool_counter]
		last1 = (int(nn_in_data_size_pooling_values[pool_counter]) + int(nn_padding_pooling_values[pool_counter]) * 2 -\
                        int(nn_channel_size_pooling_values[pool_counter]))/int(nn_stride_pooling_values[pool_counter]) + 1
		function_calls += generate_function_calls("P", "max_pooling", layers_order[i+1], str(pool_counter + 1), [in_data, out_data])
                pool_counter = pool_counter + 1 

        elif l.lower() == "innerproduct" or l.lower() == "inner_product":
		body_str += generate_layer_init("fc_layer", [nn_in_number_fc_values[fc_counter], nn_channel_size_fc_values[fc_counter], 
			    nn_out_number_fc_values[fc_counter]])  
		body_str += " F" + str(fc_counter+1) + EOS + EOL
		last = nn_out_number_fc_values[fc_counter]
		if fc_counter>0:
			fc_weight += int(nn_in_number_fc_values[fc_counter])*int(nn_in_number_fc_values[fc_counter-1])*\
				     int(nn_channel_size_fc_values[fc_counter-1])*int(nn_channel_size_fc_values[fc_counter-1])
			fc_bias += int(nn_out_number_fc_values[fc_counter-1])
		b = False
		if l.lower() == "innerproduct":
			for ll in layers_order[(i+1):]:
				if ll.lower() == "innerproduct":
					b = True
					break

		shw = ""
		shb = ""
		if fc_counter != 0:
			shifts += prefix + "int " + shift_w + "_fc" + str(fc_counter + 1) + EQUAL + str(fc_weight) + EOS + EOL
			shifts += prefix + "int " + shift_b + "_fc" + str(fc_counter + 1) + EQUAL + str(fc_bias) + EOS + EOL
			shw = " + " + shift_w + "_fc" + str(fc_counter + 1)
			shb = " + " + shift_b + "_fc" + str(fc_counter + 1)

		if b == False:
			out_data = "fc_" + n + "_out_buf"

		function_calls += generate_function_calls("F", "fc", layers_order[i+1], str(fc_counter + 1), [in_data, fc_w_port + shw, fc_b_port + shb, out_data])
                fc_counter = fc_counter + 1  	

	if l.lower().startswith("convolution") or l.lower() == "lrn" or l.lower() == "pooling" or (l.lower() == "innerproduct" and b == True):
		if out_data == out_data1:
			
			in_data = out_data1
			out_data = out_data2
		else:
			in_data = out_data2
			out_data = out_data1
		if conv_counter != 1 or lrn_counter != 0 or pool_counter != 0 or fc_counter != 0:
			function_calls += prefix + helping_functions.generate_for_loop("addr", "int", "0", prms[prms_str.index("maximum")] , [out_data + "[addr] = data_type_o(0);"], 1, 					          1)
		
	
 
	if l.lower() == "innerproduct" and b == False:
		function_calls += prefix + helping_functions.generate_for_loop("i", "int", "0", nn_out_number_fc_values[len(nn_out_number_fc_values)-1], 
				  ["fc_" + n + "_out_a[i] = fc_" + n + "_out_buf[i];"], 1, 1)

    counters = [conv_counter, pool_counter, lrn_counter, fc_counter, conv_pool_counter, conv_lrn_pool_counter]
       
    data_type_param_x_y = int(nn_in_data_size_conv_values[0]) + 2 * int(nn_padding_conv_values[0])    

    body_str += EOL*2
    """temp storage space define"""

    body_str += prefix + "data_type_o in_data_buf[" + nn_in_number_conv_values[0] + "*" + nn_in_data_size_conv_values[0] + "*" + nn_in_data_size_conv_values[0] + "]" + EOS + EOL
    body_str += prefix + "data_type_o fc_" + n + "_out_buf[" + str(nn_out_number_fc_values[len(nn_out_number_fc_values)-1]) + "]" + EOS + EOL 

    body_str += EOL * 2

    body_str += prefix + helping_functions.generate_for_loop("addr", "int", "0", prms[prms_str.index("maximum")], ["output_temp_1[addr] = data_type_o(0)" +\
                EOS + EOL + prefix*2 +"output_temp_2[addr] = data_type_o(0)" + EOS], 1, 1)

    body_str += prefix + helping_functions.generate_for_loop("i", "int", "0", nn_in_number_conv_values[0], 
		[helping_functions.generate_for_loop("j", "int", "0", nn_in_data_size_conv_values[0], 
		[helping_functions.generate_for_loop("k", "int", "0", nn_in_data_size_conv_values[0], 
		["in_data_buf[i*" + nn_in_data_size_conv_values[0] + "*" + nn_in_data_size_conv_values[0] +" + " + "j*" +\
		nn_in_data_size_conv_values[0] + "+ k] = in_data_3D[i*" + nn_in_data_size_conv_values[0] + "*" +\
		nn_in_data_size_conv_values[0] + " + j*" + nn_in_data_size_conv_values[0] + " + k];"], 3, 1)], 2, 1)], 1, 1)
    body_str += EOL + shifts + EOL
    body_str += function_calls

    body_str += "cout << \"Finished forward network process ..........................\" << endl;" + EOL +\
	        "cout << \"...........................................................\" << endl;" + EOL
    body_str += BODY_END
    return body_str, counters



def generate_layer_init(name, params, prefix=SEPARATER):
    if name == "lrn_layer":
	types = "data_type_o, "
    else:
        types = "data_type, data_type_w, data_type_o, "
    str1 = prefix + name + CLASS_BEGIN + types
  
    for i, l in enumerate(params):
        if (i+1) == len(params):
		str1 += l
	else:
        	str1 += l + COMMA_SPACE
    str1 += CLASS_END
    return str1

def generate_function_calls(nm1, tp, nm2, count, args, prefix=SEPARATER):
    str1 = prefix + nm1 + count + CALL_SYMBOL + tp
    if nm2.lower() == "relu" or nm1 == "P" or nm1 == "L" or nm1 == "C":
	
	str1 += "_layer_a" + PARAMETER_BEGIN
	if nm1 != "L":
		str1 += "activation_type, "
    else:
	str1 += "_layer_a_no_activation" + PARAMETER_BEGIN
    for i, l in enumerate(args):
	if l != None:
        	if (i+1) == len(args):
			str1 += l
		else:
        		str1 += l + COMMA_SPACE
    str1 += PARAMETER_END + EOS + EOL
    return str1

def generate_header(head_json, arr):
    head_str = EOL*3
    head_str += head_json["return_type"] + SEPARATER
    head_str += head_json["function_name"] + PARAMETER_BEGIN + EOL
	
    """param = open("net_config_params.txt", "r")"""
    
    prms, prms_str = helping_functions.extraction(arr)

    n = prms[prms_str.index("n")]
    nn_in_data_size_values = prms[prms_str.index("nn_in_data_size_conv")]	
    nn_padding_conv = prms[prms_str.index("nn_padding_conv")]	
    nn_in_number_conv = prms[prms_str.index("nn_in_number_conv")]	
    nn_out_number_fc = prms[prms_str.index("nn_out_number_fc")]	
    
    fc_nm = "fc_" + n + "_out_a"

    data_type_param_x_y = int(nn_in_data_size_values[0]) + 2 * int(nn_padding_conv[0])
	
    for s in head_json["intput_parameters"]:
	if s["pName"] == "in_data_3D":
		head_str += SEPARATER + s["pType"] + SEPARATER + s["pName"] + ARRAY_BEGIN + nn_in_number_conv[0] + "*" +\
			    nn_in_data_size_values[0] + "*" + nn_in_data_size_values[0] + ARRAY_END + COMMA + EOL
        elif s["pName"] == "fc_out_a":
        	head_str += SEPARATER + s["pType"] + SEPARATER + fc_nm + ARRAY_BEGIN + str(nn_out_number_fc[len(nn_out_number_fc)-1]) + "*1*1" + ARRAY_END + COMMA + EOL		
        elif s["pName"] == "activation_type":
		head_str += SEPARATER + s["pType"] + SEPARATER + s["pName"] + COMMA + EOL
	elif s["pName"] == "output_temp_1" or s["pName"] == "output_temp_2":
		head_str += SEPARATER + s["pType"] + SEPARATER + s["pName"] + ARRAY_BEGIN + prms[prms_str.index("maximum")] + ARRAY_END + COMMA + EOL
	else:	
        	head_str += SEPARATER + s["pType"] + s["pName"] + COMMA + EOL

    head_str = head_str[0:-2]
    head_str += PARAMETER_END + SEPARATER + BODY_BEGIN
    return head_str


def generate_import(import_json, arr):

    import_str = EOL
    for import_sen in import_json:
        import_str += import_sen + EOL

    if arr[0] != 0:
	import_str += "#include \"conv_layer_one_dim.h\"" + EOL
    if arr[1] != 0:
        import_str += "#include \"pool_layer_one_dim.h\"" + EOL
    if arr[2] != 0:
        import_str += "#include \"lrn_layer_one_dim.h\"" + EOL
    if arr[3] != 0:
        import_str += "#include \"fc_layer_one_dim.h\"" + EOL
    if arr[4] != 0:
        import_str += "#include \"conv_pool_layer_one_dim.h\"" + EOL
    if arr[5] != 0:
        import_str += "#include \"conv_lrn_pool_layer_one_dim.h\"" + EOL   
    return import_str

def generate_end(end_json):

   end_str = EOL
   for e in end_json:
	end_str += e + EOL
   return end_str

def generate_pragma(pragma_json):
   
   pragma_str = EOL*2
   for p in pragma_json:
	pragma_str += p + EOL
   return pragma_str

if __name__ == "__main__":
    generate()
