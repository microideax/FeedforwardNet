
#ifndef _SOFTMAX_H_
#define _SOFTMAX_H_

#include <iostream>

#if _KERNEL_DEBUG_
template<int size, int label_size>
void softmax(float(&x)[label_size][size][size]) {//for a image
		float max = 0.0;
		float sum = 0.0;
		for (int j = 0; j < label_size; j++) {
			if (max < x[j][0][0]) {
				max = x[j][0][0];
			}
		}
		for (int j = 0; j < label_size; j++) {
			x[j][0][0] = exp(x[j][0][0] - max);
			sum += x[j][0][0];
		}

		for (int j = 0; j < label_size; j++) {
			x[j][0][0] /= sum;
		}
		/*cout << "finished softmax ...." << endl;
		ofstream softmax_a;
		softmax_a.open("softmax_a.txt", ios::app);
		for (int j = 0; j < label_size; j++) {
		softmax_a << x[i][j] << " ";
		}
		softmax_a << endl;
		softmax_a << endl;
		softmax_a.close();
		cout << endl;*/
}
#endif

#if _BATCH_MODE_
template<int size, int label_size>
void softmax(float(&x)[size][label_size]) {//for data set
	for (int i = 0; i < size; i++) {
		float max = 0.0;
		float sum = 0.0;
		for (int j = 0; j < label_size; j++) {
			if (max < x[i][j]) {
				max = x[i][j];
			}
		}
		for (int j = 0; j < label_size; j++) {
			x[i][j] = exp(x[i][j] - max);
			sum += x[i][j];
		}

		for (int j = 0; j < label_size; j++) {
			x[i][j] /= sum;
		}
		/*cout << "finished softmax ...." << endl;
		ofstream softmax_a;
		softmax_a.open("softmax_a.txt", ios::app);
		for (int j = 0; j < label_size; j++) {
		softmax_a << x[i][j] << " ";
		}
		softmax_a << endl;
		softmax_a << endl;
		softmax_a.close();
		cout << endl;*/
	}
}
#endif
#endif 
