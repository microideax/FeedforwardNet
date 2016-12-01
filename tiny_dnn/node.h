/*
    Copyright (c) 2016, Taiga Nomi
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
#include <sstream>
#include <iomanip>
//#include <memory>
#include <numeric>
#include <vector>
#include <set>
#include <queue>
//#include <unordered_set>

#include "util/util.h"
#include "util/product.h"
#include "util/image.h"
#include "util/weight_init.h"
#include "optimizers/optimizer.h"

#include "activations/activation_function.h"

namespace tiny_dnn {

class node;
class layer;
class edge;

typedef node* nodeptr_t;
//typedef std::shared_ptr<edge> edgeptr_t;
typedef edge* edgeptr_t;
typedef layer* layerptr_t;

/**
 * base class of all kind of tinny-cnn data
 **/
//class node : public std::enable_shared_from_this<node> {
class node {
public:
    node(cnn_size_t in_size, cnn_size_t out_size)
        : prev_(in_size), next_(out_size) {}
    virtual ~node() {}

    const std::vector<edgeptr_t>& prev() const { return prev_; }
    const std::vector<edgeptr_t>& next() const { return next_; }

    cnn_size_t prev_port(const edge& e) const {
//        auto it = std::find_if(prev_.begin(), prev_.end(),
//                               [&](edgeptr_t ep) { return ep == &e; });
//        edgeptr_t it;
//        return (cnn_size_t)std::distance(prev_.begin(), it);
        cnn_size_t distance = 1;
        for (unsigned int i = 0; i < prev_.size(); i++) {
        	if (&e == prev_[i]) distance = i + 1;
        }
        return distance;
    }

    cnn_size_t next_port(const edge& e) const {
//        auto it = std::find_if(next_.begin(), next_.end(),
//                               [&](edgeptr_t ep) { return ep == &e; });
//        return (cnn_size_t)std::distance(next_.begin(), it);
    	cnn_size_t distance = 1;
    	for (unsigned int i = 0; i < next_.size(); i++) {
    		if (&e == next_[i]) distance = i + 1;
    	}
    	return distance;
    }

    std::vector<node*> prev_nodes() const; // @todo refactor and remove this method
    std::vector<node*> next_nodes() const; // @todo refactor and remove this method
 protected:
//    node() = delete;

    friend void connect(layerptr_t head, layerptr_t tail,
                        cnn_size_t head_index, cnn_size_t tail_index);

    mutable std::vector<edgeptr_t> prev_;
    mutable std::vector<edgeptr_t> next_;
};

/**
 * class containing input/output data
 **/
class edge {
 public:
    edge(node* prev, const shape3d& shape, int32_t vtype)
        : shape_(shape),
          vtype_(vtype),
//          data_({vec_t(shape.size())}),
//          grad_({vec_t(shape.size())}),
          prev_(prev) {
    	tensor_t t;
    	t.push_back(vec_t(shape.size()));
    	data_ = t;
    	grad_ = t;
    }

    void merge_grads(vec_t *dst) {
        dst->resize(grad_[0].size());
        std::fill(dst->begin(), dst->end(), static_cast<float_t>(0));

        // @todo consider adding parallelism
		for (cnn_size_t sample = 0, sample_count = grad_.size(); sample < sample_count; ++sample) {
			vectorize::reduce<float_t>(&grad_[sample][0], dst->size(), &(*dst)[0]);
		}
    }

    void clear_grads() {
		for (cnn_size_t sample = 0, sample_count = grad_.size(); sample < sample_count; ++sample) {
			std::fill(grad_[sample].begin(), grad_[sample].end(), (float_t)0);
		}
    }

    tensor_t* get_data() {
        return &data_;
    }

    const tensor_t* get_data() const {
        return &data_;
    }

    tensor_t* get_gradient() {
        return &grad_;
    }

    const tensor_t* get_gradient() const {
        return &grad_;
    }

    const std::vector<node*>& next() const { return next_; }
    node* prev() { return prev_; }
    const node* prev() const { return prev_; }

    const shape3d& shape() const { return shape_; }
    int32_t vtype() const { return vtype_; }
    void add_next_node(node* next) { next_.push_back(next); }

 private:
    shape3d shape_;
    int32_t vtype_;
    tensor_t data_;
    tensor_t grad_;
    node* prev_;               // previous node, "producer" of this tensor
    std::vector<node*> next_;  // next nodes, "consumers" of this tensor
};

inline std::vector<node*> node::prev_nodes() const {
    std::set<node*> sets;
//    for (auto& e : prev_) {
//        if (e && e->prev()) sets.insert(e->prev());
//    }
    for (unsigned int i = 0; i < prev_.size(); i++){
    	if ( prev_[i] && prev_[i]->prev())
    		sets.insert(prev_[i]->prev());
    }
    return std::vector<node*>(sets.begin(), sets.end());
}

inline std::vector<node*> node::next_nodes() const {
    std::set<node*> sets;
//    for (auto& e : next_) {
//        if (e) {
//            auto n = e->next();
//            sets.insert(n.begin(), n.end());
//        }
//    }
    for (unsigned int i = 0; i < next_.size(); i++){
    	if (next_[i]){
    		std::vector<node*> n = next_[i]->next();
    		sets.insert(n.begin(), n.end());
    	}
    }
    return std::vector<node*>(sets.begin(), sets.end());
}

template <typename T>
struct node_tuple {
    node_tuple(T l1, T l2) {
        nodes_.push_back(l1); nodes_.push_back(l2);
    }
    std::vector<T> nodes_;
};

template <typename T>
node_tuple<T*> operator , (T& l1, T& l2) {
    return node_tuple<T*>(&l1, &l2);
}

//template <typename T>
//node_tuple<std::shared_ptr<T>> operator , (std::shared_ptr<T> l1, std::shared_ptr<T> l2) {
//    return node_tuple<std::shared_ptr<T>>(l1, l2);
//}

template <typename T>
node_tuple<T*> operator , (node_tuple<T*> lhs, T*& rhs) {
	lhs.nodes_.push_back(rhs);
}
//node_tuple<std::shared_ptr<T>> operator , (node_tuple<std::shared_ptr<T>> lhs, std::shared_ptr<T>& rhs) {
//    lhs.nodes_.push_back(rhs);
//    return lhs;
//}

template <typename T>
node_tuple<T*> operator , (node_tuple<T*> lhs, T& rhs) {
    lhs.nodes_.push_back(&rhs);
    return lhs;
}

//template <typename T, typename U>
//inline std::shared_ptr<U>& operator << (std::shared_ptr<T>& lhs,
//                                        std::shared_ptr<U>& rhs) {
//    connect(lhs.get(), rhs.get());
//    return rhs;
//}

template <typename T, typename U>
inline U& operator << (const node_tuple<T>& lhs, U& rhs) {
    for (size_t i = 0; i < lhs.nodes_.size(); i++) {
        connect(&*lhs.nodes_[i], &*rhs, 0, i);
    }
    return rhs;
}

template <typename T, typename U>
inline node_tuple<T>& operator << (U& lhs, const node_tuple<T>& rhs) {
    for (size_t i = 0; i < rhs.nodes_.size(); i++) {
        connect(&*lhs, &*rhs.nodes_[i], i, 0);
    }
    return rhs;
}

template <typename T, typename U>
inline U& operator << (const node_tuple<T*>& lhs, U& rhs) {
    for (size_t i = 0; i < lhs.nodes_.size(); i++) {
        connect(lhs.nodes_[i], &rhs, 0, i);
    }
    return rhs;
}

template <typename T, typename U>
inline node_tuple<T*>& operator << (U& lhs, const node_tuple<T*>& rhs) {
    for (size_t i = 0; i < rhs.nodes_.size(); i++) {
        connect(&lhs, rhs.nodes_[i], i, 0);
    }
    return rhs;
}


}   // namespace tiny_dnn
