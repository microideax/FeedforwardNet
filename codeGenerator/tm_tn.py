import helping_functions
import sys
import math

def generate():
	"""Tm * Tn < DSP/ 5"""
	r = [55, 27, 27, 13, 13, 13, 13, 256, 4096, 4096, 1000]
	arr2 = helping_functions.read_params(sys.argv[1])   
	prms, prms_str = helping_functions.extraction(arr2)
	nn_in_number_conv_values1 = prms[prms_str.index("nn_in_number_conv")] 
	nn_in_number_fc_values = prms[prms_str.index("nn_in_number_fc")] 
	nn_out_number_fc_values = prms[prms_str.index("nn_out_number_fc")] 
	nn_channel_size_conv_values = prms[prms_str.index("nn_channel_size_conv")] 
	nn_channel_size_fc_values = prms[prms_str.index("nn_channel_size_fc")] 

	for n in nn_in_number_fc_values:
		nn_in_number_conv_values1.append(n)
	nn_in_number_conv_values1.append(nn_out_number_fc_values[len(nn_out_number_fc_values) - 1])


	
	
	nn_in_number_conv_values = [int(string) for string in nn_in_number_conv_values1]
	nn_channel_size_fc_values = [int(string) for string in nn_in_number_conv_values1]	
	mm= min(nn_in_number_conv_values)
	
	mm1 = max(nn_in_number_conv_values)
	
	DSP = 900
	d = DSP/5
	arr = []
	
	r_c = [55,27,13,13,13,6, 1, 1]
	k = [11,5,3,3,3,6,1,1]
	max_S = 6
	max_K = 11
	Tm_min = 1
	Tn_min = 1
	minimums =[]
	min_cycles = 0
	minimums1 =[]
	in_buf = 0
	out_buf = 0
	w_buf = 0
	l = int(len(nn_in_number_conv_values) - 1)
	for o in range (0, l):						
		min_cycles += r_c[o] * r_c[o] * math.ceil(int(nn_in_number_conv_values[o + 1])/max_S) * math.ceil(int(nn_in_number_conv_values[o])/1) * k[o] * k[o]
	minimums.append([1, max_S])
	minimums1.append(min_cycles)

	
	for Tm in range(1, mm1+1):
		for Tn in range(max_S, Tm):
			if Tm * Tn < d:
				cycles = 0
				for j in range (0, l):
			
					cycles += r_c[j] * r_c[j] * math.ceil(int(nn_in_number_conv_values[j + 1])/Tn) * math.ceil(int(nn_in_number_conv_values[j])/Tm) * k[j] * k[j]
				if cycles < max(minimums1):
					
					min_cycles = cycles
					Tm_min = Tm
					Tn_min = Tn
					if (len(minimums)<5):
						minimums.append([Tm, Tn])
						minimums1.append(min_cycles)
					else:
						max_among_mins = minimums1.index(max(minimums1))
						minimums1.remove(minimums1[max_among_mins])
						minimums.remove(minimums[max_among_mins])
						minimums.append([Tm, Tn])
						minimums1.append(min_cycles)
	
	print(minimums)

	
	Tr_Tc = []
	in_buff_arr = []
	w_buff_arr = []
	out_buff_arr = []
	total_arr = []
	for m in minimums:
		Tr = int(math.sqrt(m[0]*m[1]))
		Tr_Tc.append([Tr, Tr])
		in_buff = ((Tr-1)*max_S + max_K)*((Tr-1)*max_S + max_K)
		in_buff_arr.append(in_buff)
		out_buff = m[0]*m[1]*max_K*max_K
		out_buff_arr.append(out_buff)
		w_buff = Tr*Tr*m[0]
		w_buff_arr.append(w_buff)
		total = in_buff + out_buff + w_buff
		total_arr.append(total)
	print("in_buf")
	print(in_buff_arr)
	print("w_buf")
	print(w_buff_arr)
	print("out_buf")
	print(out_buff_arr)
	print("total")
	print(total_arr)
	return arr, min_cycles
	

if __name__ == "__main__":
    arr, min_cycles = generate()
    
  
