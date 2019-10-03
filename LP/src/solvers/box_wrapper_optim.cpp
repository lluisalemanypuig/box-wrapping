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

/* PROTECTED */

void box_wrapper_optim::add_objective(const gifts& data) {
	// initialise arrays and vars
	box_coordinates = IloNumVarArray(env, N, 0, L, ILOINT);
	max_length = IloNumVar(env, 0, L, ILOINT);
	
	// first, what are the coordinates?
	for (int b = 0; b < N; ++b) {
		for (length i = 0; i < L; ++i) {
			for (width j = 0; j < W; ++j) {
				model.add(
					IloIfThen(env, ((R(b) == 0) && (X(b,i,j) == 1)), (S(b) == i + data.all_boxes[b].l))
				);
				model.add(
					IloIfThen(env, ((R(b) == 1) && (X(b,i,j) == 1)), (S(b) == i + data.all_boxes[b].w))
				);
			}
		}
		
		model.add(max_length >= S(b));
	}
	
	model.add(
		IloMinimize(env, max_length)
	);
}

/* PUBLIC */

box_wrapper_optim::box_wrapper_optim() : box_wrapper_rotate() { }

box_wrapper_optim::box_wrapper_optim(const box_wrapper_optim& bw) : box_wrapper_rotate(bw) { }

box_wrapper_optim::~box_wrapper_optim() { }

} // -- namespace solver

