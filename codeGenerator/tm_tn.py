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

    arr2 = helping_functions.read_params(sys.argv[1])
    prms, prms_str = helping_functions.extraction(arr2)

    init_conv_N = prms[prms_str.index("nn_in_number_conv")]
    init_conv_r = prms[prms_str.index("nn_in_data_size_conv")]
    init_conv_M = prms[prms_str.index("nn_out_number_conv")]
    init_conv_P = prms[prms_str.index("nn_padding_conv")]
    init_conv_K = prms[prms_str.index("nn_channel_size_conv")]
    init_conv_S = prms[prms_str.index("nn_stride_conv")]
    init_fc_N   = prms[prms_str.index("nn_in_number_fc")]
    init_fc_Rin = prms[prms_str.index("nn_in_data_size_fc")]
    init_fc_M   = prms[prms_str.index("nn_out_number_fc")]
    init_fc_K   = prms[prms_str.index("nn_channel_size_fc")]

    #for fc_in_number in nn_in_number_fc_values:
    #    nn_in_number_conv_values1.append(fc_in_number)

    #for fc_out_number in nn_out_number_fc_values:
    #    nn_out_number_conv_values1.append(fc_out_number)

    #for kernel_size in nn_channel_size_fc_values:
    #    nn_channel_size_conv_values.append(kernel_size)

    #for conv_in_size in nn_in_data_sizes_fc:
    #    nn_in_data_sizes_conv.append(1)

    #for stride_value in nn_channel_size_fc_values:
    #    nn_stride_values1.append(stride_value)

    conv_N = [int(string) for string in init_conv_N]
    conv_M = [int(string) for string in init_conv_M]
    conv_r = [int(string) for string in init_conv_r]
    conv_K = [int(string) for string in init_conv_K]
    conv_S = [int(string) for string in init_conv_S]
    conv_P = [int(string) for string in init_conv_P]
    max_conv_N = max(conv_N)
    max_conv_M = max(conv_M)
    max_conv_S = max(conv_S)
    max_conv_K = max(conv_K)

    conv_R = []
    conv_layer_num = int(len(conv_r))
    for r in range(0, conv_layer_num):
        R = (conv_r[r] - conv_K[r] + conv_S[r] + 2*conv_P[r])/conv_S[r]
        conv_R.append(R)

    print("conv_N")
    print(conv_N)
    print("conv_M")
    print(conv_M)
    print("conv_r")
    print(conv_r)
    print("conv_R")
    print(conv_R)
    print("conv_K")
    print(conv_K)

    DSP = 6840
    d = int(0.8*DSP / 5)
    arr = []

    Tm_min = 1
    Tn_min = 1
    min_Tm_Tn = []
    conv_min_cycles = 0
    min_cycle_list = []
    for o in range(0, conv_layer_num):
        conv_min_cycles += conv_R[o] * conv_R[o] * math.ceil(int(conv_M[o]) / float(Tm_min)) * math.ceil(int(conv_N[o]) / float(Tn_min)) * conv_K[o] * conv_K[o]
    min_Tm_Tn.append([1, 1])
    min_cycle_list.append(conv_min_cycles)

    print("Analysis initialized point: ", min_cycle_list, min_Tm_Tn)
    #print(min_cycle_list)

    fig = plt.figure()
    ax = fig.gca(projection='3d')
    # ax = Axes3D(fig)
    ax.set_title("3D Figure")
    ax.set_xlabel("Tm")
    ax.set_ylabel("Tn")
    ax.set_zlabel("Cycles")

    x_axis = [i for i in range(1, 100 + 1)]
    y_axis = [j for j in range(1, 100 + 1)]
    XX, YY = np.meshgrid(x_axis, y_axis)
    ZZ = np.zeros((100, 100))

    conv_layer_num = int(len(conv_M))
    for Tm in range(1, max_conv_M+1):
        Tn_max = min(max_conv_N, int(int(d/Tm)))
        for Tn in range(1, Tn_max+1):
            cycles = 0
            for j in range(0, conv_layer_num):
                cycles += int(conv_R[j] * conv_R[j] * math.ceil(int(conv_N[j]) / float(Tn)) * math.ceil(int(conv_M[j]) / float(Tm)) * conv_K[j] * conv_K[j])

            if cycles > 0 and Tm < 100 and Tn < 100:
                ZZ[Tm, Tn] = cycles
            else:
                if Tm < 100 and Tn < 100:
                    ZZ[Tm, Tn] = 0

            if cycles < min(min_cycle_list) and cycles != 0:
                conv_min_cycles = cycles
                Tm_min = Tm
                Tn_min = Tn
                if len(min_Tm_Tn) < 5:
                    min_Tm_Tn.append([Tm, Tn])
                    min_cycle_list.append(conv_min_cycles)
                else:
                    max_among_mins = min_cycle_list.index(max(min_cycle_list))
                    min_cycle_list.remove(min_cycle_list[max_among_mins])
                    min_Tm_Tn.remove(min_Tm_Tn[max_among_mins])
                    min_cycle_list.append(conv_min_cycles)
                    min_Tm_Tn.append([Tm, Tn])

    surf = ax.plot_surface(XX, YY, ZZ, rstride=1, cstride=1, cmap=cm.coolwarm, linewidth=0, antialiased=True)
    fig.colorbar(surf, shrink=0.5, aspect=5)
    #plt.pause(1)
    plt.show()

    print("Tm and Tn")
    print(min_Tm_Tn)
    print("cycles")
    print(min_cycle_list)

    min_among_all = min_cycle_list.index(min(min_cycle_list))
    print("Best among all points", min_cycle_list[min_among_all], min_Tm_Tn[min_among_all])

    in_buf = 0
    out_buf = 0
    w_buf = 0
    Tr_Tc = []
    in_buff_arr = []
    w_buff_arr = []
    out_buff_arr = []
    total_arr = []
    for m in min_Tm_Tn:
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
    print("in_buf")
    print(in_buff_arr)
    print("w_buf")
    print(w_buff_arr)
    print("out_buf")
    print(out_buff_arr)
    print("total")
    print(total_arr)
    return arr, conv_min_cycles


if __name__ == "__main__":
    arr, conv_min_cycles = generate()
