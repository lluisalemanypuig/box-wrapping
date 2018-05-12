#include "box_wrapper_optim.hpp"

box_wrapper_optim::box_wrapper_optim(const gifts& _data, length max_L) : box_wrapper_rotate(_data, max_L) {
	data = _data;
}

box_wrapper_optim::box_wrapper_optim(box_wrapper_optim& bw) : box_wrapper_rotate(bw) {
	data = bw.data;
}

box_wrapper_optim::~box_wrapper_optim() {
}

