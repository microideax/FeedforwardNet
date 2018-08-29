
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
   unsigned int conv_weight_size = (2550*2) * sizeof(data_type_w);
   unsigned int conv_bias_size = (22*2) * sizeof(data_type_w);
   unsigned int fc_weight_size = (4000*2) * sizeof(data_type_w);
   unsigned int fc_bias_size = (10*2) * sizeof(data_type_w);
   unsigned int fc_3_out_size = (10*2) * sizeof(data_type_o);
   unsigned int out_size_0_1 = (1024) * sizeof(data_type_o);
   unsigned int out_size_1_1 = (1024) * sizeof(data_type_o);

   // assign memory space to different ports
   data_type_w *conv_weight_mem_port = (data_type_w*)malloc(conv_weight_size);
   if (conv_weight_mem_port == NULL) {
      printf("False memory allocation of conv_weight_mem_port\n");
   }
   else {
      printf("conv weight memory location= 0x%x \n", conv_weight_mem_port);
   }
   data_type_w *conv_bias_mem_port = (data_type_w*)malloc(conv_bias_size);
   if (conv_bias_mem_port == NULL) {
      printf("False memory allocation of conv_bias_mem_port\n");
   }
   else {
      printf("conv bias memory location= 0x%x \n", conv_bias_mem_port);
   }
   data_type_w *fc_weight_mem_port = (data_type_w*)malloc(fc_weight_size);
   if (fc_weight_mem_port == NULL) {
      printf("False memory allocation of fc_weight_mem_port\n");
   }
   else {
      printf("fc_weight_mem_port memory location= 0x%x \n", fc_weight_mem_port);
   }
   data_type_w *fc_bias_mem_port = (data_type_w*)malloc(fc_bias_size);
   if (fc_bias_mem_port == NULL) {
      printf("False memory allocation of fc_bias_mem_port\n");
   }
   else {
      printf("fc_bias_mem_port memory location= 0x%x \n", fc_bias_mem_port);
   }
   data_type_o *fc_3_out_mem_int = (data_type_o*)malloc(fc_3_out_size);
   if (fc_3_out_mem_int == NULL) {
      printf("False memory allocation of fc_out_mem_int\n");
   }
   else {
      printf("fc_out_mem_int memory location= 0x%x \n", fc_3_out_mem_int);
   }

   data_type_o *temp_out_0_1 = (data_type_o *)malloc(out_size_0_1);
   if (temp_out_0_1 == NULL) {
      printf("False memory allocation of temp_out_0_1\n");
   }
   else {
      printf("temp_out_0_1 memory location= 0x%x \n", temp_out_0_1);
   }

    data_type_o *temp_out_0_2 = (data_type_o *)malloc(out_size_0_1);
    if (temp_out_0_2 == NULL) {
        printf("False memory allocation of temp_out_0_2\n");
    }
    else {
        printf("temp_out_0_2 memory location= 0x%x \n", temp_out_0_2);
    }

    data_type_o *temp_out_0_3 = (data_type_o *)malloc(out_size_0_1);
    if (temp_out_0_3 == NULL) {
        printf("False memory allocation of temp_out_0_3\n");
    }
    else {
        printf("temp_out_0_3 memory location= 0x%x \n", temp_out_0_3);
    }

    data_type_o *temp_out_0_4 = (data_type_o *)malloc(out_size_0_1);
    if (temp_out_0_4 == NULL) {
        printf("False memory allocation of temp_out_0_4\n");
    }
    else {
        printf("temp_out_0_4 memory location= 0x%x \n", temp_out_0_4);
    }
   data_type_o *temp_out_1_1 = (data_type_o *)malloc(out_size_1_1);
   if (temp_out_1_1 == NULL) {
      printf("False memory allocation of temp_out_1_1\n");
   }
   else {
      printf("temp_out_1_1 memory location= 0x%x \n", temp_out_1_1);
   }


    ////////////////////////////////////////////////////////////////////
    data_type_o *data_o_0 = (data_type_o *)malloc(out_size_0_1);
    if (data_o_0 == NULL) {
        printf("False memory allocation of data_o_0\n");
    }
    else {
        printf("data_o_0 memory location= 0x%x \n", data_o_0);
    }
    data_type_o *data_o_1 = (data_type_o *)malloc(out_size_0_1);
    if (data_o_1 == NULL) {
        printf("False memory allocation of data_o_1\n");
    }
    else {
        printf("data_o_1 memory location= 0x%x \n", data_o_1);
    }
    data_type_o *data_o_2 = (data_type_o *)malloc(out_size_0_1);
    if (data_o_2 == NULL) {
        printf("False memory allocation of data_o_2\n");
    }
    else {
        printf("data_o_2 memory location= 0x%x \n", data_o_2);
    }
    data_type_o *data_o_3 = (data_type_o *)malloc(out_size_0_1);
    if (data_o_3 == NULL) {
        printf("False memory allocation of data_o_3\n");
    }
    else {
        printf("data_o_3 memory location= 0x%x \n", data_o_3);
    }
    data_type_o *data_o_4 = (data_type_o *)malloc(out_size_0_1);
    if (data_o_4 == NULL) {
        printf("False memory allocation of data_o_4\n");
    }
    else {
        printf("data_o_4 memory location= 0x%x \n", data_o_4);
    }
    data_type_o *data_o_5 = (data_type_o *)malloc(out_size_0_1);
    if (data_o_5 == NULL) {
        printf("False memory allocation of data_o_5\n");
    }
    else {
        printf("data_o_5 memory location= 0x%x \n", data_o_5);
    }
    data_type_o *data_o_6 = (data_type_o *)malloc(out_size_0_1);
    if (data_o_6 == NULL) {
        printf("False memory allocation of data_o_6\n");
    }
    else {
        printf("temp_out_0_2 memory location= 0x%x \n", data_o_6);
    }
    data_type_o *data_o_7 = (data_type_o *)malloc(out_size_0_1);
    if (data_o_7 == NULL) {
        printf("False memory allocation of data_o_7\n");
    }
    else {
        printf("temp_out_0_2 memory location= 0x%x \n", data_o_7);
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
   for (int i = 0; i < 1; i++) {
      for (int j = 0; j < 28; j++) {
         for (int k = 0; k < 28; k++) {
            temp_out_0_1[in_data_size] = (data_type)image_orig[i*28*28 + 28*j + k];
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
      conv_bias_num++;
   }
   free(conv_1_bias2D);
   in_number_conv++;

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
   for (int i = 0; i < 4000; i++) {
      fc_weight_mem_port[fc_weight_num] = (data_type_w)fc_1_weight2D[i];
      fc_weight_num++;
   }
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

   //Inference network process
    inf_net_0(
        conv_weight_mem_port,
        conv_weight_mem_port,
        conv_bias_mem_port,
        temp_out_0_1,
        temp_out_0_1,
        temp_out_0_1,
        temp_out_0_1,
        temp_out_0_1,
        temp_out_0_1,
        temp_out_0_1,
        temp_out_0_1,
        data_o_0,
        data_o_1,
        data_o_2,
        data_o_3,
        data_o_4,
        data_o_5,
        data_o_6,
        data_o_7);
/*
    inf_net_1(
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
*/
     /*
    inference_net_2(
        fc_weight_mem_port,
        fc_bias_mem_port,
        temp_out_0_3,
        temp_out_0_4);
    */
    finish = clock();
   totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
   cout <<"predicted time is: " << totaltime << " s" << endl;
   for (int i = 0; i < 10; i++) {
      fc_3_out[i]=(float)(fc_3_out_mem_int[i]);
   }
   softmax(fc_3_out, 10);
   predict(fc_3_out, 10);

    cout << "Finished prediction" << endl;

   return 0;

}
