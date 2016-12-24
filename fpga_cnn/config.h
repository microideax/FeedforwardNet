// This file contains the global configurations of the feedforward net.
// Created by yaochen on 20/12/16.
//TODO: add all the global configurations in.

#ifndef _CONFIG_H_
#define _CONFIG_H_

// C++ compilation debug mode
// HLS mode

#define _C_DEBUG_MODE_  1

#define _HLS_MODE_      0

#define O true
#define X false
const bool tbl[6][16] = {
        O, X, X, X, O, O, O, X, X, O, O, O, O, X, O, O,
        O, O, X, X, X, O, O, O, X, X, O, O, O, O, X, O,
        O, O, O, X, X, X, O, O, O, X, X, O, X, O, O, O,
        X, O, O, O, X, X, O, O, O, O, X, X, O, X, O, O,
        X, X, O, O, O, X, X, O, O, O, O, X, O, O, X, O,
        X, X, X, O, O, O, X, X, O, O, O, O, X, O, O, O };
#undef O
#undef X

#endif //FFNET_CONFIG_H
