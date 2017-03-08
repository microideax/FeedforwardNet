// Created by Yao Chen
// Class for data quantization functions

#ifndef _DATA_QUANTIZE_H_
#define _DATA_QUANTIZE_H_

void quantize (float &x){
    if (x >= -1.0 && x <= -0.75 )
        x = -1.0;
    else if (x > -0.75 && x <= -0.25)
        x = -0.5;
    else if (x <= 0.25 && x > -0.25)
        x = 0;
    else if (x > 0.25 && x <= 0.75)
        x = 0.5;
    else x = 1.0;
}

void quantize (float a, float b, float &x){
    if (x < a || x > b)
        x = 0;
}



#endif
