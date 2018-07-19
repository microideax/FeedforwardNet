import helping_functions
import sys
import math
from model_partition import partition_to_k
from model_split import model_split_by_list
from model_split import gop_calculate
import pprint
import threading
import Queue
import time

# convolutional layer performance
def conv_layer_perf(n, m, r, s, k, Tn, Tm, P_const):
    tmp = 0
    Tr = 8
    Tc = 8

    # revised layer performance
    R_iter = math.ceil(r / float(Tr))
    M_iter = math.ceil(m / float(Tm))
    N_iter = math.ceil(n / float(Tn))
    lat_read = Tn * (Tr-1)*s + k
    lat_com = Tr * Tc * k * k
    lat_out = Tm*Tr*Tc
    tmp = R_iter * R_iter * M_iter * (lat_read + N_iter*lat_com + lat_out)

    return tmp

def pool_layer_perf(m, r, k, Tm, P_const):
    tmp = (math.ceil(m / float(Tm))) * r * r * k * k + P_const
    return tmp


# TODO: complete wit new layer performance model
# fc layer performance
def fc_layer_perf(n, m, r, k, Tn, Tm, P_const):
    tmp = 0
    tmp += (math.ceil(n / float(Tn))) * (math.ceil(m / float(Tm))) * r * r * k * k + P_const
    return tmp

# sub-net performance model function
def conv_net_perf(N, M, R, S, K, flag, Tn, Tm, P_const):
    tmp = 0
    # Tr = 16
    # Tc = 16
    for j in range(0, int(len(N))):
        if flag[j] == True:
            tmp += conv_layer_perf(N[j], M[j], R[j], S[j], K[j], Tn, Tm, P_const)
            # tmp += pool_layer_perf(M[j], R[j], K[j], Tm, P_const)
        else:
            tmp += conv_layer_perf(N[j], M[j], R[j], S[j], K[j], Tn, Tm, P_const)
    return tmp

# Optimal Tm, Tn pair selection with given amount of DSP
def constrained_dse(N, M, r, R, K, S, flag, DSP, P_const, factor):
    opt_pair = []
    cycle_per_layer = []
    min_local_cycle = 2000000000000

    for Tm in range(1, max(M) + 1):
        Tn_max = min(max(N), int(int(DSP / Tm)), Tm)
        for Tn in range(1, Tn_max + 1):
            local_cycles = conv_net_perf(N, M, R, S, K, flag, Tn, Tm, P_const)
            if local_cycles < min_local_cycle and local_cycles != 0:
                min_local_cycle = local_cycles
                opt_pair = [Tm, Tn, local_cycles]

    # collected the detailed performance for each layer in a sub-net
    for j in range(0, int(len(N))):
        tmp = 0
        tmp = conv_layer_perf(N[j], M[j], R[j], S[j], K[j], opt_pair[1], opt_pair[0], P_const)
        cycle_per_layer.append(tmp)

    Acc_num = 1
    opt_pair.append(Acc_num)

    return opt_pair, min_local_cycle, cycle_per_layer



def local_search(sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S, sub_flag):
    """

    :param sub_conv_N: the input sub_conv_N is already splitted into several sub-nets
    :param sub_conv_M: same as above
    :param sub_conv_r: saa
    :param sub_conv_R: saa
    :param sub_conv_K: saa
    :param sub_conv_S: saa
    :param sub_flag: saa
    :return: the most optimal configuration for current sub-nets for an optimal system latency
    """
    DSP = 6840 / 15
    # datatype = fixed
    factor = 1

    pair_1 = []
    lat_1 = 0
    util_1 = 0
    pair_2 = []
    lat_2 = 0
    util_2 = 0
    pair_3 = []
    lat_3 = 0
    util_3 = 0

    pair_list = []
    lat_list = []
    util_list = []
    gop_list = []
    gop_per_subnet = []
    gop_total = 0
    dsp_per_acc = []
    dsp_occupied = 0
    # print "lists in sub_conv_N"
    # print len(sub_conv_N)
    # print sub_conv_N

    step = int(1)
    ratio = 0.05
    search_counter = 0
    Resolution = 100
    """initializing the dsp number for per acc based on the ops requirement"""
    for i in range(0, len(sub_conv_N)):
        gop_per_subnet.append(gop_calculate(sub_conv_N[i], sub_conv_M[i], sub_conv_R[i], sub_conv_K[i]))
        gop_total += gop_per_subnet[i]

    for i in range(0, len(sub_conv_N)):
        if i < len(sub_conv_N) - 1:
            dsp_per_acc.append(math.ceil(DSP * (gop_per_subnet[i]/float(gop_total))))
            dsp_occupied += dsp_per_acc[i]
        else:
            dsp_per_acc.append(math.ceil(DSP - dsp_occupied))

    """ Iteratively find the system level optimal configuration for the all the sub-nets"""
    search_stop = 0
    while search_stop == 0 and search_counter < Resolution + 1:
        for i in range(0, len(sub_conv_N)):
            pair, cycle, cycle_per_layer = constrained_dse(sub_conv_N[i], sub_conv_M[i], sub_conv_r[i], sub_conv_R[i],
                                                           sub_conv_K[i],
                                                           sub_conv_S[i], sub_flag[i], int(dsp_per_acc[i]), int(37),
                                                           factor)
            pair_list.append(pair)
            lat_list.append(cycle)
            util_list.append(pair[0]*pair[1]/float(DSP))
            if len(pair_list) > len(sub_conv_N):
                for remove_cnt in range(0, len(sub_conv_N)):
                    pair_list.remove(pair_list[0])
                    lat_list.remove(lat_list[0])
                    util_list.remove(util_list[0])

        ratio_tmp = ((max(lat_list) - min(lat_list)) / float(min(lat_list)))

        if ratio_tmp < ratio or search_counter == Resolution:
            search_stop = 1
        else:
            max_idx = lat_list.index(min(lat_list))
            min_idx = lat_list.index(max(lat_list))
            if dsp_per_acc[max_idx] > step:
                dsp_per_acc[max_idx] = dsp_per_acc[max_idx] - step
                dsp_per_acc[min_idx] = dsp_per_acc[min_idx] + step

        search_counter = search_counter + 1
        if search_stop == 1:
            print "search stopped at =", search_counter - 1, "current ratio: ", ratio_tmp

    return pair_list, lat_list, util_list


result_Q = Queue.Queue()
THREAD_NUM = 8


class SearchThread(threading.Thread):
    def __int__(self, layer_list, acc_cluster_num, conv_N, conv_M, conv_r, conv_R, conv_K, conv_S, flag, overall_lat, threadIdx):
        threading.Thread.__init__(self)
        self.layer_list = layer_list
        self.acc_cluster_num = acc_cluster_num
        self.conv_N = conv_N
        self.conv_M = conv_M
        self.conv_r = conv_r
        self.conv_R = conv_R
        self.conv_K = conv_K
        self.conv_S = conv_S
        self.flag = flag
        self.overall_lat = overall_lat
        self.threadIdx = threadIdx

    def run(self):
        gop_list = []
        item_list = []
        util_list = []
        pair_list = []

        search_counter = 0

        print "Thread " + self.threadIdx + " starts global search."

        for idx, item in enumerate(partition_to_k(self.layer_list, self.acc_cluster_num, False), 1):
            if item[0][0] % THREAD_NUM == self.threadIdx:
                sub_gop_list = []
                search_counter = search_counter + 1
                sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S, sub_flag \
                    = model_split_by_list(self.conv_N, self.conv_M, self.conv_r, self.conv_R, self.conv_K, self.conv_S, self.flag, item)
                sub_pair_list, sub_lat_list, sub_util_list = \
                    local_search(sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S, sub_flag)

                for i in range(0, len(sub_conv_N)):
                    sub_gop_list.append(gop_calculate(sub_conv_N[i], sub_conv_M[i], sub_conv_R[i], sub_conv_K[i]))

                if max(sub_lat_list) < overall_lat:
                    overall_lat = max(sub_lat_list)
                    if len(pair_list) < 4:
                        item_list.append(item)
                        pair_list.append(sub_pair_list)
                        pair_list.append([overall_lat])
                        gop_list.append(sub_gop_list)
                        util_list.append(sub_util_list)
                        # pair_list.append(sub_util_list)
                    # else:
                    #     max_among_mins = pair_list.index(max(overall_lat))
                    #     pair_list.remove(pair_list[max_among_mins])
                    #     pair_list.append(sub_pair_list)
                    #     pair_list.append([overall_lat])
                    #     pair_list.append(sub_util_list)

                print "For set starting with " + item[0][0] + ", the final explored points = ", search_counter

        result_Q.put((pair_list, item_list, gop_list, util_list, self.threadIdx))


def global_search(layer_list, acc_cluster_num, conv_N, conv_M, conv_r, conv_R, conv_K, conv_S, flag, overall_lat):
    """
    :param layer_list: a list containing each layer information in the form of a tuple (layer index, layer name).
    :param acc_cluster_num:
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
    sub_flag = []
    sub_pair_list = []
    sub_lat_list = []
    sub_util_list = []

    gop_list = []
    item_list = []
    util_list = []
    pair_list = []

    threads = []
    for i in range(THREAD_NUM):
        t = SearchThread(layer_list, acc_cluster_num, conv_N, conv_M, conv_r, conv_R, conv_K, conv_S, flag, overall_lat, i)
        threads.append(t)
        t.start()

    for thread_item in threads:
        thread_item.join()

    results = list()
    while not result_Q.empty():
        results.append(result_Q.get())
    for item in results:
        pair_list.append(item[0])
        item_list.append(item[1])
        gop_list.append(item[2])
        util_list.append(item[3])

    return pair_list, item_list, gop_list, util_list

def single_item_search(layer_list, acc_cluster_num, conv_N, conv_M, conv_r, conv_R, conv_K, conv_S, flag, pair_list,
                  overall_lat):
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
    item_list = []
    search_counter = 0
    print "started global search"

    item = [0], [1, 2], [3, 4]

    print item
    sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S, sub_flag \
            = model_split_by_list(conv_N, conv_M, conv_r, conv_R, conv_K, conv_S, flag, item)
    sub_pair_list, sub_lat_list, sub_util_list = \
            local_search(sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S, sub_flag)

    if max(sub_lat_list) < overall_lat:
        overall_lat = max(sub_lat_list)
        if len(pair_list) < 3:
            item_list.append(item)
            pair_list.append(sub_pair_list)
            pair_list.append([overall_lat])

    # print "Final explored points = ", search_counter
    return pair_list, item_list

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
def acc_one_p_cycles(Tm, Tn, Tr, Tc, K, ):
    cycles = Tr * Tc * K * K
    return cycles

    # pair_1, lat_1, util_1 = constrained_dse(sub_conv_N[0], sub_conv_M[0], sub_conv_r[0], sub_conv_R[0], sub_conv_K[0],
    #                                 sub_conv_S[0], sub_flag[0], int(DSP/factor), int(37), factor)
    # pair_2, lat_2, util_2 = constrained_dse(sub_conv_N[1], sub_conv_M[1], sub_conv_r[1], sub_conv_R[1], sub_conv_K[1],
    #                                 sub_conv_S[1], sub_flag[1], int(DSP/factor*0.8), int(37), factor)
    # pair_3, lat_3, util_3 = constrained_dse(sub_conv_N[2], sub_conv_M[2], sub_conv_r[2], sub_conv_R[2], sub_conv_K[2],
    #                                 sub_conv_S[2], sub_flag[2], int(DSP/factor), int(37), factor)
