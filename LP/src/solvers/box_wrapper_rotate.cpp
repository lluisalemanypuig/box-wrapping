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

#include "box_wrapper_rotate.hpp"

namespace solver {

/* PROTECTED */

void box_wrapper_rotate::span_cells_non_rot
(
	int b, length i, width j,
	length b_length, width b_width
)
{
	for (length ii = i; ii <= i + b_length - 1; ++ii) {
		for (width jj = j; jj <= j + b_width - 1; ++jj) {
			model.add(
				IloIfThen(env, ((R(b) == 0) && (X(b,i,j) == 1)), (C(b,ii,jj) == 1))
			);
		}
	}
}

void box_wrapper_rotate::span_cells_rot
(
	int b, length i, width j,
	length b_length, width b_width
)
{
	for (length ii = i; ii <= i + b_width - 1; ++ii) {
		for (width jj = j; jj <= j + b_length - 1; ++jj) {
			model.add(
				IloIfThen(env, ((R(b) == 1) && (X(b,i,j) == 1)), (C(b,ii,jj) == 1))
			);
		}
	}
}

void box_wrapper_rotate::span_cells_square
(
	int b, length i, width j,
	length b_length, width b_width
)
{
	for (length ii = i; ii <= i + b_length - 1; ++ii) {
		for (width jj = j; jj <= j + b_width - 1; ++jj) {
			model.add(
				IloIfThen(env, (X(b,i,j) == 1), (C(b,ii,jj) == 1))
			);
		}
	}
	model.add(R(b) == 0);
}

void box_wrapper_rotate::add_objective(const gifts& ) { }

void box_wrapper_rotate::_init(const gifts& data) {
	// initialise arrays
	box_cell = IloNumVarArray(env, N*W*L, 0, 1, ILOINT);
	box_corner = IloNumVarArray(env, N*W*L, 0, 1, ILOINT);
	box_rotated = IloNumVarArray(env, N, 0, 1, ILOINT);
	
	/// CONSTRAINTS
	
	/// (1). Exactly one corner per box
	for (int b = 0; b < N; ++b) {
		IloExpr exa_X(env);
		for (length i = 0; i < L; ++i) {
			for (width j = 0; j < W; ++j) {
				exa_X += X(b,i,j);
			}
		}
		model.add(exa_X == 1);
		exa_X.end();
	}
	
	/// (2). At most one box per cell
	for (length i = 0; i < L; ++i) {
		for (width j = 0; j < W; ++j) {
			IloExpr amo_C(env);
			for (int b = 0; b < N; ++b) {
				amo_C += C(b,i,j);
			}
			model.add(amo_C <= 1);
			amo_C.end();
		}
	}
	
	/// (3). Placing the top-left corner of a box at (i,j)
	/// makes the box occupy several cells of the roll.
	for (int b = 0; b < N; ++b) {
		const box& BOX = data.all_boxes[b];
		length b_length = BOX.l;
		width b_width = BOX.w;
		
		// for each cell to be occupied by box 'b' (without rotation)
		for (length i = 0; i < L; ++i) {
			for (width j = 0; j < W; ++j) {
				
				if (BOX.is_square()) {
					// when the box is square the constraints are simpler
					
					// consider only the cases where the whole box is within bounds
					bool within = (i + b_length - 1 < L) and (j + b_width - 1 < W);
					if (within) {
						span_cells_square(b, i, j, b_length, b_width);
					}
				}
				else {
					// rotate only if the box is not square
					
					// consider only the cases where the whole box is within bounds
					bool within1 = (i + b_length - 1 < L) and (j + b_width - 1 < W);
					if (within1) {
						span_cells_non_rot(b, i, j, b_length, b_width);
					}
					
					bool within2 = (i + b_width - 1 < L) and (j + b_length - 1 < W);
					if (within2) {
						span_cells_rot(b, i, j, b_length, b_width);
					}
				}
			}
		}
	}
	
	/// (4). Cannot place the top-left corner of a box at
	/// cell (i,j) if it will end up out of bounds
	for (int b = 0; b < N; ++b) {
		const box& BOX = data.all_boxes[b];
		length b_length = BOX.l;
		width b_width = BOX.w;
		
		// for each cell to be occupied by box 'b' (without rotation)
		for (length i = 0; i < L; ++i) {
			for (width j = 0; j < W; ++j) {
				
				if (BOX.is_square()) {
					// when the box is square the constraints are simpler
					
					// if the box is WITHIN limits then ignore
					bool within = (i + b_length - 1 <= L - 1) and (j + b_width - 1 <= W - 1);
					if (not within) {
						model.add(X(b,i,j) == 0);
					}
				}
				else {
					
					// if the box is WITHIN limits then ignore
					// the box is within bounds if:
					// i + b_length - 1 <= L - 1 and j + b_width - 1 <= W - 1
					
					bool within1 = (i + b_length - 1 <= L - 1) and (j + b_width - 1 <= W - 1);
					if (not within1) {
						model.add(
							IloIfThen(env, (R(b) == 0), (X(b,i,j) == 0))
						);
					}
					
					bool within2 = (i + b_width - 1 <= L - 1) and (j + b_length - 1 <= W - 1);
					if (not within2) {
						model.add(
							IloIfThen(env, (R(b) == 1), (X(b,i,j) == 0))
						);
					}
				}
			}
		}
	}
	
	add_objective(data);
}

/* PUBLIC */

box_wrapper_rotate::box_wrapper_rotate() : box_solver() {
	
}

box_wrapper_rotate::box_wrapper_rotate(const box_wrapper_rotate& bw) : box_solver() {
	box_rotated = bw.box_rotated;
	box_cell = bw.box_cell;
	box_corner = bw.box_corner;
	N = bw.N;
	L = bw.L;
	W = bw.W;
}

box_wrapper_rotate::~box_wrapper_rotate() { }

void box_wrapper_rotate::solution(const gifts& data, wrapped_boxes& wb) const {
	wb.init(N, L, W);
	
	for (int b = 0; b < N; ++b) {
		bool found = false;
		length pl = L + 1;
		width pw = W + 1;
		
		size_t k = 0;
		while (k < L*W and not found) {
			if (cplex.getValue(box_corner[b*L*W + k]) == 1) {
				pl = k/W;
				pw = k%W;
				found = true;
			}
			++k;
		}
		
		length box_length = data.all_boxes[b].l;
		width box_width = data.all_boxes[b].w;
		
		if (cplex.getValue(R(b)) == 1) {
			box_length = data.all_boxes[b].w;
			box_width = data.all_boxes[b].l;
		}
		
		for (length i = pl; i < pl + box_length; ++i) {
			for (width j = pw; j < pw + box_width; ++j) {
				wb.set_box_cell(b + 1, cell(i,j) );
			}
		}
		
		wb.set_tl_box_corner(b, corner(pl,pw));
		wb.set_br_box_corner(b,
			corner(
				pl + box_length - 1,
				pw + box_width - 1
			)
		);
	}
}

} // -- namespace solver

