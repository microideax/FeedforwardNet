import helping_functions
import sys
import math
from model_partition import partition_to_k
from model_split     import model_split_by_list


def local_search(sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S, sub_flag):

    DSP = 6840/15
    # datatype = fixed
    factor = 1

    pair_1 = []
    lat_1  = 0
    util_1 = 0
    pair_2 = []
    lat_2  = 0
    util_2 = 0
    pair_3 = []
    lat_3  = 0
    util_3 = 0

    pair_list = []
    lat_list = []
    util_list = []

    # print "lists in sub_conv_N"
    # print len(sub_conv_N)
    # print sub_conv_N

    dsp_per_acc = DSP/len(sub_conv_N)

    for i in range(0, len(sub_conv_N)):
        # pair_list.append([])
        pair, cycle, cycle_per_layer = constrained_dse(sub_conv_N[i], sub_conv_M[i], sub_conv_r[i], sub_conv_R[i], sub_conv_K[i],
                                            sub_conv_S[i], sub_flag[i], int(dsp_per_acc), int(37), factor)
        pair_list.append(pair)
        lat_list.append(cycle)
        util_list.append(cycle_per_layer)

    # pair_1, lat_1, util_1 = constrained_dse(sub_conv_N[0], sub_conv_M[0], sub_conv_r[0], sub_conv_R[0], sub_conv_K[0],
    #                                 sub_conv_S[0], sub_flag[0], int(DSP/factor), int(37), factor)
    # pair_2, lat_2, util_2 = constrained_dse(sub_conv_N[1], sub_conv_M[1], sub_conv_r[1], sub_conv_R[1], sub_conv_K[1],
    #                                 sub_conv_S[1], sub_flag[1], int(DSP/factor*0.8), int(37), factor)
    # pair_3, lat_3, util_3 = constrained_dse(sub_conv_N[2], sub_conv_M[2], sub_conv_r[2], sub_conv_R[2], sub_conv_K[2],
    #                                 sub_conv_S[2], sub_flag[2], int(DSP/factor), int(37), factor)

    # return pair_1, lat_1, pair_2, lat_2, pair_3, lat_3, util_1, util_2, util_3
    return pair_list, lat_list, util_list

def constrained_dse(N, M, r, R, K, S, flag, DSP, P_const, factor):

    opt_pair = []
    min_local_cycle = 2000000000000

    for Tm in range(1, max(M) + 1):
        Tn_max = min(max(N), int(int(DSP / Tm)), Tm)
        for Tn in range(1, Tn_max + 1):
            cycle_per_layer = []
            local_cycles = conv_net_perf(N, M, R, K, flag, Tn, Tm, P_const)
            if local_cycles < min_local_cycle and local_cycles != 0:
                min_local_cycle = local_cycles
                opt_pair = [Tm, Tn, min_local_cycle]
    # print "opt_pair = ", opt_pair
    for j in range(0, int(len(N))):
        tmp = conv_layer_perf(N[j], M[j], R[j], K[j], opt_pair[0], opt_pair[1], P_const)
        cycle_per_layer.append(tmp)

    # dsp_util = net_dsp_util(N, M, opt_pair[0], opt_pair[1], DSP, cycle_per_layer)

    Acc_num = 1
    # while dsp_util < float(0.5):
    #     dsp_util *= 2
    #     min_local_cycle /=2
    #     Acc_num *= 2
    opt_pair.append(Acc_num)

    return opt_pair, min_local_cycle, cycle_per_layer


def global_search(layer_list, acc_cluster_num, conv_N, conv_M, conv_r, conv_R, conv_K, conv_S, flag, pair_list, overall_lat):
    """
    :param layer_list: a list containing each layer information in the form of a tuple (layer index, layer name).
    :param acc_num:
    :param conv_N:
    :param conv_M:
    :param conv_r:
    :param conv_R:
    :param conv_K:
    :param conv_S:
    :param flag:
    :param pair_list:
    :param overall_lat:
    :return:
    """
    sub_conv_N = []
    sub_conv_M = []
    sub_conv_r = []
    sub_conv_R = []
    sub_conv_K = []
    sub_conv_S = []
    sub_flag   = []
    sub_pair_list = []
    sub_lat_list = []
    sub_util_list = []
    item_list = []

    search_counter = 0

    print "started global search"

    for idx, item in enumerate(partition_to_k(layer_list, acc_cluster_num, False), 1):
        search_counter = search_counter + 1
        sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S, sub_flag \
            = model_split_by_list(conv_N, conv_M, conv_r, conv_R, conv_K, conv_S, flag, item)
        sub_pair_list, sub_lat_list, sub_util_list = \
            local_search(sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S, sub_flag)
        # print sub_pair_list, sub_lat_list, sub_util_list

        if max(sub_lat_list) < overall_lat:
            overall_lat = max(sub_lat_list)
            if len(pair_list) < 10:
                item_list.append(item)
                pair_list.append(sub_pair_list)
                pair_list.append([overall_lat])
                pair_list.append(sub_util_list)
            # else:
            #     max_among_mins = pair_list.index(max(overall_lat))
            #     pair_list.remove(pair_list[max_among_mins])
            #     pair_list.append(sub_pair_list)
            #     pair_list.append([overall_lat])
            #     pair_list.append(sub_util_list)

    print "Final explored points = ", search_counter
    return pair_list, item_list

# sub-net performance model function
def conv_net_perf(N, M, R, K, flag, Tn, Tm, P_const):
    tmp = 0
    Tr = 16
    Tc = 16
    for j in range(0, int(len(N))):
        if flag[j] == True:
            tmp += conv_layer_perf(N[j], M[j], R[j], K[j], Tn, Tm, P_const)
            # tmp += pool_layer_perf(M[j], R[j], K[j], Tm, P_const)
        else:
            tmp += conv_layer_perf(N[j], M[j], R[j], K[j], Tn, Tm, P_const)
    return tmp

# TODO: complete with new layer performance model
# convolutional layer performance
def conv_layer_perf(n, m, r, k, Tn, Tm, P_const):
    tmp = 0
    Tr = 8
    Tc = 8
    tmp = (math.ceil(n/float(Tn)))*(math.ceil(m/float(Tm)))*(math.ceil(r/float(Tr)))*(math.ceil(r/float(Tc)))*Tr*Tc*k*k + P_const
    # print(r, (math.ceil(r/float(Tr))))
    # tmp = (math.ceil(n/float(Tn)))*(math.ceil(m/float(Tm)))*r*r*k*k + P_const
    return tmp

def pool_layer_perf(m, r, k, Tm, P_const):
    tmp = (math.ceil(m/float(Tm))) * r * r * k * k + P_const
    return tmp

# TODO: complete wit new layer performance model
# fc layer performance
def fc_layer_perf(n, m, r, k, Tn, Tm, P_const):
    tmp = 0
    tmp += (math.ceil(n/float(Tn)))*(math.ceil(m/float(Tm)))*r*r*k*k + P_const
    return tmp

# network dsp utilization
def net_dsp_util(N, M, Tm, Tn, DSP, lat):
    overall_util = 0
    overall_lat = 0
    for j in range(0, int(len(N))):
        dsp_x = min(Tm, M[j])
        dsp_y = min(Tn, N[j])
        overall_util += (dsp_x * dsp_y * lat[j]) / float(DSP)
        overall_lat += lat[j]
    net_util = overall_util / float(overall_lat)
    return net_util

# accelerator runtime performance model
def acc_one_p_cycles(Tm, Tn, Tr, Tc, K,):
    cycles = Tr * Tc * K * K
    return cycles