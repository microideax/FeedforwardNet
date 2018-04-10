import helping_functions
import sys
import math

from model_extract import model_extract
from model_split   import model_split


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

    #step 1: split the input model into multiple sub-models (here we use 3 sub-models)
    conv_N, conv_M, conv_r, conv_R, conv_K, conv_S = model_extract()

    #step 2:
    for i in range(1, int(len(conv_N)-2)):
        for j in range(int(i+1),int(len(conv_N)-1)):
            sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S = model_split(conv_N, conv_M, conv_r, conv_R, conv_K, conv_S, i, j)
            print('sub conv N = ', sub_conv_N)
    #     #step 2-1:
    #     local_search()  #find local optimal solution
    #
    # #step 3:
    # note_overall_min()


if __name__ == "__main__":
    conv_N = multiAcc_dse()