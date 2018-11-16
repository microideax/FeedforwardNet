
#include <iostream>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <malloc.h>
#include "inference_net/config.h"
#include "inference_net/construct_net.h"
#include "inference_net/image_converter.h"
#include "inference_net/weight_bias_one_dim.h"
#include "inference_net/softmax_one_dim.h"
#include "inference_net/predict_one_dim.h"
#include "inference_net/accuracy_one_dim.h"
#include "inference_net/pow_function.h"
#include "inference_net/resize_image.h"
#include "inference_net/print_array.h"

#include "ap_fixed.h"


using namespace std;

const unsigned char * loadfile(const std::string &file, int &size) {
   std::ifstream fs(file.c_str(), std::ios::binary);
   fs.seekg(0, std::ios::end);
   size = fs.tellg();
   char * data = new char[size];
   fs.seekg(0);
   fs.read(data, sizeof(char) * size);
   fs.close();
   return (unsigned char *)data;
}

int main() {

   cout<< "Calculating memory space ... ... ... ..." << endl;

   // data size calculation
   unsigned int conv_weight_size = (2550) * sizeof(data_type_w);
   unsigned int conv_bias_size = (22) * sizeof(data_type_w);
   unsigned int fc_weight_size = (4000) * sizeof(data_type_w);
   unsigned int fc_bias_size = (10) * sizeof(data_type_w);
   unsigned int fc_3_out_size = (10*2) * sizeof(data_type_o);
   unsigned int out_size_0_1 = (1024) * sizeof(data_type_o);
   unsigned int out_size_1_1 = (1024) * sizeof(data_type_o);
   unsigned int ap_data_num = (1024) * sizeof(data_type_itf);

   // assign memory space to different ports

    data_type_itf *i_data_tmp = (data_type_itf*)malloc(ap_data_num);
    data_type_itf *o_data_tmp = (data_type_itf*)malloc(ap_data_num);
    data_type_itf *i_weight   = (data_type_itf*)malloc(conv_weight_size);

   data_type_w *conv_weight_mem_port = (data_type_w*)malloc(conv_weight_size);
   if (conv_weight_mem_port == NULL) {
      printf("False memory allocation of conv_weight_mem_port\n");
   }
   ap_fixed<32,26> *conv_bias_mem_port = (ap_fixed<32,26>*)malloc(conv_bias_size);
   if (conv_bias_mem_port == NULL) {
      printf("False memory allocation of conv_bias_mem_port\n");
   }
   data_type_w *fc_weight_mem_port = (data_type_w*)malloc(fc_weight_size);
   if (fc_weight_mem_port == NULL) {
      printf("False memory allocation of fc_weight_mem_port\n");
   }
   data_type_w *fc_bias_mem_port = (data_type_w*)malloc(fc_bias_size);
   if (fc_bias_mem_port == NULL) {
      printf("False memory allocation of fc_bias_mem_port\n");
   }
   data_type_o *fc_3_out_mem_int = (data_type_o*)malloc(fc_3_out_size);
   if (fc_3_out_mem_int == NULL) {
      printf("False memory allocation of fc_out_mem_int\n");
   }

   data_type_o *temp_out_0_1 = (data_type_o *)malloc(out_size_0_1);
   if (temp_out_0_1 == NULL) {
      printf("False memory allocation of temp_out_0_1\n");
   }

    data_type_o *temp_out_0_2 = (data_type_o *)malloc(out_size_0_1);
    if (temp_out_0_2 == NULL) {
        printf("False memory allocation of temp_out_0_2\n");
    }

    data_type_o *temp_out_0_3 = (data_type_o *)malloc(out_size_0_1);
    if (temp_out_0_3 == NULL) {
        printf("False memory allocation of temp_out_0_3\n");
    }

    data_type_o *temp_out_0_4 = (data_type_o *)malloc(out_size_0_1);
    if (temp_out_0_4 == NULL) {
        printf("False memory allocation of temp_out_0_4\n");
    }
   data_type_o *temp_out_1_1 = (data_type_o *)malloc(out_size_1_1);
   if (temp_out_1_1 == NULL) {
      printf("False memory allocation of temp_out_1_1\n");
   }

    ////////////////////////////////////////////////////////////////////
    data_type_o *data_o_0 = (data_type_o *)malloc(out_size_0_1);
    if (data_o_0 == NULL) {
        printf("False memory allocation of data_o_0\n");
    }
    data_type_o *data_o_1 = (data_type_o *)malloc(out_size_0_1);
    if (data_o_1 == NULL) {
        printf("False memory allocation of data_o_1\n");
    }
    data_type_o *data_o_2 = (data_type_o *)malloc(out_size_0_1);
    if (data_o_2 == NULL) {
        printf("False memory allocation of data_o_2\n");
    }
    data_type_o *data_o_3 = (data_type_o *)malloc(out_size_0_1);
    if (data_o_3 == NULL) {
        printf("False memory allocation of data_o_3\n");
    }
    data_type_o *data_o_4 = (data_type_o *)malloc(out_size_0_1);
    if (data_o_4 == NULL) {
        printf("False memory allocation of data_o_4\n");
    }
    data_type_o *data_o_5 = (data_type_o *)malloc(out_size_0_1);
    if (data_o_5 == NULL) {
        printf("False memory allocation of data_o_5\n");
    }
    data_type_o *data_o_6 = (data_type_o *)malloc(out_size_0_1);
    if (data_o_6 == NULL) {
        printf("False memory allocation of data_o_6\n");
    }
    data_type_o *data_o_7 = (data_type_o *)malloc(out_size_0_1);
    if (data_o_7 == NULL) {
        printf("False memory allocation of data_o_7\n");
    }


    data_type_o *inf_1_o_0 = (data_type_o *)malloc(256);
    data_type_o *inf_1_o_1 = (data_type_o *)malloc(256);
    data_type_o *inf_1_o_2 = (data_type_o *)malloc(256);
    data_type_o *inf_1_o_3 = (data_type_o *)malloc(256);
    data_type_o *inf_1_o_4 = (data_type_o *)malloc(256);
    data_type_o *inf_1_o_5 = (data_type_o *)malloc(256);
    data_type_o *inf_1_o_6 = (data_type_o *)malloc(256);
    data_type_o *inf_1_o_7 = (data_type_o *)malloc(256);

#if _KERNEL_DEBUG_
   cout << "FC mem init\n";
   memset(fc_3_out_mem_int, 0, fc_3_out_size);
   memset(temp_out_0_1, 0, out_size_0_1);
   memset(temp_out_1_1, 0, out_size_1_1);
#endif

   //net weight src *****************************
#if _HLS_MODE_
   const char* weight_src = "net_weights.txt";
#else
   const char* weight_src = "net_inputs/net_weights.txt";
#endif
#if _KERNEL_DEBUG_
#if _HLS_MODE_
   string image_dir = "3.bmp";
#else
   string image_dir = "./net_inputs/test_imgs/3.bmp";
#endif
   int w;
   int h;
   int channels;
   int size;
   const unsigned char * data = loadfile(image_dir, size);
   const unsigned char * image_orig = stbi_load_from_memory(data, size, &w, &h, &channels, 1);
   int in_data_size=0;
   ofstream indata;
   indata.open("in_data.txt");
   for (int i = 0; i < 1; i++) {
      for (int j = 0; j < 28; j++) {
         for (int k = 0; k < 28; k++) {
            indata << image_orig[i *28*28 + 28*j + k] << " ";
         }

         indata << endl;
      }

      indata << endl;
   }
   indata.close();

   cout << "Writing data to input data memory space ... ... ..." << endl;
   float image_pixel_buf;
   for (int i = 0; i < 1; i++) {
      for (int j = 0; j < 28; j++) {
         for (int k = 0; k < 28; k++) {
             image_pixel_buf = image_orig[i*28*28 + 28*j + k];
             temp_out_0_1[in_data_size] = image_pixel_buf;
             in_data_size++;
             cout << temp_out_0_1[i*28*28 + j*28 + k] << " ";
         }
cout << endl;
      }
cout << endl;
   }

   cout << "Finished writing data to input data memory space ... ..." << endl;
#endif

   char tan_h = 't';
   char relu = 'r';
   char none = 'i';
   int in_number_conv = 0;
   int in_number_fc = 0;
   int in_number_pooling = 0;

   // Prepare weights and bias for conv layer 1
   float *conv_1_weight2D = (float*)malloc(150 * sizeof(float));
   memset(conv_1_weight2D, 0, 150 * sizeof(float));
   load_weight_conv(
weight_src, 
conv_1_weight2D,
 weight_bias_record,
 nn_channel_size_conv, 
 nn_in_number_conv,
 nn_out_number_conv,
 in_number_conv);
   int conv_weight_num=0;
   cout << "Loading conv weight 1 to memory space, starting at: " <<conv_weight_num << '\n';
   for (int i = 0; i < 150; i++) {
      conv_weight_mem_port[conv_weight_num] = (data_type_w)conv_1_weight2D[i];
      conv_weight_num++;
   }
   free(conv_1_weight2D);
   float *conv_1_bias2D = (float*)malloc(6 * sizeof(float));
   memset(conv_1_bias2D, 0, 6 * sizeof(float));
   load_bias_conv(
weight_src, 
conv_1_bias2D,
 weight_bias_record,
 nn_channel_size_conv, 
 nn_in_number_conv,
 nn_out_number_conv,
 in_number_conv);
   int conv_bias_num=0;
   cout << "Loading conv bias 1 to memory space, starting at: " <<conv_bias_num << '\n';
   for (int i = 0; i < 6; i++) {
      conv_bias_mem_port[conv_bias_num] = (data_type_w)conv_1_bias2D[i];
//       ap_fixed<16, 10> b = 0;
//       conv_bias_mem_port[conv_bias_num] = b.range(15, 0);
      conv_bias_num++;
   }
   free(conv_1_bias2D);
   in_number_conv++;
    cout << "layer bias data: " << endl;
    for (int i = 0; i < 6; i++){
        cout << conv_bias_mem_port[i] << " ";
    }
    cout << endl;
  // Prepare weights and bias for conv layer 2
  float *conv_2_weight2D = (float*)malloc(2400 * sizeof(float));
  memset(conv_2_weight2D, 0, 2400 * sizeof(float));
  load_weight_conv(
weight_src,
conv_2_weight2D,
weight_bias_record,
nn_channel_size_conv,
nn_in_number_conv,
nn_out_number_conv,
in_number_conv);
  cout << "Loading conv weight 2 to memory space, starting at: " <<conv_weight_num << '\n';
  for (int i = 0; i < 2400; i++) {
     conv_weight_mem_port[conv_weight_num] = (data_type_w)conv_2_weight2D[i];
     conv_weight_num++;
  }
  free(conv_2_weight2D);
  float *conv_2_bias2D = (float*)malloc(16 * sizeof(float));
  memset(conv_2_bias2D, 0, 16 * sizeof(float));
  load_bias_conv(
weight_src,
conv_2_bias2D,
weight_bias_record,
nn_channel_size_conv,
nn_in_number_conv,
nn_out_number_conv,
in_number_conv);
  cout << "Loading conv bias 2 to memory space, starting at: " <<conv_bias_num << '\n';
  for (int i = 0; i < 16; i++) {
     conv_bias_mem_port[conv_bias_num] = (data_type_w)conv_2_bias2D[i];
     conv_bias_num++;
  }
  free(conv_2_bias2D);
  in_number_conv++;

  cout<<"Finished loading conv weight into memory! Total: " <<conv_weight_num  << "... ... ..."<<endl;
  cout<<"Finished loading conv bias into memory! Total: " <<conv_bias_num  << "... ... ..."<<endl;

  // Prepare weights and bias for fc layer 1
  float *fc_1_weight2D = (float*)malloc(4000 * sizeof(float));
  memset(fc_1_weight2D, 0, 4000 * sizeof(float));
  load_weight_fc(
weight_src,
fc_1_weight2D,
weight_bias_record,
nn_channel_size_fc,
nn_in_number_fc,
nn_out_number_fc,
in_number_fc);
  int fc_weight_num=0;
  cout << "Loading fc weight 1 to memory space, starting at: " <<fc_weight_num << '\n';
    data_type_w fc_w = 0;
  for (int i = 0; i < 4000; i++) {
      fc_weight_mem_port[fc_weight_num] =fc_1_weight2D[i];
      fc_weight_num++;
//      cout << fc_weight_mem_port[i] << " ";
  }
    cout << endl;
  free(fc_1_weight2D);
  float *fc_1_bias2D = (float*)malloc(10 * sizeof(float));
  memset(fc_1_bias2D, 0, 10 * sizeof(float));
  load_bias_fc(
weight_src,
fc_1_bias2D,
weight_bias_record,
nn_channel_size_fc,
nn_in_number_fc,
nn_out_number_fc,
in_number_fc);
  int fc_bias_num=0;
  cout << "Loading fc bias 1 to memory space, starting at: " <<fc_bias_num << '\n';
  for (int i = 0; i < 10; i++) {
     fc_bias_mem_port[fc_bias_num] = (data_type_w)fc_1_bias2D[i];
     fc_bias_num++;
  }
  free(fc_1_bias2D);
  in_number_fc++;

  cout<<"Finished loading fc weight into memory! Total: " <<fc_weight_num  << "... ... ..."<<endl;
  cout<<"Finished loading fc bias into memory! Total: " <<fc_bias_num  << "... ... ..."<<endl;

#if _KERNEL_DEBUG_
  float fc_3_out[10] = { 0 };
  clock_t start, finish, inf_start, inf_finish;
  double totaltime, inf_time;
  start = clock();
#endif

/*
    cout << "Input test data in ap_fixed<512>:" << endl;
    data_type input = 0;
    for(int ch = 0; ch < 1; ch++) {
        for (int i = 0; i < 28; i++) {
            for (int j = 0; j < 28; j++) {
                for (int wd = 0; wd <= ch && wd < 32; wd++) {
                    i_data_tmp[i * 28 + j].range((wd + 1) * 16 - 1, wd * 16) = input.range(15, 0);
//                            temp_out_0_1[i * 28 + j].range(15,0);
#if _HLS_MODE_
                    cout << temp_out_0_1[i * 28 + j] << " ";
#else
                    cout << setw(3) << input << " ";
                    input++;
#endif
                }
            }
            cout << endl;
        }
        cout << endl;
    }
    cout << endl;
*/
/*
    cout << "Loading layer 0 weight data ... ... ..." << endl;
    ap_fixed<16,10> tmp = 0;
    for (int i = 0; i < 1; i++){
        for (int j = 0; j < 1; j++){
            for (int k1 = 0; k1 < 1; k1++){
                for (int k2 = 0; k2 < 1; k2++){
                    for(int ch = 0; ch <= i; ch++){
//                        ap_fixed<16,10> w = conv_weight_mem_port[j*5*5 + k1*5 + k2];
                        ap_fixed<16,10> w = 1;
                        i_weight[j*5*5 + k1*5 + k2].range(15, 0) = w.range(15, 0);
                    }
                }
            }
        }
    }
*/
//    data_type_w  tmp_weight_in[150] = {1,1,1,1,1,1,1,1,0};

    squeeze_input("Input data array", 1, 28, 28, temp_out_0_1, i_data_tmp, 1);
//    print_array_3d("Input data array print()", 1, 28, 28, i_data_tmp);
    squeeze_weight("1st conv layer weight", 1, 6, 5, conv_weight_mem_port, i_weight, 1);


    Tparam param_conv_0[16] = { 1, 5, 6, 28, 28, 28, 28, 1, 2, 1, 0, 0, 0, 0, 0, 0};
    Tparam param_pool_0[16] = { 28, 28,  6, 2, 14, 14, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0};

   //conv layer 0 process
    inf_net_0(
        param_conv_0,
        param_pool_0,
        conv_bias_mem_port,
        i_weight,
        i_data_tmp,
        o_data_tmp
    );
//    print_array_3d("conv layer 0 output", param_conv_0[2], param_conv_0[5], param_conv_0[6], o_data_tmp);
/*
    param_pool_0[15] = 1;
    //maxpool layer 0 process
    inf_net_0(
            param_conv_0,
            param_pool_0,
            conv_bias_mem_port,
            i_weight,
            o_data_tmp,
            i_data_tmp
    );
//    print_array_3d("max_pool layer 0 output", param_pool_0[2], param_pool_0[4], param_pool_0[5], i_data_tmp);

    squeeze_weight("2nd conv layer weight", 6, 16, 5, conv_weight_mem_port+150, i_weight, 0);
    Tparam param_conv_1[16] = {6, 5, 16, 14, 14, 10, 10, 1, 0, 1, 0, 0, 0, 0, 0, 0};
    Tparam param_pool_1[16] = {10, 10, 16, 2,  5,  5, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0};

    //conv layer 1 process
    inf_net_0(
            param_conv_1,
            param_pool_1,
            conv_bias_mem_port+6,
            i_weight,
            i_data_tmp,
            o_data_tmp
    );
//    print_array_3d("conv layer 1 output", param_conv_1[2], param_conv_1[5], param_conv_1[6], o_data_tmp);

    param_pool_1[15] = 1;
    //maxpool layer 1 process
    inf_net_0(
            param_conv_1,
            param_pool_1,
            conv_bias_mem_port,
            i_weight,
            o_data_tmp,
            i_data_tmp
    );

//    print_array_3d("pool layer 1 output", param_pool_1[2], param_pool_1[4], param_pool_1[5], i_data_tmp);

    squeeze_weight("1st fc layer weight", 16, 10, 5, fc_weight_mem_port, i_weight, 1);
    Tparam param_fc_0[16] = {16, 5, 10, 5, 5, 1, 1, 5, 0, 1, 0, 0, 0, 0, 0, 0};

    inf_net_0(
            param_fc_0,
            param_fc_0,
            fc_bias_mem_port,
            i_weight,
            i_data_tmp,
            o_data_tmp
    );

    print_array_3d("fc layer output", param_fc_0[2], param_fc_0[5], param_fc_0[6], o_data_tmp);
*/
/*
    cout << "MAX_POOL: output after max pool ... ... ..." << endl;
    for (int ch = 0; ch < param_pool_0[2]; ch++) {
        for (int j = 0; j < param_pool_0[4]; j++) {
            for (int i = 0; i < param_pool_0[5]; i++) {
                print_tmp.range(15,0) = i_data_tmp[j*param_pool_0[5]+i].range((ch+1)*16-1, ch*16);
#if _HLS_MODE_
                cout << print_tmp << " ";
#else
                cout << setw(3) << print_tmp << " ";
#endif
            }
            cout << endl;
        }
        cout << endl;
    }
    cout << endl;
    cout << endl;

    cout << "Loading layer 1 weight data " << endl;
    cout << "Input layer 1 weight data in ap_fixed<512>:" << endl;
    ap_fixed<16,10> weight = 0;
    for (int j = 0; j < param_conv_1[2]; j++){
        for (int k1 = 0; k1 < 5; k1++){
            for (int k2 = 0; k2 < 5; k2++){
                for (int i = 0; i < param_conv_1[0]; i++){
                    if (j < param_conv_1[2] && i < param_conv_1[0]) {
//                        weight = ap_fixed<16,10>(i * 16 *5 * 5 + j* 5 * 5 + k1 * 5 + k2);
                        weight = conv_weight_mem_port[150 + i*5*5 + j*6*5*5 + k1*5 + k2];
//                        cout<< setw(5) << weight;
                    } else {
                        weight = ap_fixed<16,10>(0);
                    }
                    i_weight[(i/32) * 16 * 5 * 5 + j * 5 * 5 + k1 * 5 + k2].range(i * 16 + 15, i * 16) = weight.range(15, 0);
                }
            }
        }
    }

    for (int line = 0; line < 4; line++){
        cout << endl;
    }

    Tparam param_conv_1[16] = {6, 5, 16, 14, 14, 10, 10, 1, 0, 1, 0, 0, 0, 0, 0, 0};
    Tparam param_pool_1[16] = { 10, 10, 16, 2,  5,  5, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0};


    // layer 1 process
    inf_net_0(
        param_conv_1,
        param_pool_1,
        conv_bias_mem_port,
        i_weight,
        o_data_tmp,
        i_data_tmp
    );

    for (int line = 0; line < 4; line++){
        cout << endl;
    }
*/
/*
    for (int ch = 0; ch < 16; ch++) {
        cout << "print output channel: " << ch << endl;
        for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 10; i++) {
                print_tmp.range(15,0) = i_data_tmp[j*10 + i].range(ch*16+15, ch*16);
#if _HLS_MODE_
                cout << print_tmp << " ";
#else
                cout << setw(3) << print_tmp << " ";
#endif
            }
            cout << endl;
        }
        cout << endl;
    }
    cout << endl;
    cout << endl;
*/

#if _C_DEBUG_MODE_
//    print_array_3d("conv layer output: ", param_conv_1[2], param_conv_1[5], param_conv_1[6], i_data_tmp);
#endif

 /*
    inf_net_1(
        conv_weight_mem_port,
        conv_weight_mem_port,
        conv_weight_mem_port,
        conv_weight_mem_port,
        conv_weight_mem_port,
        conv_weight_mem_port,
        conv_weight_mem_port,
        conv_weight_mem_port,
        conv_bias_mem_port,
        data_o_0,
        data_o_1,
        data_o_2,
        data_o_3,
        data_o_4,
        data_o_5,
        data_o_6,
        data_o_7,
        inf_1_o_0,
        inf_1_o_1,
        inf_1_o_2,
        inf_1_o_3,
        inf_1_o_4,
        inf_1_o_5,
        inf_1_o_6,
        inf_1_o_7);


    inf_net_2(
        fc_weight_mem_port,
        fc_bias_mem_port,
        inf_1_o_0,
        inf_1_o_1,
        inf_1_o_2,
        inf_1_o_3,
        inf_1_o_4,
        inf_1_o_5,
        inf_1_o_6,
        inf_1_o_7,
        temp_out_0_4);
*/
    /*
    finish = clock();
   totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
   cout <<"predicted time is: " << totaltime << " s" << endl;
   for (int i = 0; i < 10; i++) {
      fc_3_out[i]=(float)(fc_3_out_mem_int[i]);
   }
   softmax(fc_3_out, 10);
   predict(fc_3_out, 10);

    cout << "Finished prediction" << endl;
*/
   return 0;

}
