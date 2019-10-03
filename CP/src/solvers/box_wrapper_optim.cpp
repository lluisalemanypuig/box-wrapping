/*********************************************************************
 * Box Wrapping solver
 * Copyright (C) 2018  Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

#include "box_wrapper_optim.hpp"

namespace solver {
	
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

} // -- namespace solver

