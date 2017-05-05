
#ifndef _GET_CONFIG_PARAMS_H_
#define _GET_CONFIG_PARAMS_H_

#include <iostream>

void get_config_params(string config_params_file) {
	ifstream infile(config_params_file);
	string temp;
	int idx = 0;
	int num_idx = 0;
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

	bool has_nn_local_size_lrn = false;
	bool has_nn_alpha_lrn = false;
	bool has_nn_beta_lrn = false;
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

			else if (num_idx == 0 && word == "nn_local_size_lrn:") {
				has_nn_local_size_lrn = true;
			}
			else if (num_idx == 0 && word == "nn_alpha_lrn:") {
				has_nn_alpha_lrn = true;
			}
			else if (num_idx == 0 && word == "nn_beta_lrn:") {
				has_nn_beta_lrn = true;
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
			else if (has_nn_local_size_lrn) {
				if (num_idx>0) {
					nn_local_size_lrn[idx] = (int)atof(word.c_str());
					idx++;
				}
			}
			else if (has_nn_alpha_lrn) {
				if (num_idx>0) {
					nn_alpha_lrn[idx] = atof(word.c_str());
					idx++;
				}
			}
			else if (has_nn_beta_lrn) {
				if (num_idx>0) {
					nn_beta_lrn[idx] = atof(word.c_str());
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

		has_nn_local_size_lrn = false;
		has_nn_alpha_lrn = false;
		has_nn_beta_lrn = false;
	}
}

#endif 
