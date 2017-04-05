
#ifndef _GET_CONFIG_PARAMS_H_
#define _GET_CONFIG_PARAMS_H_

#include <iostream>

void get_config_params(string config_params_file) {
	ifstream infile(config_params_file);
	string temp;
	int count = 0;
	int idx = 0;
	int num_idx = 0;
	char *buf = new char[1024];
	bool has_nn_in_data_size_conv = false;
	bool has_nn_in_number_conv = false;
	bool has_nn_out_number_conv = false;
	bool has_nn_channel_size_conv = false;

	bool has_nn_in_data_size_pooling = false;
	bool has_nn_in_number_pooling = false;
	bool has_nn_channel_size_pooling = false;

	bool has_nn_in_data_size_fc = false;
	bool has_nn_in_number_fc = false;
	bool has_nn_out_number_fc = false;
	bool has_nn_channel_size_fc = false;

	while (getline(infile, temp))
	{
		stringstream ss(temp);
		string word;
		while (ss >> word) {
			if (num_idx==0&&word=="nn_in_data_size_conv:") {
				has_nn_in_data_size_conv = true;
			}
			else if (num_idx == 0 && word == "nn_in_number_conv:") {
				has_nn_in_number_conv = true;
			}
			else if (num_idx == 0 && word == "nn_out_number_conv:") {
				has_nn_out_number_conv = true;
			}
			else if (num_idx == 0 && word == "nn_channel_size_conv:") {
				has_nn_channel_size_conv = true;
			}

			else if (num_idx == 0 && word == "nn_in_data_size_pooling:") {
				has_nn_in_data_size_pooling = true;
			}
			else if (num_idx == 0 && word == "nn_in_number_pooling:") {
				has_nn_in_number_pooling = true;
			}
			else if (num_idx == 0 && word == "nn_channel_size_pooling:") {
				has_nn_channel_size_pooling = true;
			}

			else if (num_idx == 0 && word == "nn_in_data_size_fc:") {
				has_nn_in_data_size_fc = true;
			}
			else if (num_idx == 0 && word == "nn_in_number_fc:") {
				has_nn_in_number_fc = true;
			}
			else if (num_idx == 0 && word == "nn_out_number_fc:") {
				has_nn_out_number_fc = true;
			}
			else if (num_idx == 0 && word == "nn_channel_size_fc:") {
				has_nn_channel_size_fc = true;
			}

			if (has_nn_in_data_size_conv) {
				if (num_idx>0) {
					nn_in_data_size_conv[idx] = (int)atof(word.c_str());
					idx++;
				}
			}
			else if(has_nn_in_number_conv) {
				if (num_idx>0) {
					nn_in_number_conv[idx] = (int)atof(word.c_str());
					idx++;
				}
			}
			else if (has_nn_out_number_conv) {
				if (num_idx>0) {
					nn_out_number_conv[idx] = (int)atof(word.c_str());
					idx++;
				}
			}
			else if (has_nn_channel_size_conv) {
				if (num_idx>0) {
					nn_channel_size_conv[idx] = (int)atof(word.c_str());
					idx++;
				}
			}
			else if (has_nn_in_data_size_pooling) {
				if (num_idx>0) {
					nn_in_data_size_pooling[idx] = (int)atof(word.c_str());
					idx++;
				}
			}
			else if (has_nn_in_number_pooling) {
				if (num_idx>0) {
					nn_in_number_pooling[idx] = (int)atof(word.c_str());
					idx++;
				}
			}
			else if (has_nn_channel_size_pooling) {
				if (num_idx>0) {
					nn_channel_size_pooling[idx] = (int)atof(word.c_str());
					idx++;
				}
			}
			else if (has_nn_in_data_size_fc) {
				if (num_idx>0) {
					nn_in_data_size_fc[idx] = (int)atof(word.c_str());
					idx++;
				}
			}
			else if (has_nn_in_number_fc) {
				if (num_idx>0) {
					nn_in_number_fc[idx] = (int)atof(word.c_str());
					idx++;
				}
			}
			else if (has_nn_out_number_fc) {
				if (num_idx>0) {
					nn_out_number_fc[idx] = (int)atof(word.c_str());
					idx++;
				}
			}
			else if (has_nn_channel_size_fc) {
				if (num_idx>0) {
					nn_channel_size_fc[idx] = (int)atof(word.c_str());
					idx++;
				}
			}
			num_idx++;
		}
		idx = 0;
		num_idx = 0;
		has_nn_in_data_size_conv = false;
		has_nn_in_number_conv = false;
		has_nn_out_number_conv = false;
		has_nn_channel_size_conv = false;

		has_nn_in_data_size_pooling = false;
		has_nn_in_number_pooling = false;
		has_nn_channel_size_pooling = false;

		has_nn_in_data_size_fc = false;
		has_nn_in_number_fc = false;
		has_nn_out_number_fc = false;
		has_nn_channel_size_fc = false;
		//if (count == 1) {
		//	while (ss >> word) {
		//		if (num_idx>0) {
		//			nn_in_number_conv[idx] = (int)atof(word.c_str());
		//			idx++;
		//		}
		//		num_idx++;
		//	}
		//	idx = 0;
		//	num_idx = 0;
		//}
		//if (count == 2) {
		//	while (ss >> word) {
		//		if (num_idx>0) {
		//			nn_out_number_conv[idx] = (int)atof(word.c_str());
		//			idx++;
		//		}
		//		num_idx++;
		//	}
		//	idx = 0;
		//	num_idx = 0;
		//}
		//if (count == 3) {
		//	while (ss >> word) {
		//		if (num_idx>0) {
		//			nn_channel_size_conv[idx] = (int)atof(word.c_str());
		//			idx++;
		//		}
		//		num_idx++;
		//	}
		//	idx = 0;
		//	num_idx = 0;
		//}
		//if (count == 4) {
		//	while (ss >> word) {
		//		if (num_idx>0) {
		//			nn_in_data_size_pooling[idx] = (int)atof(word.c_str());
		//			idx++;
		//		}
		//		num_idx++;
		//	}
		//	idx = 0;
		//	num_idx = 0;
		//}
		//if (count == 5) {
		//	while (ss >> word) {
		//		if (num_idx>0) {
		//			nn_in_number_pooling[idx] = (int)atof(word.c_str());
		//			idx++;
		//		}
		//		num_idx++;
		//	}
		//	idx = 0;
		//	num_idx = 0;
		//}
		//if (count == 6) {
		//	while (ss >> word) {
		//		if (num_idx>0) {
		//			nn_channel_size_pooling[idx] = (int)atof(word.c_str());
		//			idx++;
		//		}
		//		num_idx++;
		//	}
		//	idx = 0;
		//	num_idx = 0;
		//}
		//if (count == 7) {
		//	while (ss >> word) {
		//		if (num_idx>0) {
		//			nn_in_data_size_fc[idx] = (int)atof(word.c_str());
		//			idx++;
		//		}
		//		num_idx++;
		//	}
		//	idx = 0;
		//	num_idx = 0;
		//}
		//if (count == 8) {
		//	while (ss >> word) {
		//		if (num_idx>0) {
		//			nn_in_number_fc[idx] = (int)atof(word.c_str());
		//			idx++;
		//		}
		//		num_idx++;
		//	}
		//	idx = 0;
		//	num_idx = 0;
		//}
		//if (count == 9) {
		//	while (ss >> word) {
		//		if (num_idx>0) {
		//			nn_out_number_fc[idx] = (int)atof(word.c_str());
		//			idx++;
		//		}
		//		num_idx++;
		//	}
		//	idx = 0;
		//	num_idx = 0;
		//}
		//if (count == 10) {
		//	while (ss >> word) {
		//		if (num_idx>0) {
		//			nn_channel_size_fc[idx] = (int)atof(word.c_str());
		//			idx++;
		//		}
		//		num_idx++;
		//	}
		//	idx = 0;
		//	num_idx = 0;
		//}
		///*if (count == 11) {
		//	while (ss >> word) {
		//		if (num_idx>0) {
		//			nn_local_size_lrn[idx] = (int)atof(word.c_str());
		//			idx++;
		//		}
		//		num_idx++;
		//	}
		//	idx = 0;
		//	num_idx = 0;
		//}
		//if (count == 12) {
		//	while (ss >> word) {
		//		if (num_idx>0) {
		//			alpha[idx] = atof(word.c_str());
		//			idx++;
		//		}
		//		num_idx++;
		//	}
		//	idx = 0;
		//	num_idx = 0;
		//}
		//if (count == 13) {
		//	while (ss >> word) {
		//		if (num_idx>0) {
		//			beta[idx] = atof(word.c_str());
		//			idx++;
		//		}
		//		num_idx++;
		//	}
		//	idx = 0;
		//	num_idx = 0;
		//}*/
		count++;
	}
}

#endif 
