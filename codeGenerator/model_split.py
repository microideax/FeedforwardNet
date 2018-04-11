import sys
import math


def model_split(conv_N, conv_M, conv_r, conv_R, conv_K, conv_S, i, j):

    sub_conv_N = []
    sub_conv_M = []
    sub_conv_r = []
    sub_conv_R = []
    sub_conv_K = []
    sub_conv_S = []

    sub_conv_N.append(conv_N[0: int(i)])
    sub_conv_N.append(conv_N[int(i): int(j)])
    sub_conv_N.append(conv_N[int(j): int(len(conv_N))])

    sub_conv_M.append(conv_M[0: int(i)])
    sub_conv_M.append(conv_M[int(i): int(j)])
    sub_conv_M.append(conv_M[int(j): int(len(conv_N))])

    sub_conv_r.append(conv_r[0: int(i)])
    sub_conv_r.append(conv_r[int(i): int(j)])
    sub_conv_r.append(conv_r[int(j): int(len(conv_N))])

    sub_conv_R.append(conv_R[0: int(i)])
    sub_conv_R.append(conv_R[int(i): int(j)])
    sub_conv_R.append(conv_R[int(j): int(len(conv_N))])

    sub_conv_K.append(conv_K[0: int(i)])
    sub_conv_K.append(conv_K[int(i): int(j)])
    sub_conv_K.append(conv_K[int(j): int(len(conv_N))])

    sub_conv_S.append(conv_S[0: int(i)])
    sub_conv_S.append(conv_S[int(i): int(j)])
    sub_conv_S.append(conv_S[int(j): int(len(conv_N))])

    return sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S


# if __name__ == "__main__":
#     sub_conv_N = model_split()