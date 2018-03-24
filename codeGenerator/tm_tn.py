import helping_functions
import sys
import math
from math import log10
from matplotlib import pyplot as plt
from matplotlib import cm
import numpy as np
from mpl_toolkits.mplot3d import Axes3D


def generate():
    """Tm * Tn < DSP/ 5"""

    # max_ratio = 0
    # max_sk = []
    # tm_tn_tr_tc = [32,8, 16,16]
    # for s in range(1,5):
    #     k_max = min(80-15*s, 11)
    #     for k in range (s, k_max):
    #         ctc_ratio = (32*16*16*((15*s+k)*(15*s+k)*8+1))/(4*(8*32*k*k + 32 + 8*(15*s+k)*(15*s+k)+32*16*16))
    #         if ctc_ratio > max_ratio:
    #             max_ratio = ctc_ratio
    #             max_sk = [s,k]
    # print("max ctc and s,k")
    # print(max_ratio)
    # print(max_sk)

    arr2 = helping_functions.read_params(sys.argv[1])
    prms, prms_str = helping_functions.extraction(arr2)

    init_conv_N = prms[prms_str.index("nn_in_number_conv")]
    init_conv_r = prms[prms_str.index("nn_in_data_size_conv")]
    init_conv_M = prms[prms_str.index("nn_out_number_conv")]
    init_conv_P = prms[prms_str.index("nn_padding_conv")]
    init_conv_K = prms[prms_str.index("nn_channel_size_conv")]
    init_conv_S = prms[prms_str.index("nn_stride_conv")]
    init_conv_G = prms[prms_str.index("nn_group_conv")]
    init_fc_N   = prms[prms_str.index("nn_in_number_fc")]
    init_fc_Rin = prms[prms_str.index("nn_in_data_size_fc")]
    init_fc_M   = prms[prms_str.index("nn_out_number_fc")]
    init_fc_K   = prms[prms_str.index("nn_channel_size_fc")]

    nn_in_number_conv_values1 = []
    if isinstance(init_fc_N, list):
        for fc_in_number in init_fc_N:
           nn_in_number_conv_values1.append(fc_in_number)
    else:
        nn_in_number_conv_values1.append(0)


    nn_out_number_conv_values1 = []
    if isinstance(init_fc_M, list):
        for fc_out_number in init_fc_M:
            nn_out_number_conv_values1.append(fc_out_number)
    else:
        nn_out_number_conv_values1.append(0)

    nn_fc_sizes_conv = []
    if isinstance(init_fc_Rin, list):
        for fc_in_size in init_fc_Rin:
           nn_fc_sizes_conv.append(fc_in_size)
    else:
        nn_fc_sizes_conv.append(0)

    nn_channel_size_conv_values=[]
    if isinstance(init_fc_K, list):
        for kernel_size in init_fc_K:
           nn_channel_size_conv_values.append(kernel_size)
    else:
        nn_channel_size_conv_values.append(0)

    nn_stride_values1=[]
    if isinstance(init_fc_Rin, list):
        for stride_value in init_fc_Rin:
           nn_stride_values1.append(stride_value)
    else:
        nn_stride_values1.append(0)

    conv_only_M = [int(val) for val in init_conv_M]
    # print init_conv_M
    # print conv_only_M

    init_conv_N = init_conv_N + nn_in_number_conv_values1
    init_conv_M = init_conv_M + nn_out_number_conv_values1
    init_conv_r = init_conv_r + nn_fc_sizes_conv
    init_conv_K = init_conv_K + nn_channel_size_conv_values
    init_conv_S = init_conv_S + nn_stride_values1

    conv_N = [int(string) for string in init_conv_N]
    conv_M = [int(string) for string in init_conv_M]
    conv_r = [int(string) for string in init_conv_r]
    conv_K = [int(string) for string in init_conv_K]
    conv_S = [int(string) for string in init_conv_S]
    conv_P = [int(string) for string in init_conv_P]
    conv_P = conv_P + [0] * len(init_fc_Rin)
    conv_G = [int(string) for string in init_conv_G]
    max_conv_N = max(conv_N)
    max_conv_M = max(conv_M)
    max_conv_S = max(conv_S)
    max_conv_K = max(conv_K)

    conv_R = []
    conv_layer_num = int(len(conv_r))
    for r in range(0, conv_layer_num):
        R = (conv_r[r] - conv_K[r] + conv_S[r] + 2*conv_P[r])/conv_S[r]
        conv_R.append(R)

    # find the positions of Conv layers followed by Pooling layer
    flag = [False] * conv_layer_num
    count = 0
    print prms[0]
    print len(prms[0])
    for prms_index in range(len(prms[0]) - 2):
        if "Convolution" in prms[0][prms_index]:
            if "Pooling" in prms[0][prms_index + 1] + prms[0][prms_index + 2]:
                flag[count] = True
            count += 1

    print "conv_N: ", conv_N
    print "conv_M: ", conv_M
    print "conv_r: ", conv_r
    print "conv_R: ", conv_R
    print "conv_K: ", conv_K


    DSP = 6800
    # DSP = 2800
    d = int(DSP / 5)
    arr = []
    # bandwidth = 2.5  # 1GB/s for float
    bandwidth = 5    # 1GB/s for fixed16

    Tm_min = 1
    Tn_min = 1
    Tr = 1
    min_Tm_Tn_Tr_Tc = []
    conv_min_cycles = 0
    min_cycle_list = []

    # double buffering modeling parameters
    min_iBuf_size_list = []
    P_const = 36
    Upp_bound = 128

    for o in range(0, conv_layer_num):
        conv_min_cycles += math.ceil(conv_R[o] / float(Tr)) * math.ceil(conv_R[o] / float(Tr)) * Tr *  Tr * math.ceil(int(conv_M[o]) / float(Tm_min)) * math.ceil(int(conv_N[o]) / float(Tn_min)) * conv_K[o] * conv_K[o] + (Tr * Tr) / 2
    min_Tm_Tn_Tr_Tc.append([1, 1, 1, 1])
    min_cycle_list.append(conv_min_cycles)

    print "Analysis initialized point: ", min_cycle_list, min_Tm_Tn_Tr_Tc

    # target = 0
    # for j in range(0, conv_layer_num):
    #     target += int(conv_R[j] * conv_R[j] * math.ceil(int(conv_N[j]) / float(16)) * math.ceil(int(conv_M[j]) / float(64))*conv_K[j] * conv_K[j])
    # print("targeted cycle numbers [32, 8]")
    # print(target)

    # fig = plt.figure()
    # ax = fig.gca(projection='3d')
    # # ax = Axes3D(fig)
    # ax.set_title("3D Figure")
    # ax.set_xlabel("Tm")
    # ax.set_ylabel("Tn")
    # ax.set_zlabel("Cycles")
    #
    # x_axis = [i for i in range(1, 100 + 1)]
    # y_axis = [j for j in range(1, 100 + 1)]
    # XX, YY = np.meshgrid(x_axis, y_axis)
    # ZZ = np.zeros((100, 100))e

    # explore the space
    for Tm in range(1, max(conv_only_M)+1):
        Tn_max = min(max_conv_N, int(int(d/Tm)), Tm)
        for Tn in range(1, Tn_max+1):
            for Tr in range(1, int(max(init_conv_r))):
                Tc = Tr

                for ibuf in [x for x in range((max_conv_S*Tr+max_conv_K), Upp_bound)]:
                    cycles = 0
                    T_trans_total = 0
                    T_Conv_com_total = 0
                    for j in range(0, conv_layer_num):
                        Pooling_cycles = 0
                        # T_trans = math.ceil(Tn * math.pow(ibuf, 2) / float(1024)) * 200
                        T_trans = Tn*math.ceil((math.pow(ibuf, 2)/bandwidth))
                        T_Conv_com = Tr * Tc * math.pow(int(init_conv_K[j]), 2) + P_const
                        # if pooling
                        #     pool_cycle
                        #     cycles += pool_cycle
                        # else
                        if flag[j]:
                            Pooling_cycles = int(math.ceil(conv_R[j] / float(Tr)) * math.ceil(conv_R[j] / float(Tc))) * math.ceil(conv_N[j] / float(Tn))
                        cycles += (Pooling_cycles + int(math.ceil(conv_R[j] / float(Tr)) * math.ceil(conv_R[j] / float(Tc)) * math.ceil(conv_N[j] / float(Tn)) * math.ceil(conv_M[j] / float(Tm)) * max(T_trans, T_Conv_com)))
                        T_trans_total += T_trans
                        T_Conv_com_total += (int(math.ceil(conv_R[j] / float(Tr)) * math.ceil(conv_R[j] / float(Tc)) * math.ceil(conv_N[j] / float(Tn)) * math.ceil(conv_M[j] / float(Tm))))*T_Conv_com

                    if cycles < min(min_cycle_list) and cycles != 0:
                        conv_min_cycles = cycles
                        if len(min_Tm_Tn_Tr_Tc) < 10:
                            min_Tm_Tn_Tr_Tc.append([Tm, Tn, Tr, Tc, ibuf, T_Conv_com_total, cycles])
                            min_cycle_list.append(conv_min_cycles)
                        else:
                            max_among_mins = min_cycle_list.index(max(min_cycle_list))
                            min_cycle_list.remove(min_cycle_list[max_among_mins])
                            min_Tm_Tn_Tr_Tc.remove(min_Tm_Tn_Tr_Tc[max_among_mins])
                            min_cycle_list.append(conv_min_cycles)
                            min_Tm_Tn_Tr_Tc.append([Tm, Tn, Tr, Tc, ibuf, T_Conv_com_total, cycles])

    # def getKey(item):
    #     return item[0]
    # for item in min_Tm_Tn_Tr_Tc:
    #     tn = item[1]
    #     tr = item[2]
    #     tc = item[3]
    #
    #     # iBuf size
    #     for ibuf in [x for x in range(11, Upp_bound)]:
    #         flag = True
    #         for j in range(0, conv_layer_num):
    #             T_trans = tn * math.ceil(math.pow(ibuf, 2) / float(1024)) * 1200
    #             T_Conv_com = tr * tc * math.pow(int(init_conv_K[j]), 2) + P_const
    #             if T_trans >= T_Conv_com:
    #                 flag = False
    #         if flag:
    #             if len(min_iBuf_size_list) < 10:
    #                 min_iBuf_size_list.append([ibuf, tn] + item)
    #                 sorted(min_iBuf_size_list, key=getKey)
    #             else:
    #                 # min_iBuf_index = min_iBuf_size_list.index(max(min_iBuf_size_list))
    #                 min_iBuf_size_list.remove(min_iBuf_size_list[9])
    #                 min_iBuf_size_list.append([ibuf, tn] + item)
    #                 sorted(min_iBuf_size_list, key=getKey)

    # surf = ax.plot_surface(XX, YY, ZZ, rstride=1, cstride=1, cmap=cm.coolwarm, linewidth=0, antialiased=True)
    # fig.colorbar(surf, shrink=0.5, aspect=5)
    # #plt.pause(1)
    # plt.show()

    print "Tm, Tn, Tr, Tc, ibuf, T_Conv_com, cycles: ", min_Tm_Tn_Tr_Tc
    print "cycles: ", min_cycle_list

    min_among_all = min_cycle_list.index(min(min_cycle_list))
    print "Best among all points", min_cycle_list[min_among_all], min_Tm_Tn_Tr_Tc[min_among_all]

    in_buf = 0
    out_buf = 0
    w_buf = 0
    Tr_Tc = []
    in_buff_arr = []
    w_buff_arr = []
    out_buff_arr = []
    total_arr = []
    for m in min_Tm_Tn_Tr_Tc:
        Tr = int(math.sqrt(m[0] * m[1]))
        Tr_Tc.append([Tr, Tr])
        in_buff = ((Tr - 1) * max_conv_S + max_conv_K) * ((Tr - 1) * max_conv_S + max_conv_K)
        in_buff_arr.append(in_buff)
        out_buff = m[0] * m[1] * max_conv_K * max_conv_K
        out_buff_arr.append(out_buff)
        w_buff = Tr * Tr * m[0]
        w_buff_arr.append(w_buff)
        total = in_buff + out_buff + w_buff
        total_arr.append(total)
    print "in_buf: ", in_buff_arr
    print "w_buf: ", w_buff_arr
    print "out_buf: ", out_buff_arr
    print "total: ", total_arr
    return arr, conv_min_cycles


if __name__ == "__main__":
    arr, conv_min_cycles = generate()