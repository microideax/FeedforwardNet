//This layer deal with the arithmatic processure of the network

#ifndef _ARITHMATIC_LAYER_H_
#define _ARITHMATIC_LAYER_H_

// rescale output to 0-100
double rescale(double x) {
    Activation a;
    return 100.0 * (x - a.scale().first) / (a.scale().second - a.scale().first);
}

#endif
