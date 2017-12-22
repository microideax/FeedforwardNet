import json
import sys
import helping_functions

EOL = "\n"
para = open("parameter3.json", "r")
port_num = int(json.load(para))

def generate(generated_file_name="max_pool_acc_innerpp.h"):
    str1 = "#ifndef _MAX_POOL_ACC_H_" + EOL
    str1 += "#define _MAX_POOL_ACC_H_" + EOL + EOL
    str1 += "#include <iostream>" + EOL
    str1 += "#include <fstream>" + EOL
    str1 += '#include "activation_functions.h"' + EOL + EOL
    str1 += "#if _C_DEBUG_MODE_" + EOL
    str1 += "#include <algorithm>" + EOL
    str1 += "#endif" + EOL + EOL
    str1 += "using namespace std;" + EOL + EOL
    str1 += "template <typename T, typename W, typename G, int Tn, int Tr, int Tc, int S_max, int K_max>" + EOL
    str1 += "class max_pool_acc {" + EOL + EOL
    str1 += "private:" + EOL
    str1 += "	int pool_layer_number;" + EOL + EOL
    str1 += "public:" + EOL
    str1 += "	max_pool_acc() : pool_layer_number(0) {pool_layer_number = 0;};" + EOL + EOL

    str1 += "	////------------------------------C++ debugging functions---------------------------------------////" + EOL
    str1 += "	// Reset output buffer" + EOL
    str1 += "	void out_buf_reset(G buf[][Tr][Tc]){" + EOL
    str1 += "        for(int i = 0; i < Tn; i++){" + EOL
    str1 += "            for(int j = 0; j < Tr; j++){" + EOL
    str1 += "                for(int k = 0; k < Tc; k++){" + EOL
    str1 += "                    buf[i][j][k] = G(0);" + EOL
    str1 += "				}" + EOL
    str1 += "			}" + EOL
    str1 += "		}" + EOL
    str1 += "	}" + EOL

    str1 += "    ////-----------------------------Accelerator Functions---------------------------------------////" + EOL
    str1 += "    // Load input data" + EOL
    str1 += "    void in_buf_load(T buf[][(Tr-1)*S_max + K_max][(Tc-1)*S_max + K_max]"
    for j in range(1,port_num + 1):
        str1 += ",T " + "*in_data_" + str(j)

    str1 += ", int n, int r, int c, int S, int K, int P, int R, int C, int N, int R_IN, int C_IN, int TR, int TC) {" + EOL

    str1 += "        for (int j = r * S - P; j < r * S + TR - P; j++) {" + EOL
    str1 += "            for (int k = c * S - P; k < c * S + TC - P; k++) {" + EOL
    str1 += "#pragma HLS PIPELINE" + EOL
    str1 += "        		 for (int i = 0; i < Tn; i+=" + str(port_num) + "){" + EOL
    str1 += "#pragma HLS UNROLL" + EOL

    for j in range(0,port_num):
        str1 += "                    if ((n + Tn > N && i + n + " + str(j) + " >= N ) || j < 0 || j >= (R_IN - 2 * P) || k < 0 || k >= (C_IN - 2 * P)) {" + EOL
        str1 += "                        buf[i + " + str(j) + "][j - r * S + P][k - c * S + P] = T(0);" + EOL
        str1 += "                    }else {" + EOL
        str1 += "                        buf[i + " + str(j) + "][j - r * S + P][k - c * S + P] = *(in_data_" + str(j+1) + " + (i+n)/" + str(port_num) + " * (R_IN - 2 * P) * (C_IN - 2 * P) + j * (C_IN - 2 * P) + k);" + EOL
        str1 += "                    }" + EOL

    str1 += "				}" + EOL
    str1 += "			}" + EOL
    str1 += "		}" + EOL
    str1 += "	}" + EOL

    str1 += "    // Max pooling computation kernel" + EOL
    str1 += "    void pool_engine(T in_buf[][(Tr-1)*S_max + K_max][(Tc-1)*S_max + K_max], G out_buf[][Tr][Tc], int S, int n, int r, int c, int K, int R, int C, int TR, int TC){" + EOL
    str1 += "        for(int i=0; i<K; i++){" + EOL
    str1 += "            for(int j=0; j<K; j++){" + EOL
    str1 += "                for(int tr=0; tr<Tr&&tr+r<R&&(S * tr + i)<TR; tr++){" + EOL
    str1 += "                    for(int tc=0; tc<Tc&&tc+c<C&&(S * tc + j)<TC; tc++){" + EOL
    str1 += "#pragma HLS PIPELINE" + EOL
    str1 += "                        for(int tn=0; tn<Tn; tn++){" + EOL
    str1 += "#pragma HLS UNROLL" + EOL
    str1 += "                            out_buf[tn][tr][tc] = (i==0&&j==0)?in_buf[tn][S*tr][S*tc]:((out_buf[tn][tr][tc]>in_buf[tn][S*tr+i][S*tc+j])?out_buf[tn][tr][tc]:in_buf[tn][S*tr+i][S*tc+j]);" + EOL
    str1 += "                        }" + EOL
    str1 += "                    }" + EOL
    str1 += "                }" + EOL
    str1 += "            }" + EOL
    str1 += "        }" + EOL
    str1 += "    }" + EOL

    str1 += "    // Ouput out_buf data to output interface" + EOL
    str1 += "    void output_res(G out_buf[][Tr][Tc]"
    for j in range(1,port_num + 1):
        str1 += ",G " + "*out_data_" + str(j)
    str1 += ", int n, int r, int c, int N, int R, int C, bool act){" + EOL
    #str1 += "                if (N < n + Tn && i == N) { break; }" + EOL
    str1 += "        for (int j = r; j < r + Tr && j < R; j++) {" + EOL
    #str1 += "                    if (R < r + Tr && j == R) { break; }" + EOL
    str1 += "            for (int k = c; k < c + Tc && k < C; k++) {" + EOL
    #str1 += "                        if (C < c + Tc && k == C) { break; }" + EOL
    str1 += "                for (int i = 0; i < Tn && i < N - n; i += " + str(port_num) + ") {" + EOL
    str1 += "#pragma HLS PIPELINE" + EOL
    str1 += "                        if (act) {" + EOL
    for j in range(1,port_num + 1):
        str1 += "                        	if (i + " + str(j-1) + " < N - n)" + EOL
        str1 += "                            	*(out_data_" + str(j) + " + ((i+n)/" + str(port_num) + ") * R * C + j * C + k) = relu(out_buf[i + " +\
            str(j-1) + "][j - r][k - c]);" + EOL

    str1 += "                        }" + EOL
    str1 += "                        else {" + EOL
    for j in range(1,port_num + 1):
        str1 += "                        	if (i + " + str(j-1) + " <N)" + EOL
        str1 += "                            	*(out_data_" + str(j) + "+ (i/" + str(port_num) + ") * R * C + j * C + k) = out_buf[i + " +\
            str(j-1) + " - n][j - r][k - c];" + EOL

    str1 += "                        }" + EOL
    str1 += "                    }" + EOL
    str1 += "                }" + EOL
    str1 += "            }" + EOL
    str1 += "    }" + EOL

    str1 += "///////////////////////------------------conv accelerator----------------//////////////////////////" + EOL
    str1 += "    void max_pool_layer_acc(" + EOL
    str1 += "            int R_IN,// input Row" + EOL
    str1 += "            int C_IN,// input column" + EOL
    str1 += "            int N, //input feature number" + EOL
    str1 += "            int K, //input kernel size" + EOL
    str1 += "            int R, // output Row" + EOL
    str1 += "            int C, // output column" + EOL
    str1 += "            int S, // stride size" + EOL
    str1 += "            int P, // padding size" + EOL
    str1 += "            bool act, // activation function bit (1-- with act, 0--without act)" + EOL
    for j in range(1,port_num + 1):
        str1 += "            T *in_data_" + str(j) + "," + " // in_data[N][(R-1)*S + K][(C-1)*S + K] --> [N][(R-1)*S + K - 2*P][(C-1)*S + K - 2*P]" + EOL
    for j in range(1,port_num + 1):
        if j == port_num:
            str1 += "            G *out_data_" + str(j) + "){ // out[M][R][C]" + EOL + EOL
        else:
            str1 += "            G *out_data_" + str(j) + "," + " // out[M][R][C]" + EOL

    str1 += "        /***************local data buffer******************************/" + EOL
    str1 += "        int TR=0;" + EOL
    str1 += "        int TC=0;" + EOL
    str1 += "        T in_buf_1[Tn][(Tr-1)*S_max + K_max][(Tc-1)*S_max + K_max];" + EOL
    str1 += "        T in_buf_0[Tn][(Tr-1)*S_max + K_max][(Tc-1)*S_max + K_max];" + EOL
    str1 += "        G out_buf_1[Tn][Tr][Tc];" + EOL
    str1 += "        G out_buf_0[Tn][Tr][Tc];" + EOL + EOL
    str1 += "        /***************Ptr and buffer initialization******************************/" + EOL
    str1 += "        bool in_buf_0_empty = 1;" + EOL
    str1 += "        bool in_buf_1_empty = 1;" + EOL
    str1 += "        bool out_buf_0_empty = 1;" + EOL
    str1 += "        bool out_buf_1_empty = 1;" + EOL
    str1 += "        int loadbufPtr = 0;" + EOL
    str1 += "        int combufPtr = 0;" + EOL
    str1 += "        int resbufPtr = 0;" + EOL + EOL

    str1 += "#if _HLS_MODE_" + EOL
    str1 += "#pragma HLS ARRAY_PARTITION variable=in_buf_1 complete dim=1" + EOL
    str1 += "#pragma HLS ARRAY_PARTITION variable=in_buf_0 complete dim=1" + EOL
    str1 += "#pragma HLS ARRAY_PARTITION variable=out_buf_1 complete dim=1" + EOL
    str1 += "#pragma HLS ARRAY_PARTITION variable=out_buf_0 complete dim=1" + EOL
    str1 += "#endif" + EOL + EOL
    str1 += "#if _C_DEBUG_MODE_" + EOL
    str1 += "#if _KERNEL_DEBUG_" + EOL
    str1 += '            cout << "Starting max_pool_acc_innerpp layer ...." << endl;' + EOL
    str1 += "            //buffer local data initiallization: must do it in C++ debug!" + EOL
    str1 += "            out_buf_reset(out_buf_1);" + EOL
    str1 += "            out_buf_reset(out_buf_0);" + EOL
    str1 += "#endif" + EOL
    str1 += "#endif" + EOL
    str1 += "        for(int r = 0; r < R; r += Tr){" + EOL
    str1 += "            for(int c = 0; c < C; c += Tc){" + EOL
    str1 += "                TR=((r * S + (Tr - 1) * S + K)>R_IN?(R_IN - r * S):((Tr - 1) * S + K));" + EOL
    str1 += "                TC=((c * S + (Tc - 1) * S + K)>C_IN?(C_IN - c * S):((Tc - 1) * S + K));" + EOL
    str1 += "                    for(int n = 0; n < N; n += 2*Tn){" + EOL
    #str1 += "#if _HLS_MODE_" + EOL
    #str1 += "#pragma HLS DATAFLOW" + EOL
    #str1 += "#endif" + EOL
    str1 += "   //--------------------------Load input B W D in ping-pong manner-------------------------//" + EOL
    str1 += "						while (in_buf_0_empty | in_buf_1_empty) {" + EOL
    str1 += "							if (loadbufPtr == 1) {" + EOL
    str1 += '                    			cout << "loading input buffer 1...." << endl;' + EOL
    str1 += "                        		// load input data" + EOL
    str1 += "                        		in_buf_load(in_buf_1"
    for j in range(1,port_num + 1):
        str1 += ", in_data_" + str(j)
    str1 += ", n+Tn, r, c, S, K, P, R, C, N, R_IN, C_IN, TR, TC);" + EOL
    str1 += "                        		in_buf_1_empty = 0;" + EOL
    str1 += '                        		cout << "buffer 1 full" << endl;' + EOL
    str1 += "                        		loadbufPtr = 0;" + EOL
    str1 += "                        	} else {" + EOL
    str1 += '                    			cout << "loading input buffer 0...." << endl;' + EOL
    str1 += "                        		// load input data" + EOL
    str1 += "                        		in_buf_load(in_buf_0"
    for j in range(1,port_num + 1):
        str1 += ", in_data_" + str(j)
    str1 += ", n, r, c, S, K, P, R, C, N, R_IN, C_IN, TR, TC);" + EOL
    str1 += "                        		in_buf_0_empty = 0;" + EOL
    str1 += '                        		cout << "buffer 0 full" << endl;' + EOL
    str1 += "                        		loadbufPtr = 1;" + EOL
    str1 += "							}" + EOL
    str1 += "                       }" + EOL
    str1 += "                       loadbufPtr = 0;" + EOL
    str1 += "  //------------------------------compute buffered data -----------------------------------//" + EOL
    str1 += "                    	while (!in_buf_0_empty | !in_buf_1_empty) {" + EOL
    str1 += "                    		if (combufPtr == 1) {" + EOL
    str1 += '                    			cout << "computing input buffer 1...." << endl;' + EOL
    str1 += "                        		pool_engine(in_buf_1, out_buf_1, S, n+Tn, r, c, K, R, C, TR, TC);" + EOL
    str1 += "                    			out_buf_1_empty = 0;" + EOL
    str1 += "                    			in_buf_1_empty = 1;" + EOL
    str1 += "                    			combufPtr = 0;" + EOL
    str1 += '                    			cout << "buffer 1 computed" << endl;' + EOL
    str1 += "                    		} else {" + EOL
    str1 += '                    			cout << "computing input buffer 0...." << endl;' + EOL
    str1 += "                        		pool_engine(in_buf_0, out_buf_0, S, n, r, c, K, R, C, TR, TC);" + EOL
    str1 += "                    			out_buf_0_empty = 0;" + EOL
    str1 += "                    			in_buf_0_empty = 1;" + EOL
    str1 += "                    			combufPtr = 1;" + EOL
    str1 += '                    			cout << "buffer 0 computed" << endl;' + EOL
    str1 += "							}" + EOL
    str1 += "                       }" + EOL
    str1 += "                       combufPtr = 0;" + EOL
    str1 += "  //---------------------------transfer output data----------------------------------------//" + EOL
    str1 += "                    	while (!out_buf_0_empty | !out_buf_1_empty) {" + EOL
    str1 += "                    		if (resbufPtr == 1) {" + EOL
    str1 += '                    			cout << "output buffer 1...." << endl;' + EOL
    str1 += "                    			// transfer output data" + EOL
    str1 += "                    			output_res(out_buf_1"
    for j in range(1,port_num + 1):
        str1 += ", out_data_" + str(j)
    str1 += ", n+Tn, r, c, N, R, C, act);" + EOL
    str1 += "                    			out_buf_1_empty = 1;" + EOL
    str1 += "                    			resbufPtr = 0;" + EOL
    str1 += '                    			cout << "buffer 1 res" << endl;' + EOL
    str1 += "                    		} else {" + EOL
    str1 += '                    			cout << "output buffer 0...." << endl;' + EOL
    str1 += "                    			// transfer output data" + EOL
    str1 += "                    			output_res(out_buf_0"
    for j in range(1,port_num + 1):
        str1 += ", out_data_" + str(j)
    str1 += ", n, r, c, N, R, C, act);" + EOL
    str1 += "                    			out_buf_0_empty = 1;" + EOL
    str1 += "                    			resbufPtr = 1;" + EOL
    str1 += '								cout << "buffer 0 res" << endl;' + EOL
    str1 += "							}" + EOL
    str1 += "						}" + EOL
    str1 += "						resbufPtr = 0;" + EOL
    str1 += "					}" + EOL
    str1 += "				}" + EOL
    str1 += "			}" + EOL

    str1 += "#if _C_DEBUG_MODE_" + EOL
    str1 += "#if _KERNEL_DEBUG_" + EOL
    str1 += '            cout << "Finished max_pool_acc_innerpp layer ...." << endl;' + EOL
    str1 += "            ofstream max_pool_out;" + EOL
    str1 += '            max_pool_out.open("max_pool_out_data.txt",ios::app);' + EOL
    str1 += '            max_pool_out <<"max_pool output: "<< endl;' + EOL
    str1 += "            for (int i = 0; i < N/" + str(port_num) + "; i++) {" + EOL
    for j in range(1,port_num + 1):
        str1 += "                for (int j = 0; j < R; j++) {" + EOL
        str1 += "                    for(int k = 0; k < C; k++){" + EOL
        str1 += "                        max_pool_out << *(out_data_" + str(j) + ' + i*R*C + j*C + k) << " ";' + EOL
        str1 += "                    }max_pool_out << endl;" + EOL
        str1 += "                }max_pool_out << endl;" + EOL

    str1 += "            }max_pool_out.close();" + EOL
    str1 += "#endif" + EOL
    str1 += "#endif" + EOL
    str1 += "    }" + EOL
    str1 += "};" + EOL
    str1 += "#endif" + EOL

    with open("../example/test_demo/inference_net/" + generated_file_name, "w") as generated_file:
            generated_file.write(str1)

    return str1

if __name__ == "__main__":
    generate()

