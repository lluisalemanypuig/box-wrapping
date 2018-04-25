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
	
	const size_t N = data.total_boxes;
	const width W = data.W;
	const length L = min(upper_bound_L, data.get_max_length_ro());
	
	// T := this solution's score (i.e. roll's length)
	// R := ROLL's score (i.e. roll's length)
	
	// impose (using rel) that T < R
	// that is, no top-left corner in this solution can
	// be placed beyond the upper bound determined by R
	
	// find ROLL's length
	length RL = 0;
	for (size_t b = 0; b < N; ++b) {
		
		bool found = false;
		for (size_t i = 0; i < L and not found; ++i) {
			for (size_t j = 0; j < W and not found; ++j) {
				if (ROLL.box_corner[b*W*L + i*W + j].val() == 1) {
					found = true;
					if (ROLL.box_rotated[b].val() == 0) {
						RL = max(RL, i + data.all_boxes[b].l);
					}
					else {
						RL = max(RL, i + data.all_boxes[b].w);
					}
				}
			}
		}
	}
	for (size_t b = 0; b < N; ++b) {
		// the top-left corner cannot be in those positions that will 
		// take up more than RL length of the roll
		for (size_t i = RL - 1; i < L; ++i) {
			for (size_t j = 0; j < W; ++j) {
				rel(*this, box_cell[b*W*L + i*W + j] == 0);
			}
		}
	}
}

