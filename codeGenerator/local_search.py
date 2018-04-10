import helping_functions
import sys
import math


def local_search(sub_network, res_list):
    for Tm in range(1, max(conv_only_M) + 1):
        Tn_max = min(max_conv_N, int(int(d / Tm)), Tm)
        for Tn in range(1, Tn_max + 1):
            for Tr in range(1, int(max(init_conv_r))):
                Tc = Tr
                # for ibuf in [x for x in range((max_conv_S*Tr+max_conv_K), Upp_bound)]:
                # for j in range(0, conv_layer_num):
                #     if (conv_S[j]*Tr + conv_K[j]) > ibuf:
                #         ibuf = (conv_S[j]*Tr + conv_K[j])
                cycles = 0
                T_trans_total = 0
                T_Conv_com_total = 0
                for j in range(0, conv_layer_num):
                    Pooling_cycles = 0
                    # T_trans = math.ceil(Tn * math.pow(ibuf, 2) / float(1024)) * 100
                    T_trans = 0
                    # T_trans = (Tn*math.pow((Tr*conv_S[j] + conv_K[j]), 2))/160
                    if flag[j]:
                        T_Conv_com = Tr * Tc * (math.pow(int(init_conv_K[j]), 2) + 2) + P_const
                    else:
                        T_Conv_com = Tr * Tc * (math.pow(int(init_conv_K[j]), 2) + 1) + P_const

                    if flag[j]:
                        Pooling_cycles = int(math.ceil(conv_R[j] / (2 * float(Tr))) * math.ceil(
                            conv_R[j] / (2 * float(Tc)))) * math.ceil(conv_M[j] / float(Tm))
                    cycles += (Pooling_cycles + int(
                        math.ceil(conv_R[j] / float(Tr)) * math.ceil(conv_R[j] / float(Tc)) * math.ceil(
                            conv_N[j] / float(Tn)) * math.ceil(conv_M[j] / float(Tm)) * max(T_trans, T_Conv_com)))
                    T_trans_total += T_trans
                    T_Conv_com_total += (int(
                        math.ceil(conv_R[j] / float(Tr)) * math.ceil(conv_R[j] / float(Tc)) * math.ceil(
                            conv_N[j] / float(Tn)) * math.ceil(conv_M[j] / float(Tm)))) * T_Conv_com

                if cycles < min(min_cycle_list) and cycles != 0:
                    conv_min_cycles = cycles
                    # compute ibuf size with current Tr Tc
                    if len(min_Tm_Tn_Tr_Tc) < 10:
                        for j in range(0, conv_layer_num):
                            if (conv_S[j] * Tr + conv_K[j]) > ibuf:
                                ibuf = (conv_S[j] * Tr + conv_K[j])
                        min_Tm_Tn_Tr_Tc.append([Tm, Tn, Tr, Tc, ibuf, T_Conv_com_total, cycles])
                        min_cycle_list.append(conv_min_cycles)
                    else:
                        max_among_mins = min_cycle_list.index(max(min_cycle_list))
                        min_cycle_list.remove(min_cycle_list[max_among_mins])
                        min_Tm_Tn_Tr_Tc.remove(min_Tm_Tn_Tr_Tc[max_among_mins])
                        min_cycle_list.append(conv_min_cycles)
                        min_Tm_Tn_Tr_Tc.append([Tm, Tn, Tr, Tc, ibuf, T_Conv_com_total, cycles])

    return conv_min_cycles, arr


if __name__ == '__local_search__':
    local_search()