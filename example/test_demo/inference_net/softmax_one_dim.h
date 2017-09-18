
#ifndef _SOFTMAX_H_
#define _SOFTMAX_H_

#include <iostream>

#if _KERNEL_DEBUG_
//template<int size, int label_size>
void softmax(float x[],int label_size) {//for a image
		float max = 0.0;
		float sum = 0.0;
		for (int j = 0; j < label_size; j++) {
			if (max < x[j]) {
				max = x[j];
			}
		}
		for (int j = 0; j < label_size; j++) {
			x[j] = exp(x[j] - max);
			sum += x[j];
		}

		for (int j = 0; j < label_size; j++) {
			x[j] /= sum;
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
//template<int num, int size, int label_size>
//void softmax(float x[], int num, int label_size) {//for imagenet dataset
//	for (int i = 0; i < num; i++) {
//		float max = 0.0;
//		float sum = 0.0;
//			for (int j = 0; j < label_size; j++) {
//				if (max < x[i * label_size + j]) {
//					max = x[i * label_size + j];
//				}
//			}
//			for (int j = 0; j < label_size; j++) {
//				x[i * label_size + j] = exp(x[i * label_size + j] - max);
//				sum += x[i * label_size + j];
//			}

//			for (int j = 0; j < label_size; j++) {
//				x[i * label_size + j] /= sum;
//			}
//	}
//}

//template<int size, int label_size>
void softmax(float x[], int size, int label_size) {//for mnist dataset
	for (int i = 0; i < size; i++) {
		float max = 0.0;
		float sum = 0.0;
		for (int j = 0; j < label_size; j++) {
			if (max < x[i * label_size + j]) {
				max = x[i * label_size + j];
			}
		}
		for (int j = 0; j < label_size; j++) {
			x[i * label_size + j] = exp(x[i * label_size + j] - max);
			sum += x[i * label_size + j];
		}

		for (int j = 0; j < label_size; j++) {
			x[i * label_size + j] /= sum;
		}
	}
}
#endif


#endif // end of the header file
