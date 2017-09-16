import json
import sys
import helping_functions
import math
import generator_ff_test
import re

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
para = open("parameter3.json", "r")
port_num = int(json.load(para))

def generate(generated_file_name="construct_net.h"):
    paraJS = open("parameter.json", "r")
    json_data = json.load(paraJS)

    arr = helping_functions.read_params(sys.argv[1])   
 
    body_s, count, acc_str, wb_arr = generate_body(arr)
    import_s = generate_import(json_data["import"], count)
    header_s = generate_header(json_data["head"], arr)
    pragma_s = generate_pragma(wb_arr)
    end_s = generate_end(json_data["end"])
    
    function_str = import_s + header_s + pragma_s + body_s + end_s
  
    with open("../example/test_demo/inference_net/" + generated_file_name, "w") as generated_file:
        generated_file.write(function_str)
    print(acc_str)

def generate_body(arr2, prefix=SEPARATER):
    body_str = EOL*2
    c_debug = "#if _C_DEBUG_MODE_"
    ker_debug = "#if _KERNEL_DEBUG_"
    end_deb = "#endif"
    
    body_str += c_debug + EOL + ker_debug + EOL 
    body_str += prefix + "cout << " + "\"starting forward network process...........................\" << endl;" + EOL +\
            prefix + "cout << \"...........................................................\" << endl;" + EOL
    body_str += end_deb + EOL + end_deb + EOL*2
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
    nn_in_data_size_fc_values = prms[prms_str.index("nn_in_data_size_fc")]
    nn_in_number_fc_values = prms[prms_str.index("nn_in_number_fc")]  
    nn_channel_size_fc_values = prms[prms_str.index("nn_channel_size_fc")]   
    nn_out_number_fc_values = prms[prms_str.index("nn_out_number_fc")]    
    layers_order = prms[prms_str.index("layers_order")]

    if "nn_padding_fc" in prms_str:
        nn_padding_fc_values = prms[prms_str.index("nn_padding_fc")] 
    else:
        nn_padding_fc_values = ["0"] * len(nn_in_number_fc_values)
    strides = [[], [], []]
    kernels = [[], [], []]
    acc_str = EOL + "Accelerators: " + EOL
    function_calls = ""
    w_port = "conv_weight_port"
    b_port = "conv_bias_port"
    fc_w_port = "fc_weight_port"
    fc_b_port = "fc_bias_port"
    mean = "mean"
    denominator = "denominator"
    gamma = "gamma"
    scale_beta = "beta"

    out_data1 = ""
    out_data2 = ""
    for j in range(1,port_num + 1):
        if j == port_num:
            out_data1 += SPACE + "temp_out_0" + "_" + str(j)
            out_data2 += SPACE + "temp_out_1" + "_" + str(j)
        else:
            out_data1 += SPACE + "temp_out_0" + "_" + str(j) + COMMA
            out_data2 += SPACE + "temp_out_1" + "_" + str(j) + COMMA
    in_data = out_data1
    out_data = out_data2
    
    alpha = "nn_alpha_lrn"
    beta = "nn_beta_lrn"
    act_type = "activation_type"
    shift_w = "shift_weight"
    shift_b = "shift_bias"
    in_shift = "in_shift"
    out_shift = "out_shift"
    shifts = ""
    conv_weight = 0
    conv_bias = 0
    fc_weight = 0
    fc_bias = 0
    fc_weight = 0
    fc_bias = 0
    cw = ""
    cb = ""
    cc = 1
    #clean_count = 1
    act = ""

    '''choose layer object&layer type'''
    '''get stride&kernel of each layer'''
    for i, l in enumerate(layers_order):
        if l.lower().startswith("convolution"):
            last = nn_out_number_conv_values[conv_counter]
            in_size = nn_in_data_size_conv_values[conv_counter]
            last1 = int((int(nn_in_data_size_conv_values[conv_counter]) + int(nn_padding_conv_values[conv_counter]) * 2 -\
                        int(nn_channel_size_conv_values[conv_counter]))/float(nn_stride_conv_values[conv_counter]) + 1);
            fun = "conv_layer_new"
            if layers_order[i+1].lower() == "relu":
                fun = "conv_layer_new"
                act = "1"
                strides[0].append(int(nn_stride_conv_values[conv_counter]))
                kernels[0].append(int(nn_channel_size_conv_values[conv_counter]))
            else:
                act = "0"
                strides[0].append(int(nn_stride_conv_values[conv_counter]))
                kernels[0].append(int(nn_channel_size_conv_values[conv_counter]))

            for k in range(int(nn_group_conv_values[conv_counter])):
                in_shift = "0"
                out_shift = "0"
                cw = str(conv_weight)
                cb = str(conv_bias)

                shifts += prefix + "int " + shift_w + "_conv" + str(cc) + "_" + str(k + 1) + EQUAL + cw + EOS + EOL
                shifts += prefix + "int " + shift_b + "_conv" + str(cc) + "_" + str(k + 1) + EQUAL + cb + EOS + EOL

                if k + 1 == int(nn_group_conv_values[conv_counter]) and k > 0:
                    in_shift = "in_shift_" + str(cc)
                    out_shift = "out_shift_" + str(cc)
                in_n = int(math.ceil(int(nn_in_number_conv_values[conv_counter])/float(nn_group_conv_values[conv_counter])))
                out_n = int(math.ceil(int(nn_out_number_conv_values[conv_counter])/float(nn_group_conv_values[conv_counter])))
                function_calls += generate_function_calls1(fun, [str(in_n), str(nn_channel_size_conv_values[conv_counter]), \
                    str(out_n), str(in_size), str(in_size), str(last1), str(last1), nn_stride_conv_values[conv_counter], nn_padding_conv_values[conv_counter], \
                    act, w_port, b_port, EOL + "#if _BATCH_NORM_" + EOL + SEPARATER + mean, denominator, EOL + "#endif" + \
                    EOL + "#if _SCALE_" + EOL + SEPARATER + gamma, scale_beta, EOL + "#endif" + EOL + SEPARATER + shift_w + "_conv" + str(cc) + "_" + str(k+1),\
                    shift_b + "_conv" + str(cc) + "_" + str(k+1), in_shift, out_shift, in_data, out_data])

                if k + 1 == int(nn_group_conv_values[conv_counter]):
                    if k > 0:
                        in_sh = str(int(nn_in_number_conv_values[conv_counter])/int(nn_group_conv_values[conv_counter])) + "/" + str(port_num) + "*" + str(last1) + "*" + str(last1)
                        out_sh = str(int(nn_out_number_conv_values[conv_counter])/int(nn_group_conv_values[conv_counter])) + "/" + str(port_num) + "*" + str(last1) + "*" + str(last1)
                        shifts += prefix + "int " + in_shift + EQUAL + in_sh + EOS + EOL
                        shifts += prefix + "int " + out_shift + EQUAL + out_sh + EOS + EOL

                    cc = cc + 1
                    shifts += EOL

                conv_weight += int(nn_in_number_conv_values[conv_counter])*int(nn_out_number_conv_values[conv_counter])/\
                    int(nn_group_conv_values[conv_counter])*int(nn_channel_size_conv_values[conv_counter])*int(nn_channel_size_conv_values[conv_counter])/int(nn_group_conv_values[conv_counter])
                conv_bias += int(nn_out_number_conv_values[conv_counter])/int(nn_group_conv_values[conv_counter])
            conv_counter = conv_counter + 1

        elif l.lower() == "lrn":
            body_str += generate_layer_init("lrn_layer", [last, nn_local_size_lrn_values[lrn_counter], str(last1)])
            body_str += " L" + str(lrn_counter+1) + EOS + EOL
            alpha1 = alpha + ARRAY_BEGIN + str(lrn_counter) + ARRAY_END
            beta1 = beta + ARRAY_BEGIN + str(lrn_counter) + ARRAY_END
            function_calls += generate_function_calls("L", "lrn", layers_order[i+1], str(lrn_counter+1), [alpha1, beta1, in_data, out_data])
            lrn_counter = lrn_counter + 1

        elif l.lower() == "avepooling" or l.lower() == "maxpooling": 
            last =  nn_in_number_pooling_values[pool_counter]

            if layers_order[i+1].lower() == "relu":
                if l.lower() == "maxpooling":
                    fun = "max_pool_layer_new"
                    act = "1"
                    strides[1].append(int(nn_stride_pooling_values[pool_counter]))
                    kernels[1].append(int(nn_channel_size_pooling_values[pool_counter]))
                if l.lower() == "avepooling":
                    fun = "ave_pool_layer_new"
                    act = "1"
                    strides[2].append(int(nn_stride_pooling_values[pool_counter]))
                    kernels[2].append(int(nn_channel_size_pooling_values[pool_counter]))
            else:
                if l.lower() == "maxpooling":
                    fun = "max_pool_layer_new"
                    act = "0"
                    strides[1].append(int(nn_stride_pooling_values[pool_counter]))
                    kernels[1].append(int(nn_channel_size_pooling_values[pool_counter]))
                if l.lower() == "avepooling":
                    fun = "ave_pool_layer_new"
                    act = "0"
                    strides[2].append(int(nn_stride_pooling_values[pool_counter]))
                    kernels[2].append(int(nn_channel_size_pooling_values[pool_counter]))

            last2 = int(math.ceil((int(nn_in_data_size_pooling_values[pool_counter]) + int(nn_padding_pooling_values[pool_counter]) * 2 -\
                            int(nn_channel_size_pooling_values[pool_counter]))/float(nn_stride_pooling_values[pool_counter]) + 1))
            function_calls += generate_function_calls1(fun, [str(last1), str(last1), nn_in_number_pooling_values[pool_counter], \
                nn_channel_size_pooling_values[pool_counter], str(last2), str(last2), nn_stride_pooling_values[pool_counter], \
                nn_padding_pooling_values[pool_counter], act, in_data, out_data])
            last1 = last2
            pool_counter = pool_counter + 1 

        elif l.lower() == "innerproduct" or l.lower() == "inner_product":
            last = nn_out_number_fc_values[fc_counter]
            fun = "conv_layer_new"
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
            if i + 1 != len(layers_order):
                if layers_order[i+1].lower() == "relu":
                    fun = "conv_layer_new"
                    act = "1"
                    strides[0].append(int(nn_channel_size_fc_values[fc_counter]))
                    kernels[0].append(int(nn_channel_size_fc_values[fc_counter]))
                else:
                    act = "0"
                    strides[0].append(int(nn_channel_size_fc_values[fc_counter]))
                    kernels[0].append(int(nn_channel_size_fc_values[fc_counter]))

            shifts += prefix + "int " + shift_w + "_fc" + str(fc_counter + 1) + EQUAL + str(fc_weight) + EOS + EOL
            shifts += prefix + "int " + shift_b + "_fc" + str(fc_counter + 1) + EQUAL + str(fc_bias) + EOS + EOL*2

            function_calls += generate_function_calls1(fun, [nn_in_number_fc_values[fc_counter], \
                nn_channel_size_fc_values[fc_counter], nn_out_number_fc_values[fc_counter], nn_in_data_size_fc_values[fc_counter], \
                nn_in_data_size_fc_values[fc_counter], "1", "1", nn_channel_size_fc_values[fc_counter], nn_padding_fc_values[fc_counter], \
                act, fc_w_port, fc_b_port, EOL + "#if _BATCH_NORM_" + EOL + SEPARATER + mean, denominator, EOL + "#endif" + \
                    EOL + "#if _SCALE_" + EOL + SEPARATER + gamma, scale_beta, EOL + "#endif" + EOL + SEPARATER + shift_w + "_fc" + str(fc_counter + 1), shift_b + "_fc" + str(fc_counter + 1), "0", "0", in_data, out_data])

            fc_counter = fc_counter + 1  	

        '''reset output_temp'''
        if l.lower().startswith("convolution") or l.lower() == "lrn" or l.lower() == "maxpooling" or l.lower() == "avepooling" or (l.lower() == "innerproduct" and b == True):
            if out_data == out_data1:
                in_data = out_data1
                out_data = out_data2
            else:
                in_data = out_data2
                out_data = out_data1

            in_out_reset_list = re.split('[,]', out_data)
            in_out_reset_loop = ""
            for x in range(0,port_num):
                in_out_reset_loop += in_out_reset_list[x] + "[addr] = data_type_o(0);"

            #function_calls += prefix + "clean_" + str(clean_count) + ":" + prefix + helping_functions.generate_for_loop1("addr", "int", "0", int(math.ceil(float(prms[prms_str.index("maximum")])/port_num)) , in_out_reset_loop) + EOL
            #clean_count = clean_count + 1

        if l.lower() == "innerproduct" and b == False:
            in_out_reset_list = re.split('[,]', out_data)
            in_out_reset_loop = ""
            for x in range(0,port_num):
                in_out_reset_loop += "fc_" + n + "_out_a[i+" + str(x) + "] =" + in_out_reset_list[x] + "[i/" + str(port_num) + "];" +EOL + SEPARATER*2
            function_calls += prefix + helping_functions.generate_for_loop("i", "int", "0", str(int(nn_out_number_fc_values[len(nn_out_number_fc_values)-1])), 
                    [in_out_reset_loop], 1, port_num)

    counters = [conv_counter, pool_counter, lrn_counter, fc_counter]
    
    body_str += EOL + shifts + EOL
    
    body_str += function_calls
    body_str += EOL*2 + c_debug + EOL + ker_debug + EOL 
    body_str += prefix + "cout << \"Finished forward network process ..........................\" << endl;" + EOL +\
            prefix + "cout << \"...........................................................\" << endl;" + EOL
    body_str += end_deb + EOL + end_deb + EOL
    body_str += BODY_END + EOL

    '''show the biggest stride&kernel of each kind of layer'''
    layers1 = ["conv_act", "pool_max_act", "pool_ave_act"]
    for k1 in range(len(kernels)):

        if len(kernels[k1]) != 0:
            acc_str += layers1[k1] + " - max_kernel: " + str(max(kernels[k1]))
        if len(strides[k1]) != 0:
            acc_str += ", max_stride: " + str(max(strides[k1])) + EOL
        #else:
        #    acc_str += ", stride: 1" + EOL
    
    
    w_fc_last = fc_weight + int(nn_in_number_fc_values[fc_counter-1])*int(nn_out_number_fc_values[fc_counter-1])*\
            int(nn_channel_size_fc_values[fc_counter-1])*int(nn_channel_size_fc_values[fc_counter-1])
    b_fc_last = fc_bias + int(nn_out_number_fc_values[fc_counter-1])
    
    w_b_arr = [conv_weight, conv_bias, w_fc_last, b_fc_last, nn_out_number_fc_values[len(nn_out_number_fc_values)-1]]
    for i in range(0,2):
        for j in range(1,port_num + 1):
            w_b_arr.append(str(int(math.ceil(float(prms[prms_str.index("maximum")])/port_num))))

    w_b_arr.append(prms[prms_str.index("n")])
    return body_str, counters, acc_str, w_b_arr

'''new other kind of layer object'''
'''eg:lrn_layer'''
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

def generate_function_calls1(fun, args, prefix=SEPARATER):

    fn_str = prefix + fun + PARAMETER_BEGIN + ", ".join(args) + PARAMETER_END + EOS + EOL
    
    return fn_str

def generate_function_calls(nm1, tp, nm2, count, args, prefix=SEPARATER):
    str1 = prefix + nm1 + count + CALL_SYMBOL + tp
    if nm2.lower() == "relu" or nm1 == "P" or nm1 == "L":
        str1 += "_layer_a" + PARAMETER_BEGIN
    for i, l in enumerate(args):
        if l != None:
            if (i+1) == len(args):
                str1 += l
            else:
                str1 += l + COMMA_SPACE
    str1 += PARAMETER_END + EOS + EOL
    return str1

def generate_header(head_json, arr):
    std = "using namespace std;" + EOL*2
    head_str = EOL + std 

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

    '''write actual parameters of inference_net function'''
    for s in head_json["intput_parameters"]:
        if s["pName"] == "in_data_3D":
            head_str += SEPARATER + s["pType"] + SEPARATER + s["pName"] + ARRAY_BEGIN + nn_in_number_conv[0] + "*" +\
                    nn_in_data_size_values[0] + "*" + nn_in_data_size_values[0] + ARRAY_END + COMMA + EOL
        elif s["pName"] == "fc_out_a":
            head_str += SEPARATER + s["pType"] + SEPARATER + fc_nm + ARRAY_BEGIN + str(nn_out_number_fc[len(nn_out_number_fc)-1]) + "*1*1" + ARRAY_END + COMMA + EOL		
        elif s["pName"] == "activation_type":
            head_str += SEPARATER + s["pType"] + SEPARATER + s["pName"] + COMMA + EOL
        #elif s["pName"] == "output_temp_1" or s["pName"] == "output_temp_2":
        #    head_str += SEPARATER + s["pType"] + SEPARATER + s["pName"] + ARRAY_BEGIN + prms[prms_str.index("maximum")] + ARRAY_END + COMMA + EOL
        elif s["pName"] == "#if _BATCH_NORM_" or s["pName"] == "#endif" or s["pName"] == "#if _SCALE_":
            head_str += s["pName"] + EOL
        else:
            head_str += SEPARATER + s["pType"] + s["pName"] + COMMA + EOL

    for i in range(0,2):
        for j in range(1,port_num + 1):
            if i == 1 and j == port_num:
                head_str += SEPARATER + "data_type_o" + SEPARATER + "temp_out_" + str(i) + "_" + str(j) + ARRAY_BEGIN +\
                    str(int(math.ceil(float(prms[prms_str.index("maximum")])/port_num))) + ARRAY_END + PARAMETER_END + COMMA + EOL
            else:
                head_str += SEPARATER + "data_type_o" + SEPARATER + "temp_out_" + str(i) + "_" + str(j) + ARRAY_BEGIN +\
                    str(int(math.ceil(float(prms[prms_str.index("maximum")])/port_num))) + ARRAY_END + COMMA + EOL

    head_str = head_str[0:-2]
    head_str += BODY_BEGIN
    return head_str


def generate_import(import_json, arr):

    import_str = EOL
    for import_sen in import_json:
        import_str += import_sen + EOL

    """if arr[0] != 0:
    import_str += "#include \"conv_layer_one_dim.h\"" + EOL
    if arr[1] != 0:
        import_str += "#include \"pool_layer_one_dim.h\"" + EOL"""
    if arr[2] != 0:
        import_str += "#include \"lrn_layer_one_dim.h\"" + EOL
    #if arr[3] != 0:
    #    import_str += "#include \"fc_layer_one_dim.h\"" + EOL
    """if arr[4] != 0:
        import_str += "#include \"conv_pool_layer_one_dim.h\"" + EOL
    if arr[5] != 0:
        import_str += "#include \"conv_lrn_pool_layer_one_dim.h\"" + EOL """  
    return import_str

def generate_end(end_json):

    end_str = EOL
    for e in end_json:
        end_str += e + EOL
    return end_str

def generate_pragma(wb_arr):
    hls_deb = "#if _HLS_MODE_"
    end_deb = "#endif"
    pr1 = "#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS"
    pr2 = "#pragma HLS INTERFACE s_axilite port=activation_type bundle=CRTL_BUS"
    pr3 = "#pragma HLS INTERFACE m_axi depth="
    pr4 = " port="
    arr = ["conv_weight_port", "conv_bias_port", "fc_weight_port", "fc_bias_port", "fc_" + str(wb_arr[len(wb_arr)-1]) + "_out_a"]
    for i in range(0,2):
        for j in range(1,port_num + 1):
            arr.append("temp_out_" + str(i) + "_" + str(j))

    pragma_str = EOL*2
    pragma_str += hls_deb + EOL + pr1 + EOL + pr2 + EOL
    pragma_str += pr3 + str(50) + pr4 + "in_data_3D" + EOL
    for i, wb in enumerate(wb_arr[:-1]):
        pragma_str += pr3 + str(wb) + pr4 + arr[i] + EOL
    pragma_str += end_deb
    return pragma_str

if __name__ == "__main__":
    generate()
