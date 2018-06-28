# Author Songyou on 28 June 2018

from sklearn.cluster import KMeans
import numpy as np
from random import randint

def clusters_layers(sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S, K):

    # layer_tmp = [len(sub_conv_N)][6]
    # layer_tmp = []
    #
    # for l in range(0, len(sub_conv_N), 1):
    #     layer_tmp[l].append(sub_conv_N[l])
    #     layer_tmp[l].append(sub_conv_M[l])
    #     layer_tmp[l].append(sub_conv_r[l])
    #     layer_tmp[l].append(sub_conv_R[l])
    #     layer_tmp[l].append(sub_conv_K[l])
    #     layer_tmp[l].append(sub_conv_S[l])
    #
    # for l in range(0, len(sub_conv_N), 1):
    #     layer_tmp[l] = np.array(layer_tmp[l], dtype=float)

    sub_conv_N = np.array(sub_conv_N, dtype=int)
    sub_conv_M = np.array(sub_conv_M, dtype=int)
    sub_conv_r = np.array(sub_conv_r, dtype=int)
    sub_conv_R = np.array(sub_conv_R, dtype=int)
    sub_conv_K = np.array(sub_conv_K, dtype=int)
    sub_conv_S = np.array(sub_conv_S, dtype=int)

    X = np.stack((sub_conv_N, sub_conv_M, sub_conv_r, sub_conv_R, sub_conv_K, sub_conv_S))
    #
    # for l in range(0, len(sub_conv_N), 1):
    #     X = np.stack((layer_tmp[l]))

    # http://scikit-learn.org/stable/modules/generated/sklearn.cluster.KMeans.html
    Y = np.transpose(X)

    kmeans = KMeans(n_clusters=K, random_state=0).fit(Y)

    print Y
    print Y.shape
    print kmeans.labels_
    # print kmeans.predict([[0, 0, 0, 0, 0, 0, 0, 0, 0, 0]])
    # print kmeans.cluster_centers_

    return kmeans
