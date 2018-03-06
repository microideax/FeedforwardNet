import json
import sys
import helping_functions

EOL = "\n"
para = open("parameter3.json", "r")
port_num = int(json.load(para))


def generate(generated_file_name="conv_acc_innerpp.h"):
    arr = helping_functions.read_params(sys.argv[1])
    prms, prms_str = helping_functions.extraction(arr)
    str1 = "#ifndef _CONV_ACC_H_" + EOL
    str1 += "#define _CONV_ACC_H_" + EOL + EOL
    str1 += "#include <iostream>" + EOL
    str1 += "#include <fstream>" + EOL
    str1 += '#include "activation_functions.h"' + EOL + EOL
    str1 += "#if _C_DEBUG_MODE_" + EOL
    str1 += "#include <algorithm>" + EOL
    str1 += "#endif" + EOL + EOL
    str1 += "using namespace std;" + EOL + EOL
    str1 += "template <typename T, typename W, typename G, int Tm, int Tn, int Tr, int Tc, int S_max, int K_max, int IBUF_t, int WBUF_t, int OBUF_t>" + EOL
    str1 += "class conv_acc {" + EOL + EOL
    str1 += "private:" + EOL
    str1 += "	int conv_layer_number;" + EOL + EOL
    str1 += "public:" + EOL
    str1 += "	conv_acc() : conv_layer_number(0) {conv_layer_number = 0;};" + EOL + EOL

    str1 += "	////------------------------------C++ debugging functions---------------------------------------////" + EOL
    str1 += "	// Reset output buffer" + EOL
    str1 += "	void out_buf_reset(G buf[][Tr][Tc]){" + EOL
    str1 += "        for(int i = 0; i < Tm; i++){" + EOL
    str1 += "            for(int j = 0; j < Tr; j++){" + EOL
    str1 += "                for(int k = 0; k < Tc; k++){" + EOL
    str1 += "                    buf[i][j][k] = G(0);" + EOL
    str1 += "				}" + EOL
    str1 += "			}" + EOL
    str1 += "		}" + EOL
    str1 += "	}" + EOL

    str1 += "    // Reset weight buffer" + EOL
    str1 += "    void w_buf_reset(int K, W buf[][Tm][K_max][K_max]){" + EOL
    str1 += "        for(int i = 0; i < Tn; i++){" + EOL
    str1 += "            for(int j = 0; j < Tm; j++){" + EOL
    str1 += "                for(int k = 0; k < K; k++){" + EOL
    str1 += "                    for(int l = 0; l < K; l++){" + EOL
    str1 += "                        buf[i][j][k][l] = W(0);" + EOL
    str1 += "                    }" + EOL
    str1 += "				}" + EOL
    str1 += "			}" + EOL
    str1 += "		}" + EOL
    str1 += "	}" + EOL

    if "conv_bias_size" in prms_str:
        str1 += "    // Reset bias buffer" + EOL
        str1 += "    void b_buf_reset(W buf[]){" + EOL
        str1 += "        for(int i = 0; i < Tm; i++){" + EOL
        str1 += "            buf[i]= W(0);" + EOL
        str1 += "		}" + EOL
        str1 += "	}" + EOL

    str1 += "    ////-----------------------------Accelerator Functions---------------------------------------////" + EOL

    if "conv_bias_size" in prms_str:
        str1 += "    // Load bias data" + EOL
        str1 += "    void b_buf_load(W buf[], W *layer_bias, int bias_offset, int m){" + EOL
        str1 += "        for(int i = 0; i < Tm; i++){" + EOL
        # str1 += "#pragma HLS UNROLL" + EOL
        str1 += "            buf[i] = *(layer_bias + bias_offset + i + m);" + EOL
        str1 += "		}" + EOL
        str1 += "	}" + EOL

    str1 += "    // Load input data" + EOL
    #str1 += "    void in_buf_load(T buf[][(Tr-1)*S_max + K_max][(Tc-1)*S_max + K_max]"
    str1 += "    void in_buf_load(T buf[][IBUF_t][IBUF_t]"
    for j in range(1, port_num + 1):
        str1 += ",T " + "*in_data_" + str(j)

    str1 += ", int in_offset, int n, int r, int c, int S, int K, int P, int R_IN, int C_IN, int N) {" + EOL

    str1 += "       for (int j = r * S - P; j < (r + Tr - 1) * S + K - P; j++) {" + EOL
    str1 += "           for (int k = c * S - P; k < (c + Tc - 1) * S + K - P; k++) {" + EOL
    str1 += "#pragma HLS PIPELINE" + EOL
    str1 += "        	for (int i = 0; i < Tn; i+=" + str(port_num) + "){" + EOL
    str1 += "#pragma HLS UNROLL" + EOL
    #	str1 += "#pragma HLS DEPENDENCE variable=buf inter false" + EOL

    for j in range(0, port_num):
        str1 += "                   	if ((n + Tn > N && i + " + str(
            j) + " >= N - n ) || j < 0 || j >= R_IN || k < 0 || k >= C_IN) {" + EOL
        str1 += "                       	buf[i + " + str(j) + "][j - r * S + P][k - c * S + P] = T(0);" + EOL
        str1 += "                   	} else {" + EOL
        str1 += "                       	buf[i + " + str(j) + "][j - r * S + P][k - c * S + P] = *(in_data_" + str(
            j + 1) + " + in_offset + (i + n)/" + str(port_num) + " * R_IN * C_IN + j * C_IN + k);" + EOL
        str1 += "               		}" + EOL

    str1 += "				}" + EOL
    str1 += "			}" + EOL
    str1 += "		}" + EOL
    str1 += "	}" + EOL
    str1 += EOL
    str1 += EOL

    str1 += "    // Load weights to weight buffer" + EOL
    #str1 += "   void w_buf_load(W buf[][Tm][K_max][K_max], W *layer_weights, int weight_offset, int n, int m, int K, int N, int M){" + EOL
    str1 += "   void w_buf_load(W buf[][Tm][WBUF_t][WBUF_t], W *layer_weights, int weight_offset, int n, int m, int K, int N, int M){" + EOL
    str1 += "       for(int k1 = 0; k1 < K; k1++){" + EOL
    str1 += "           for(int k2 = 0; k2 < K; k2++){" + EOL
    str1 += "        	    for(int j = 0; j < Tn && j < N - n; j++){" + EOL
    str1 += "            	    for(int i = 0; i < Tm && i < M-m; i++){" + EOL
    str1 += "#pragma HLS PIPELINE" + EOL
    str1 += "                        buf[j][i][k1][k2] = *(layer_weights + weight_offset + (i+m)*N*K*K + (j+n)*K*K + k1*K + k2);" + EOL
    str1 += "                   }" + EOL
    str1 += "				}" + EOL
    str1 += "			}" + EOL
    str1 += "		}" + EOL
    str1 += "	}" + EOL

    str1 += "    // Convolution computation kernel" + EOL
    if "conv_bias_size" in prms_str:
        #str1 += "    void conv_engine(T in_buf[][(Tr-1)*S_max + K_max][(Tc-1)*S_max + K_max], W w_buf[][Tm][K_max][K_max], W b_buf[], G out_buf[][Tr][Tc], int S, int n, int r, int c, int K, int R_OUT, int C_OUT){" + EOL
        str1 += "    void conv_engine(T in_buf[][IBUF_t][IBUF_t], W w_buf[][Tm][WBUF_t][WBUF_t], W b_buf[], G out_buf[][Tr][Tc], int S, int n, int r, int c, int K, int R_OUT, int C_OUT, int w_offset, int i_offset){" + EOL
    else:
        str1 += "    void conv_engine(T in_buf[][(Tr-1)*S_max + K_max][(Tc-1)*S_max + K_max], W w_buf[][Tm][K_max][K_max], G out_buf[][Tr][Tc], int S, int n, int r, int c, int K, int R_OUT, int C_OUT){" + EOL
    str1 += "        for(int i=0; i<K; i++){" + EOL
    str1 += "            for(int j=0; j<K; j++){" + EOL
    str1 += "                for(int tr=0; tr<Tr; tr++){" + EOL
    str1 += "                    for(int tc=0; tc<Tc; tc++){" + EOL
    str1 += "#pragma HLS PIPELINE" + EOL
    str1 += "                        for(int tm = 0; tm < Tm; tm++){" + EOL
    str1 += "#pragma HLS UNROLL" + EOL
    str1 += "                            for(int tn=0; tn<Tn; tn++){" + EOL
    str1 += "#pragma HLS UNROLL" + EOL
    str1 += "                                if(i==0&&j==0&&tn==0&&n==0)" + EOL
    if "conv_bias_size" in prms_str:
        str1 += "                                    out_buf[tm][tr][tc] = b_buf[tm] + w_buf[tn][tm][i + w_offset][j]*in_buf[tn][S*(tr)+i + i_offset][S*(tc)+j];" + EOL
    else:
        str1 += "                                    out_buf[tm][tr][tc] = w_buf[tn][tm][i][j]*in_buf[tn][S*(tr)+i][S*(tc)+j];" + EOL
    str1 += "                                else" + EOL
    str1 += "                                    out_buf[tm][tr][tc] = out_buf[tm][tr][tc] + w_buf[tn][tm][i + w_offset][j]*in_buf[tn][S*(tr)+i + i_offset][S*(tc)+j];" + EOL
    str1 += "                            }" + EOL
    str1 += "                        }" + EOL
    str1 += "                    }" + EOL
    str1 += "                }" + EOL
    str1 += "            }" + EOL
    str1 += "        }" + EOL
    str1 += "    }" + EOL
    str1 += EOL
    str1 += EOL

    str1 += "    // Ouput out_buf data to output interface" + EOL
    str1 += "    void output_res(G out_buf[][Tr][Tc]"
    for j in range(1, port_num + 1):
        str1 += ",G " + "*out_data_" + str(j)
    str1 += ", int out_offset, int n, int m, int r, int c, int N, int M, int R_OUT, int C_OUT, bool act){" + EOL
    str1 += "        if (n >= N - Tn) {" + EOL
    str1 += "            for (int j = r; j < r + Tr && j < R_OUT; j++) {" + EOL
    # str1 += "                if (C_OUT < c + Tc && k == C_OUT) { break; }" + EOL
    str1 += "                for (int k = c; k < c + Tc && k < C_OUT; k++) {" + EOL
    # str1 += "                    if (R_OUT < r + Tr && j == R_OUT) { break; }" + EOL
    str1 += "                    for (int i = 0; i < Tm && i < M-m; i += " + str(port_num) + ") {" + EOL
    str1 += "#pragma HLS PIPELINE" + EOL
    # str1 += "#pragma HLS UNROLL" + EOL
    # str1 += "                        if (M < m + Tm && i+m == M) { break; }" + EOL
    str1 += "                        if (act) {" + EOL
    for j in range(1, port_num + 1):
        str1 += "                        	if (i + " + str(j - 1) + " < M-m)" + EOL
        str1 += "                            	*(out_data_" + str(j) + " + out_offset + ((i+m)/" + str(
            port_num) + ") * R_OUT * C_OUT + j * C_OUT + k) = relu(out_buf[i + " + \
                str(j - 1) + "][j - r][k - c]);" + EOL

    str1 += "                        }" + EOL
    str1 += "                        else {" + EOL
    for j in range(1, port_num + 1):
        str1 += "                        	if (i + " + str(j - 1) + " < M-m)" + EOL
        str1 += "                            	*(out_data_" + str(j) + " + out_offset + ((i+m)/" + str(
            port_num) + ") * R_OUT * C_OUT + j * C_OUT + k) = out_buf[i + " + \
                str(j - 1) + "][j - r][k - c];" + EOL

    str1 += "                        }" + EOL
    str1 += "                    }" + EOL
    str1 += "                }" + EOL
    str1 += "            }" + EOL
    str1 += "        }" + EOL
    str1 += "    }" + EOL

    str1 += "///////////////////////------------------conv accelerator----------------//////////////////////////" + EOL
    str1 += "#if _LAYER_MODE_" + EOL
    str1 += "    void conv_layer_acc(" + EOL
    str1 += "            int N, //input feature number" + EOL
    str1 += "            int K, //input kernel size" + EOL
    str1 += "            int M, // output feature number" + EOL
    str1 += "            int R_IN, // input Row" + EOL
    str1 += "            int C_IN, // input column" + EOL
    str1 += "            int R_OUT, // output Row" + EOL
    str1 += "            int C_OUT,// output column" + EOL
    str1 += "            int S, // stride size" + EOL
    str1 += "            int P, // padding size" + EOL
    str1 += "            bool act, // activation function bit (1-- with act, 0--without act)" + EOL
    str1 += "            W *layer_weights, //w[M][N][K][K]" + EOL
    if "conv_bias_size" in prms_str:
        str1 += "            W *layer_bias, // b[M]" + EOL
    str1 += "            int weight_offset," + EOL
    if "conv_bias_size" in prms_str:
        str1 += "            int bias_offset," + EOL
    str1 += "            int in_offset," + EOL
    str1 += "            int out_offset," + EOL
    for j in range(1, port_num + 1):
        str1 += "            T *in_data_" + str(
            j) + "," + " // in_data[N][(R-1)*S + K][(C-1)*S + K] --> [N][(R-1)*S + K - 2*P][(C-1)*S + K - 2*P]" + EOL
    for j in range(1, port_num + 1):
        if j == port_num:
            str1 += "            G *out_data_" + str(j) + "){ // out[M][R][C]" + EOL + EOL
        else:
            str1 += "            G *out_data_" + str(j) + "," + " // out[M][R][C]" + EOL

    str1 += "        /***************local data buffer******************************/" + EOL
    str1 += "        T in_buf_1[Tn][(Tr-1)*S_max + K_max][(Tc-1)*S_max + K_max];" + EOL
    str1 += "        T in_buf_0[Tn][(Tr-1)*S_max + K_max][(Tc-1)*S_max + K_max];" + EOL
    str1 += "        W w_buf_1[Tn][Tm][K_max][K_max];" + EOL
    str1 += "        W w_buf_0[Tn][Tm][K_max][K_max];" + EOL
    if "conv_bias_size" in prms_str:
        str1 += "        W b_buf_1[Tm];" + EOL
        str1 += "        W b_buf_0[Tm];" + EOL
    str1 += "        G out_buf_1[Tm][Tr][Tc];" + EOL
    str1 += "        G out_buf_0[Tm][Tr][Tc];" + EOL + EOL
    str1 += "        /***************Ptr and buffer initialization******************************/" + EOL
    str1 += "        bool in_buf_0_empty = 1;" + EOL
    str1 += "        bool in_buf_1_empty = 1;" + EOL
    str1 += "        bool out_buf_0_empty = 1;" + EOL
    str1 += "        bool out_buf_1_empty = 1;" + EOL
    str1 += "        int loadbufPtr = 0;" + EOL
    str1 += "        int combufPtr = 0;" + EOL
    str1 += "        int resbufPtr = 0;" + EOL
    str1 += "        bool last_com = 0;" + EOL
    str1 += "        bool last_load = 0;" + EOL
    str1 += "        bool last_res = 0;" + EOL + EOL

    str1 += "#if _HLS_MODE_" + EOL
    str1 += "#pragma HLS ARRAY_PARTITION variable=in_buf_1 complete dim=1" + EOL
    str1 += "#pragma HLS ARRAY_PARTITION variable=in_buf_0 complete dim=1" + EOL
    str1 += "#pragma HLS ARRAY_PARTITION variable=w_buf_1 complete dim=1" + EOL
    str1 += "#pragma HLS ARRAY_PARTITION variable=w_buf_1 complete dim=2" + EOL
    str1 += "#pragma HLS ARRAY_PARTITION variable=w_buf_0 complete dim=1" + EOL
    str1 += "#pragma HLS ARRAY_PARTITION variable=w_buf_0 complete dim=2" + EOL
    if "conv_bias_size" in prms_str:
        str1 += "#pragma HLS ARRAY_PARTITION variable=b_buf_1 complete dim=1" + EOL
        str1 += "#pragma HLS ARRAY_PARTITION variable=b_buf_0 complete dim=1" + EOL
    str1 += "#pragma HLS ARRAY_PARTITION variable=out_buf_1 complete dim=1" + EOL
    str1 += "#pragma HLS ARRAY_PARTITION variable=out_buf_0 complete dim=1" + EOL
    str1 += "#endif" + EOL + EOL

    str1 += "#if _C_DEBUG_MODE_" + EOL
    str1 += "#if _KERNEL_DEBUG_" + EOL
    str1 += '            cout << "Starting conv_acc_innerpp layer ...." << endl;' + EOL
    str1 += "            //buffer local data initiallization: must do it in C++ debug!" + EOL
    str1 += "            out_buf_reset(out_buf_1);" + EOL
    str1 += "            out_buf_reset(out_buf_0);" + EOL
    if "conv_bias_size" in prms_str:
        str1 += "            b_buf_reset(b_buf_1);" + EOL
        str1 += "            b_buf_reset(b_buf_0);" + EOL
    str1 += "            w_buf_reset(K, w_buf_1);" + EOL
    str1 += "            w_buf_reset(K, w_buf_0);" + EOL
    str1 += "#endif" + EOL
    str1 += "#endif" + EOL
    str1 += "		for(int r = 0; r < R_OUT; r += Tr){" + EOL
    str1 += "			for(int c = 0; c < C_OUT; c += Tc){" + EOL
    str1 += "				for(int m = 0; m < M; m += Tm){" + EOL
    str1 += "					for(int n = 0; n < N; n += 2*Tn){" + EOL
    # str1 += "#if _HLS_MODE_" + EOL
    # str1 += "#pragma HLS DATAFLOW" + EOL
    # str1 += "#endif" + EOL
    str1 += "   //--------------------------Load input B W D in ping-pong manner-------------------------//" + EOL
    str1 += "						while ((in_buf_0_empty | in_buf_1_empty)&& (!last_load)) {" + EOL
    str1 += "							if (loadbufPtr == 1) {" + EOL
    str1 += '                    			cout << "loading input buffer 1...." << endl;' + EOL
    if "conv_bias_size" in prms_str:
        str1 += "                    			//load input bias" + EOL
        str1 += "                        		b_buf_load(b_buf_1, layer_bias, bias_offset, m);" + EOL
    str1 += "                        		// load input data" + EOL
    str1 += "                        		in_buf_load(in_buf_1"
    for j in range(1, port_num + 1):
        str1 += ", in_data_" + str(j)
    str1 += ", in_offset, n+Tn, r, c, S, K, P, R_IN, C_IN, N);" + EOL
    str1 += "                        		// load input weights" + EOL
    str1 += "                        		w_buf_load(w_buf_1, layer_weights, weight_offset, n+Tn, m, K, N, M);" + EOL
    str1 += "                        		in_buf_1_empty = 0;" + EOL
    str1 += '                        		cout << "buffer 1 full" << endl;' + EOL
    str1 += "                        		loadbufPtr = 0;" + EOL
    str1 += "                        		if (n+2*Tn >= N) {last_load = 1;}" + EOL
    str1 += "                        	} else {" + EOL
    str1 += '                    			cout << "loading input buffer 0...." << endl;' + EOL
    if "conv_bias_size" in prms_str:
        str1 += "                    			//load input bias" + EOL
        str1 += "                        		b_buf_load(b_buf_0, layer_bias, bias_offset, m);" + EOL
    str1 += "                        		// load input data" + EOL
    str1 += "                        		in_buf_load(in_buf_0"
    for j in range(1, port_num + 1):
        str1 += ", in_data_" + str(j)
    str1 += ", in_offset, n, r, c, S, K, P, R_IN, C_IN, N);" + EOL
    str1 += "                        		// load input weights" + EOL
    str1 += "                        		w_buf_load(w_buf_0, layer_weights, weight_offset, n, m, K, N, M);" + EOL
    str1 += "                        		in_buf_0_empty = 0;" + EOL
    str1 += '                        		cout << "buffer 0 full" << endl;' + EOL
    str1 += "                        		loadbufPtr = 1;" + EOL
    str1 += "                        		if (n+Tn >= N) {last_load = 1;}" + EOL
    str1 += "							}" + EOL
    str1 += "                       }" + EOL
    str1 += "                       loadbufPtr = 0;" + EOL
    str1 += "                       last_load = 0;" + EOL
    str1 += "   //------------------------------compute buffered data -----------------------------------//" + EOL
    str1 += "                    	while ((!in_buf_0_empty | !in_buf_1_empty)&& (!last_com)) {" + EOL
    str1 += "                    		if (combufPtr == 1) {" + EOL
    str1 += '                    			cout << "computing input buffer 1...." << endl;' + EOL
    str1 += "                    			if(resbufPtr == 1){" + EOL
    if "conv_bias_size" in prms_str:
        str1 += "                        			conv_engine(in_buf_1, w_buf_1, b_buf_1, out_buf_1, S, n+Tn, r, c, K, R_OUT, C_OUT);" + EOL
    else:
        str1 += "                        			conv_engine(in_buf_1, w_buf_1, out_buf_1, S, n+Tn, r, c, K, R_OUT, C_OUT);" + EOL
    str1 += "                    				out_buf_1_empty = 0;" + EOL
    str1 += "                    			}else{" + EOL
    if "conv_bias_size" in prms_str:
        str1 += "                        			conv_engine(in_buf_1, w_buf_1, b_buf_1, out_buf_0, S, n+Tn, r, c, K, R_OUT, C_OUT);" + EOL
    else:
        str1 += "                        			conv_engine(in_buf_1, w_buf_1, out_buf_0, S, n+Tn, r, c, K, R_OUT, C_OUT);" + EOL
    str1 += "                    				out_buf_0_empty = 0;" + EOL
    str1 += "                    			}" + EOL
    str1 += "                    			in_buf_1_empty = 1;" + EOL
    str1 += "                    			combufPtr = 0;" + EOL
    str1 += '                    			cout << "buffer 1 computed" << endl;' + EOL
    str1 += "                    			if (n+2*Tn >= N) {last_com = 1;}" + EOL
    str1 += "                    		} else {" + EOL
    str1 += '                    			cout << "computing input buffer 0...." << endl;' + EOL
    str1 += "                    			if(resbufPtr == 1){" + EOL
    if "conv_bias_size" in prms_str:
        str1 += "                        			conv_engine(in_buf_0, w_buf_0, b_buf_0, out_buf_1, S, n, r, c, K, R_OUT, C_OUT);" + EOL
    else:
        str1 += "                        			conv_engine(in_buf_0, w_buf_0, out_buf_1, S, n, r, c, K, R_OUT, C_OUT);" + EOL
    str1 += "                    				out_buf_1_empty = 0;" + EOL
    str1 += "                    			}else{" + EOL
    if "conv_bias_size" in prms_str:
        str1 += "                        			conv_engine(in_buf_0, w_buf_0, b_buf_0, out_buf_0, S, n, r, c, K, R_OUT, C_OUT);" + EOL
    else:
        str1 += "                        			conv_engine(in_buf_0, w_buf_0, out_buf_0, S, n, r, c, K, R_OUT, C_OUT);" + EOL
    str1 += "                    				out_buf_0_empty = 0;" + EOL
    str1 += "                    			}" + EOL
    str1 += "                    			in_buf_0_empty = 1;" + EOL
    str1 += "                    			combufPtr = 1;" + EOL
    str1 += '                    			cout << "buffer 0 computed" << endl;' + EOL
    str1 += "								if (n+Tn >= N) {last_com = 1;}" + EOL
    str1 += "							}" + EOL
    str1 += "                       }" + EOL
    str1 += "                       combufPtr = 0;" + EOL
    str1 += "                       last_com = 0;" + EOL
    str1 += "   //---------------------------transfer output data----------------------------------------//" + EOL
    str1 += "                    	while ((!out_buf_0_empty | !out_buf_1_empty)&& (!last_res)) {" + EOL
    str1 += "                    		if (resbufPtr == 1) {" + EOL
    str1 += '                    			cout << "output buffer 1...." << endl;' + EOL
    str1 += "                    			// transfer output data" + EOL
    str1 += "                    			if (n+Tn >= N) {" + EOL
    str1 += "                    				last_res = 1;" + EOL
    str1 += "                    				resbufPtr = 0;" + EOL
    str1 += "                    				output_res(out_buf_1"
    for j in range(1, port_num + 1):
        str1 += ", out_data_" + str(j)
    str1 += ", out_offset, n, m, r, c, N, M, R_OUT, C_OUT, act);" + EOL
    str1 += "                    			}else if (n+2*Tn >= N) {" + EOL
    str1 += "                    				last_res = 1;" + EOL
    str1 += "                    				resbufPtr = 0;" + EOL
    str1 += "                    				output_res(out_buf_1"
    for j in range(1, port_num + 1):
        str1 += ", out_data_" + str(j)
    str1 += ", out_offset, n+Tn, m, r, c, N, M, R_OUT, C_OUT, act);" + EOL
    str1 += "                    			}" + EOL
    str1 += "                    			out_buf_1_empty = 1;" + EOL
    str1 += '                    			cout << "buffer 1 res" << endl;' + EOL
    str1 += "                    		} else {" + EOL
    str1 += '                    			cout << "output buffer 0...." << endl;' + EOL
    str1 += "                    			// transfer output data" + EOL
    str1 += "                    			if (n+Tn >= N) {" + EOL
    str1 += "                    				last_res = 1;" + EOL
    str1 += "                    				resbufPtr = 1;" + EOL
    str1 += "                    				output_res(out_buf_0"
    for j in range(1, port_num + 1):
        str1 += ", out_data_" + str(j)
    str1 += ", out_offset, n, m, r, c, N, M, R_OUT, C_OUT, act);" + EOL
    str1 += "                    			}else if (n+2*Tn >= N) {" + EOL
    str1 += "                    				last_res = 1;" + EOL
    str1 += "                    				resbufPtr = 1;" + EOL
    str1 += "                    				output_res(out_buf_0"
    for j in range(1, port_num + 1):
        str1 += ", out_data_" + str(j)
    str1 += ", out_offset, n+Tn, m, r, c, N, M, R_OUT, C_OUT, act);" + EOL
    str1 += "                    			}" + EOL
    str1 += "                    			out_buf_0_empty = 1;" + EOL
    str1 += '								cout << "buffer 0 res" << endl;' + EOL
    str1 += "							}" + EOL
    str1 += "						}" + EOL
    str1 += "						last_res = 0;" + EOL
    str1 += "					}" + EOL
    str1 += "				}" + EOL
    str1 += "			}" + EOL
    str1 += "		}" + EOL

    str1 += "#if _C_DEBUG_MODE_" + EOL
    str1 += "#if _KERNEL_DEBUG_" + EOL
    str1 += '            cout << "Finished conv_acc_innerpp layer ...." << endl;' + EOL
    str1 += "            ofstream conv_out;" + EOL
    str1 += '            conv_out.open("conv_out_data.txt",ios::app);' + EOL
    str1 += '            conv_out <<"conv output: "<< endl;' + EOL
    str1 += "            for (int i = 0; i < M/" + str(port_num) + "; i++) {" + EOL
    for j in range(1, port_num + 1):
        str1 += "                for (int j = 0; j < R_OUT; j++) {" + EOL
        str1 += "                    for(int k = 0; k < C_OUT; k++){" + EOL
        str1 += "                        conv_out << *(out_data_" + str(
            j) + ' + out_offset + i*R_OUT*C_OUT + j*C_OUT + k) << " ";' + EOL
        str1 += "                    }conv_out << endl;" + EOL
        str1 += "                }conv_out << endl;" + EOL

    str1 += "            }conv_out.close();" + EOL
    str1 += "#endif" + EOL
    str1 += "#endif" + EOL
    str1 += "    }" + EOL
    str1 += "#endif" + EOL

    str1 += "    " + EOL
    str1 += "////////-----------------CONV_ACC----------------////////" + EOL
    str1 += "#if _ACC_MODE_" + EOL
    str1 += "   void conv_core_acc( " + EOL
    str1 += "        data_type_w in_buf_0[Tn][IBUF_t][IBUF_t]," + EOL
    str1 += "        data_type_w w_buf_0[Tn][Tm][WBUF_t][WBUF_t]," + EOL
    str1 += "        data_type_w b_buf_0[Tm]," + EOL
    str1 += "        data_type_w out_buf_0[Tm][OBUF_t][OBUF_t]," + EOL
    str1 += "        int param[16] ) {" + EOL
    str1 += "    " + EOL

    str1 += "        data_type_w out_buf_tmp[Tm][Tr][Tc];" + EOL
    str1 += "#pragma HLS ARRAY_PARTITION variable=out_buf_tmp complete dim=1" + EOL
    str1 += "    " + EOL
    str1 += "        conv_engine(in_buf_0, w_buf_0, b_buf_0, out_buf_tmp, param[0], param[1], param[2], param[3], param[4], param[5], param[6], param[7], param[8]);" + EOL

    str1 += "    " + EOL
    str1 += "           for(int j =0; j < Tr; j++) { " + EOL
    str1 += "               for(int k=0; k < Tc; k++) {" + EOL
    str1 += "#pragma HLS PIPELINE " + EOL
    str1 += "                   for(int i=0; i < Tm; i++) { " + EOL
    str1 += "                       out_buf_0[i][j][k] = out_buf_tmp[i][j][k]; " + EOL
    str1 += "                   } " + EOL
    str1 += "               }" + EOL
    str1 += "           }" + EOL
    str1 += "}" + EOL

    str1 += "#endif" + EOL

    str1 += "};" + EOL
    str1 += "#endif" + EOL
    str1 += "    " + EOL
    str1 += "    " + EOL

    with open("../example/test_demo/inference_net/" + generated_file_name, "w") as generated_file:
        generated_file.write(str1)

    return str1


if __name__ == "__main__":
    generate()
