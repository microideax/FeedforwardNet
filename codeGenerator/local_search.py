import helping_functions
import sys
import math


def local_search(sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S):

    pair_1 = []
    lat_1  = 0
    pair_2 = []
    lat_2  = 0
    pair_3 = []
    lat_3  = 0

    DSP = 6800/3
    # datatype = fixed
    factor = 1

    # for i in xrange(int(len(sub_conv_N))):
    pair_1, lat_1 = constrained_dse(sub_conv_N[0], sub_conv_M[0], sub_conv_r[0], sub_conv_R[0], sub_conv_K[0],
                                    sub_conv_S[0], int(DSP/factor), int(37))
    pair_2, lat_2 = constrained_dse(sub_conv_N[1], sub_conv_M[1], sub_conv_r[1], sub_conv_R[1], sub_conv_K[1],
                                    sub_conv_S[1], int(DSP/factor), int(37))
    pair_3, lat_3 = constrained_dse(sub_conv_N[2], sub_conv_M[2], sub_conv_r[2], sub_conv_R[2], sub_conv_K[2],
                                    sub_conv_S[2], int(DSP/factor), int(37))

    return pair_1, lat_1, pair_2, lat_2, pair_3, lat_3

#
# if __name__ == '__local_search__':
#     local_search()


def constrained_dse(N, M, r, R, K, S, DSP, P_const):

    opt_pair = []
    min_local_cycle = 2000000000000

    for Tm in range(1, max(M) + 1):
        Tn_max = min(max(N), int(int(DSP / Tm)), Tm)
        for Tn in range(1, Tn_max + 1):
            local_cycles = 0
            for j in range(0, int(len(N))):
                local_cycles += math.ceil(N[j]/float(Tn)) * math.ceil(M[j]/float(Tm)) * R[j] *R[j] * K[j] * K[j]

            if local_cycles < min_local_cycle and local_cycles != 0:
                min_local_cycle = local_cycles
                opt_pair = [Tm, Tn]
    return opt_pair, min_local_cycle