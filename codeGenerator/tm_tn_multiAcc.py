import helping_functions
import sys
import math

from model_extract import model_extract
from model_split   import model_split
from local_search  import local_search

def multiAcc_dse():

    # define the network parameter containers
    conv_N = []
    conv_M = []
    conv_r = []
    conv_R = []
    conv_K = []
    conv_S = []

    sub_conv_N = []
    sub_conv_M = []
    sub_conv_r = []
    sub_conv_R = []
    sub_conv_K = []
    sub_conv_S = []

    pair_1 = []
    pair_2 = []
    pair_3 = []
    lat_1 = 0
    lat_2 = 0
    lat_3 = 0
    pair_list = []
    overall_lat = 2000000000

    #step 1: split the input model into multiple sub-models (here we use 3 sub-models)
    conv_N, conv_M, conv_r, conv_R, conv_K, conv_S = model_extract()

    #step 2: split the original model into multiple sub models
    for i in range(1, int(len(conv_N)-2)):
        for j in range(int(i+1),int(len(conv_N)-1)):
            sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S = model_split(conv_N, conv_M, conv_r, conv_R, conv_K, conv_S, i, j)
            pair_1, lat_1, pair_2, lat_2, pair_3, lat_3  = local_search(sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S)

            # print(sub_conv_M, sub_conv_N)
            print(pair_1, pair_2, pair_3, lat_1, lat_2, lat_3)

            if max(lat_1, lat_2, lat_3) < overall_lat:
                overall_lat = max(lat_1, lat_2, lat_3)
                # if len(pair_list) < 10:
                pair_list.append(pair_1)
                pair_list.append(pair_2)
                pair_list.append(pair_3)
                pair_list.append(overall_lat)
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