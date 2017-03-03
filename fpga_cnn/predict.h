
#ifndef __PREDICT_H__
#define __PREDICT_H__

#include <iostream>

template<int size, int label_size>
void predict(float (&x)[size][label_size], float(&y)[label_size]) {
	for (int i = 0; i < size; i++)
	{
		int predict_label = 0;
		float max = 0;
		for (int j = 0; j < label_size; j++) {
			if (x[i][j]>max) {
				predict_label = j;
				max = x[i][j];
			}
		}
		/*cout << "finished predict ...." << endl;
		ofstream predict_a;
		predict_a.open("predict_a.txt", ios::app);
		predict_a << "predicted class and probabilities is:  " << y[predict_label] << "  " << max << endl;
		predict_a << endl;
		predict_a.close();
		cout << endl;*/
	}
}
#endif
