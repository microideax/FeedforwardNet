
#ifndef __PREDICT_H__
#define __PREDICT_H__

#include <iostream>

#if _KERNEL_DEBUG_
//template<int size, int label_size>
void predict(float x[], int label_size) {
		int predict_label = 0;
		float max = 0;
		for (int j = 0; j < label_size; j++) {
			if (x[j]>max) {
				predict_label = j;
				max = x[j];
			}
		}
		cout << "finished predict ...." << endl;
		ofstream predict_a;
		predict_a.open("predict_a.txt", ios::app);
		predict_a << "predicted class and probabilities is:  " << predict_label << "  " << max << endl;
		cout << "predicted class and probabilities is: " << predict_label << "  " << max << endl;
		predict_a << endl;
		predict_a.close();
		cout << endl;
}

#endif

#if _BATCH_MODE_
//template<int num, int size, int label_size>
void predict(float x[], int size, int label_size) {//for imagenet dataset
	for (int i = 0; i < size; i++)
	{
		int predict_label = 0;
		float max = 0;
		for (int j = 0; j < label_size; j++) {
			if (x[i * label_size + j]>max) {
				predict_label = j;
				max = x[i * label_size + j];
			}
		}
		cout << "finished predict ...." << endl;
		ofstream predict_a;
		predict_a.open("predict_a.txt", ios::app);
		predict_a << "predicted class and probabilities is:  " << predict_label << "  " << max << endl;
		predict_a << endl;
		predict_a.close();
		cout << endl;
	}
}
//template<int size, int label_size>
void predict(float x[], float y[], int size, int label_size) {//for mnist dataset
	for (int i = 0; i < size; i++)
	{
		int predict_label = 0;
		float max = 0;
		for (int j = 0; j < label_size; j++) {
			if (x[i * label_size + j]>max) {
				predict_label = j;
				max = x[i * label_size + j];
			}
		}
//		cout << "finished predict ...." << endl;
		ofstream predict_a;
		predict_a.open("predict_a.txt", ios::app);
		predict_a << "predicted class and probabilities is:  " << y[predict_label] << "  " << max << endl;
		predict_a << endl;
		predict_a.close();
	}
//	cout << endl;
}
#endif
#endif
