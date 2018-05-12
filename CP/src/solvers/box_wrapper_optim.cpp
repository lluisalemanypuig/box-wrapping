#include "box_wrapper_optim.hpp"

box_wrapper_optim::box_wrapper_optim(const gifts& _data, length max_L) : box_wrapper_rotate(_data, max_L) {
	data = _data;
}

box_wrapper_optim::box_wrapper_optim(box_wrapper_optim& bw) : box_wrapper_rotate(bw) {
	data = bw.data;
}

box_wrapper_optim::~box_wrapper_optim() {
}

Space *box_wrapper_optim::copy() {
	return new box_wrapper_optim(*this);
}

void box_wrapper_optim::constrain(const Space& _b) {
	const box_wrapper_optim& ROLL = static_cast<const box_wrapper_optim&>(_b);
	
	const int N = data.total_boxes;
	
	// T := this solution's score (i.e. roll's length)
	// R := ROLL's score (i.e. roll's length)
	
	// impose (using rel) that T < R
	// that is, no top-left corner in this solution can
	// be placed beyond the upper bound determined by R
	
	// find ROLL's length
	length RL = 0;
	for (int b = 0; b < N; ++b) {
		
		bool found = false;
		for (length i = 0; i < L and not found; ++i) {
			for (width j = 0; j < W and not found; ++j) {
				if (ROLL.X(b,i,j).val() == 1) {
					found = true;
					if (ROLL.R(b).val() == 0) {
						RL = max(RL, i + data.all_boxes[b].l);
					}
					else {
						RL = max(RL, i + data.all_boxes[b].w);
					}
				}
			}
		}
	}
	for (int b = 0; b < N; ++b) {
		// the top-left corner cannot be in those positions that will 
		// take up more than RL length of the roll
		for (length i = RL - 1; i < L; ++i) {
			for (width j = 0; j < W; ++j) {
				rel(*this, C(b,i,j) == 0);
			}
		}
	}
}

