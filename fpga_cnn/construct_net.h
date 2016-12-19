//
// Created by yaochen on 19/12/16.
//

#ifndef _CONSTRUCT_NET_H
#define _CONSTRUCT_NET_H


/*
//void construct_net(network<sequential>& nn) {
void construct_net(network<sequential>& nn) {
// connection table [Y.Lecun, 1998 Table.1]
#define O true
#define X false
static const bool tbl[] = {
O, X, X, X, O, O, O, X, X, O, O, O, O, X, O, O,
O, O, X, X, X, O, O, O, X, X, O, O, O, O, X, O,
O, O, O, X, X, X, O, O, O, X, X, O, X, O, O, O,
X, O, O, O, X, X, O, O, O, O, X, X, O, X, O, O,
X, X, O, O, O, X, X, O, O, O, O, X, O, O, X, O,
X, X, X, O, O, O, X, X, O, O, O, O, X, O, O, O
};
#undef O
#undef X

// construct nets
nn << convolutional_layer<tan_h>(32, 32, 5, 1, 6)  // C1, 1@32x32-in, 6@28x28-out
<< average_pooling_layer<tan_h>(28, 28, 6, 2)   // S2, 6@28x28-in, 6@14x14-out
<< convolutional_layer<tan_h>(14, 14, 5, 6, 16,
connection_table(tbl, 6, 16))              // C3, 6@14x14-in, 16@10x10-out
<< average_pooling_layer<tan_h>(10, 10, 16, 2)  // S4, 16@10x10-in, 16@5x5-out
<< convolutional_layer<tan_h>(5, 5, 5, 16, 120) // C5, 16@5x5-in, 120@1x1-out
<< fully_connected_layer<tan_h>(120, 10);       // F6, 120-in, 10-out
}

*/

#endif //FFNET_CONSTRUCT_NET_H
