/*
    Copyright (c) 2013, Taiga Nomi
    All rights reserved.
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY 
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#pragma once
#include "../util/util.h"
#include <algorithm>

namespace tiny_dnn {
namespace activation {

class function {
public:
    function(){};
    function(const function &){};
// #ifndef CNN_DEFAULT_MOVE_CONSTRUCTOR_UNAVAILABLE
//     function(function &&){};
// #endif
    function &operator =(const function &){};
// #ifndef CNN_DEFAULT_ASSIGNMENT_OPERATOR_UNAVAILABLE
//     function &operator =(function &&) = default;
// #endif
    virtual ~function(){}; //Yao: Modified default destruct function
    virtual float_t f(const vec_t& v, cnn_size_t index) const = 0;

    // dfi/dyi
    virtual float_t df(float_t y) const = 0;

    // dfi/dyk (k=0,1,..n)
    virtual vec_t df(const vec_t& y, cnn_size_t i) const { vec_t v(y.size(), 0); v[i] = df(y[i]); return v; }

    // return if dfi/dyk is one-hot vector
    virtual bool one_hot() const { return true; }

    // target value range for learning
    virtual std::pair<float_t, float_t> scale() const = 0;
};

class identity : public function {
public:
    using function::df;
    float_t f(const vec_t& v, cnn_size_t i) const { return v[i]; } //Yao: deleted override
    float_t df(float_t /*y*/) const  { return float_t(1); } //Yao: deleted override
    std::pair<float_t, float_t> scale() const { return std::make_pair(float_t(0.1), float_t(0.9)); } //Yao: deleted override
};

class sigmoid : public function {
public:
    using function::df;
    float_t f(const vec_t& v, cnn_size_t i) const { return float_t(1) / (float_t(1) + exp(-v[i])); }  //Yao: deleted override
    float_t df(float_t y) const { return y * (float_t(1) - y); } //Yao: deleted override
    std::pair<float_t, float_t> scale() const { return std::make_pair(float_t(0.1), float_t(0.9)); } //Yao: deleted override
};

class relu : public function {
public:
    using function::df;
    float_t f(const vec_t& v, cnn_size_t i) const { return std::max(float_t(0), v[i]); } //Yao: deleted override
    float_t df(float_t y) const { return y > float_t(0) ? float_t(1) : float_t(0); } //Yao: deleted override
    std::pair<float_t, float_t> scale() const { return std::make_pair(float_t(0.1), float_t(0.9)); } //Yao: deleted override
};

typedef relu rectified_linear; // for compatibility

class leaky_relu : public function {
public:
    using function::df;
    float_t f(const vec_t& v, cnn_size_t i) const { return (v[i] > float_t(0)) ? v[i] : float_t(0.01) * v[i]; }//Yao:deleted override
    float_t df(float_t y) const { return y > float_t(0) ? float_t(1) : float_t(0.01); }//Yao: deleted override
    std::pair<float_t, float_t> scale() const { return std::make_pair(float_t(0.1), float_t(0.9)); }//Yao: deleted override
};

class elu : public function {
public:
    using function::df;
    float_t f(const vec_t& v, cnn_size_t i) const  { return (v[i]<float_t(0) ? (exp(v[i])- float_t(1)) : v[i]); }//Yao: deleted override
    float_t df(float_t y) const { return (y > float_t(0) ? float_t(1) : (float_t(1)+y)); }//Yao: deleted override
    std::pair<float_t, float_t> scale() const { return std::make_pair(float_t(0.1), float_t(0.9)); }//Yao: deleted override
};

class softmax : public function {
public:
    float_t f(const vec_t& v, cnn_size_t i) const {//Yao: deleted override
        float_t alpha = *std::max_element(v.begin(), v.end());
        float_t numer = exp(v[i] - alpha);
        float_t denom = float_t(0);
        // for (auto x : v)
        //     denom += std::exp(x - alpha);
        for (int i=0; i < v.size(); i++){
            denom += exp(v[i] - alpha);
        }
        return numer / denom;
    }

    float_t df(float_t y) const {//Yao: deleted override
        return y * (float_t(1) - y);
    }

    virtual vec_t df(const vec_t& y, cnn_size_t index) const  {//Yao: deleted override
        vec_t v(y.size(), 0);
        for (cnn_size_t i = 0; i < y.size(); i++)
            v[i] = (i == index) ? df(y[index]) : -y[i] * y[index];

        return v;
    }

    virtual bool one_hot() const { return false; }//Yao: deleted override

    std::pair<float_t, float_t> scale() const { return std::make_pair(float_t(0), float_t(1)); }//Yao: deleted override
};

class tan_h : public function {
public:
    using function::df;
    float_t f(const vec_t& v, cnn_size_t i) const {//Yao: deleted override
        const float_t ep = exp(v[i]);
        const float_t em = exp(-v[i]); 
        return (ep - em) / (ep + em);
    }

    // fast approximation of tanh (improve 2-3% speed in LeNet-5)
    /*float_t f(float_t x) const {
        const float_t x2 = x * x;
        x *= 1.0 + x2 * (0.1653 + x2 * 0.0097);
        return x / std::sqrt(1.0 + x * x);// invsqrt(static_cast<float>(1.0 + x * x));
    }*/

    float_t df(float_t y) const { return float_t(1) - sqr(y); }//Yao: deleted override
    std::pair<float_t, float_t> scale() const { return std::make_pair(float_t(-0.8), float_t(0.8)); }//Yao: deleted override

private:
    /*float invsqrt(float x) const {
        float x2 = x * 0.5f;
        long i = *reinterpret_cast<long*>(&x);

        i = 0x5f3759df - (i >> 1);
        x = *reinterpret_cast<float*>(&i);
        x = x * (1.5f - (x2 * x * x));
        return x;
    }*/
};

// s tan_h, but scaled to match the other functions
class tan_hp1m2 : public function {
public:
    using function::df;
    float_t f(const vec_t& v, cnn_size_t i) const {//Yao: deleted override
        const float_t ep = exp(v[i]);
        return ep / (ep + exp(-v[i]));
    }

    float_t df(float_t y) const { return 2 * y *(float_t(1) - y); }//Yao: deleted override
    std::pair<float_t, float_t> scale() const { return std::make_pair(float_t(0.1), float_t(0.9)); }//Yao: deleted override
};

} // namespace activation
} // namespace tiny_dnn
