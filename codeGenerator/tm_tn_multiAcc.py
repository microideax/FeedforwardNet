import helping_functions
import sys
import math

from model_extract import model_extract
from model_split   import model_split_ordered
from model_split   import model_split_unordered
from local_search  import local_search
from model_split   import gop_calculate
from model_split   import max_layer_dataout

def multiAcc_dse():

    # define the network parameter containers
    conv_N = []
    conv_M = []
    conv_r = []
    conv_R = []
    conv_K = []
    conv_S = []
    flag   = []

    sub_conv_N = []
    sub_conv_M = []
    sub_conv_r = []
    sub_conv_R = []
    sub_conv_K = []
    sub_conv_S = []
    sub_flag   = []

    pair_1 = []
    pair_2 = []
    pair_3 = []
    lat_1 = 0
    lat_2 = 0
    lat_3 = 0
    util_1 = 0
    util_2 = 0
    util_3 = 0
    OPs = 0
    pair_list = []
    pair_lists = []
    overall_lat = 605514

    #step 1: split the input model into multiple sub-models (here we use 3 sub-models)
    conv_N, conv_M, conv_r, conv_R, conv_K, conv_S, flag = model_extract('include_fc')
    OPs = gop_calculate(conv_N, conv_M, conv_R, conv_K)
    max_layerout = max_layer_dataout(conv_N, conv_M, conv_R, conv_K)

    print("Model extracted")
    print("Overall Operation requirement: ", OPs)
    print("max layer output data: ", max_layerout)

    # step 2: split the original model into multiple sub models
    print("layer number is: ", int(len(conv_N)))
    for i in range(1, int(len(conv_N)-1)):
        for j in range(int(i+1), int(len(conv_N))):
    # for i in range(1, 10):
    #     for j in range(1, 10):
            sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S, sub_flag = model_split_ordered(conv_N, conv_M, conv_r, conv_R, conv_K, conv_S, flag, i, j)
            # sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S, sub_flag = model_split_unordered(conv_N, conv_M, conv_r, conv_R, conv_K, conv_S, flag)
            print(sub_conv_N)
            pair_1, lat_1, pair_2, lat_2, pair_3, lat_3, util_1, util_2, util_3 = local_search(sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S, sub_flag)

            print(i, j, pair_1, "%.2f" % util_1, pair_2, "%.2f" % util_2, pair_3, "%.2f" % util_3, lat_1, lat_2, lat_3)

            if max(lat_1, lat_2, lat_3) < overall_lat:
                overall_lat = max(lat_1, lat_2, lat_3)
                # if len(pair_list) < 50:
                pair_list.append([i, j])
                pair_list.append(pair_1)
                pair_list.append(pair_2)
                pair_list.append(pair_3)
                pair_list.append([overall_lat])
                # else:
                #     max_among_mins  = pair_list.index(max(overall_lat))
                #     pair_list.remove(pair_list[max_among_mins])
                #     pair_list.append(pair_1)
                #     pair_list.append(pair_2)
                #     pair_list.append(pair_3)
                #     pair_list.append(overall_lat)
    print(pair_list)


    # #step 3:
    # find_min_in_pairs()
    # min_among_mins = pair_list.index(min(overall_lat))
    # print(pair_list[min_among_mins])


if __name__ == "__main__":
    conv_N = multiAcc_dse()