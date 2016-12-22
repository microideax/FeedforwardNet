//This layer deal with the arithmatic processure of the network

#ifndef _ARITHMATIC_LAYER_H_
#define _ARITHMATIC_LAYER_H_

#include "data_type.h"

// rescale output to 0-100
double rescale(double x) {
    Activation a;
    return 100.0 * (x - a.scale().first) / (a.scale().second - a.scale().first);
}

/*
//�����������֣�����
void print_score(vec_t res)
{
vector<pair<double, int> > scores;
// sort & print top-3
for (int i = 0; i < 10; i++)
scores.emplace_back(rescale<tan_h>(res[i]), i);
//�����ʴӴ�С����
sort(scores.begin(), scores.end(), greater<pair<double, int>>());
for (int i = 0; i < 10; i++)
cout << scores[i].second << "," << scores[i].first << endl;
getchar();
}
*/

#endif
