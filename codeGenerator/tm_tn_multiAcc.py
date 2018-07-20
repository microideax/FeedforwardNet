import helping_functions
import sys
import math

from model_extract import model_extract
from model_split import model_split_ordered
from model_split import model_split_unordered
from local_search import local_search
from model_split import gop_calculate
from model_split import max_layer_dataout
from model_split import model_split_by_label
from model_split import model_split_by_list
from cluster import clusters_layers_kmeans
from model_partition import partition
from model_partition import partition_to_k
from local_search import global_search
from local_search import single_item_search
import time


def multiAcc_dse():
    # define the network parameter containers
    conv_N = []
    conv_M = []
    conv_r = []
    conv_R = []
    conv_K = []
    conv_S = []
    flag = []

    sub_conv_N = []
    sub_conv_M = []
    sub_conv_r = []
    sub_conv_R = []
    sub_conv_K = []
    sub_conv_S = []
    sub_flag = []

    pair_1 = []
    pair_2 = []
    pair_3 = []
    lat_1 = 0
    lat_2 = 0
    lat_3 = 0
    sub_lat_list = []
    lat_list = []

    util_1 = 0
    util_2 = 0
    util_3 = 0
    sub_util_list = []
    util_list = []

    OPs = 0
    sub_pair_list = []
    item_list = []
    pair_list = []
    overall_lat = 60551400
    layer_list = []
    gop_list = []

    """
    step 1: extract model from the original txt file with parameter no_include_fc / include_fc
    """
    conv_N, conv_M, conv_r, conv_R, conv_K, conv_S, flag = model_extract('no_include_fc')
    OPs = gop_calculate(conv_N, conv_M, conv_R, conv_K)
    max_layerout = max_layer_dataout(conv_N, conv_M, conv_R, conv_K)

    print("Model extracted")
    print("Overall Operation required: ", OPs)
    print("max layer output data: ", max_layerout)

    '''step 2: randomly cluster, param k=4, layer label results are in iterm'''
    for i in range(0, len(conv_N)):
        layer_list.append(i)
    # kmeans=clusters_layers_kmeans(conv_N, conv_M, conv_r, conv_R, conv_K, conv_S, 2)
    # print kmeans

    start = time.clock()
    acc_cluster_num = 3
    pair_list, item_list, gop_list, util_list = global_search(layer_list, acc_cluster_num, conv_N, conv_M, conv_r, conv_R, conv_K, conv_S, flag, overall_lat)
    # pair_list, item_list = single_item_search(layer_list, acc_cluster_num, conv_N, conv_M, conv_r, conv_R, conv_K,
    #                                           conv_S, flag, pair_list, overall_lat)
    print time.clock() - start, "s"

    print item_list
    print gop_list
    print pair_list
    for i in range(0, len(util_list)):
        print util_list[i], sum(util_list[i])

    # item = return_partition(layer_list, 4, False)
    #
    # '''step 3: split the layers based on label clustering results'''
    # print("layer number is: ", int(len(conv_N)))
    # sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S, sub_flag \
    #     = model_split_by_list(conv_N, conv_M, conv_r, conv_R, conv_K, conv_S, flag, item)
    # print sub_conv_N
    # print "model clustering test done!!!"
    #
    # '''step 4: do local search for all sub-models and find optimial <Tm, Tn> pair, lat, and util'''
    # sub_pair_list, sub_lat_list, sub_util_list = \
    #     local_search(sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S, sub_flag)
    # print sub_pair_list, sub_lat_list, sub_util_list
    #
    # if max(sub_lat_list) < overall_lat:
    #     overall_lat = max(sub_lat_list)
    #     if len(pair_list) < 10:
    #         pair_list.append(sub_pair_list)
    #         pair_list.append([overall_lat])
    #     else:
    #         max_among_mins = pair_list.index(max(overall_lat))
    #         pair_list.remove(pair_list[max_among_mins])
    #         pair_list.append(sub_pair_list)
    #         pair_list.append([overall_lat])

    # print(pair_1, "%.2f" % util_1, pair_2, "%.2f" % util_2, pair_3, "%.2f" % util_3, lat_1, lat_2, lat_3)
    # for i in range(1, int(len(conv_N)-1)):
    #     for j in range(int(i+1), int(len(conv_N))):
    # for i in range(1, 10):
    #     for j in range(1, 10):
    #         sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S, sub_flag = model_split_ordered(conv_N, conv_M, conv_r, conv_R, conv_K, conv_S, flag, i, j)
    # sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S, sub_flag = model_split_unordered(conv_N, conv_M, conv_r, conv_R, conv_K, conv_S, flag)
    # sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S, sub_flag = model_split_by_label(conv_N, conv_M, conv_r, conv_R, conv_K, conv_S, flag, kmeans.labels_)
    # print(sub_conv_N)
    # pair_1, lat_1, pair_2, lat_2, pair_3, lat_3, util_1, util_2, util_3 = local_search(sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S, sub_flag)
    #
    # print(i, j, pair_1, "%.2f" % util_1, pair_2, "%.2f" % util_2, pair_3, "%.2f" % util_3, lat_1, lat_2, lat_3)
    #
    # if max(lat_1, lat_2, lat_3) < overall_lat:
    #     overall_lat = max(lat_1, lat_2, lat_3)
    #     # if len(pair_list) < 50:
    #     pair_list.append([i, j])
    #     pair_list.append(pair_1)
    #     pair_list.append(pair_2)
    #     pair_list.append(pair_3)
    #     pair_list.append([overall_lat])
    #     # else:
    #     #     max_among_mins  = pair_list.index(max(overall_lat))
    #     #     pair_list.remove(pair_list[max_among_mins])
    #     #     pair_list.append(pair_1)
    #     #     pair_list.append(pair_2)
    #     #     pair_list.append(pair_3)
    #     #     pair_list.append(overall_lat)
    # print(pair_list)

    # #step 3:
    # find_min_in_pairs()
    # min_among_mins = pair_list.index(min(overall_lat))
    # print(pair_list[min_among_mins])


if __name__ == "__main__":
    conv_N = multiAcc_dse()
