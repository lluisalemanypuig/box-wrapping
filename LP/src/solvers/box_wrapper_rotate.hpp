#pragma once

// C++ includes
#include <iostream>
using namespace std;

// CPLEX
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

// Custom includes
#include "utils/wrapped_boxes.hpp"
#include "utils/definitions.hpp"
#include "utils/input_data.hpp"
#include "utils/logger.hpp"
#include "box_solver.hpp"

class box_wrapper_rotate : public box_solver {
	protected:
		// constraints for spanning cells when box is rotable
		// but it is not rotated
		void span_cells_non_rot
		(
			int b, length i, width j,
			length b_length, width b_width
		);
		
		// constraints for spanning cells when box is rotable
		// and it is rotated
		void span_cells_rot
		(
			int b, length i, width j,
			length b_length, width b_width
		);
		
		// constraints for spanning cells when box is square
		void span_cells_square
		(
			int b, length i, width j,
			length b_length, width b_width
		);
		
	protected:
		// box_rotated[b] = 0 <-> box b is placed in the way it is indicated
		// 						  in the input (l,w)
		// box_rotated[b] = 1 <-> box b is rotated (w,l)
		IloNumVarArray box_rotated;
		
		// box_cell[b][i][j] = 1 <-> cell (i, j) is occupied by box b
		IloNumVarArray box_cell;
		
		// box_corner[b][i][j] = 1 <-> top-left corner of box b
		// is at cell (i,j) with
		//     1 <= j <= W - w_b (width)
		//     1 <= i <= L - l_b (length)
		// where
		//     w_b is the width of box b
		//     l_b is the length of box b
		IloNumVarArray box_corner;
		
		// upper bound on the roll's length
		length L;
		// roll's width. Fixed value
		width W;
	
	protected:
		IloNumVar X(size_t b, size_t i, size_t j) const { return box_corner[b*W*L + i*W + j]; }
		IloNumVar C(size_t b, size_t i, size_t j) const { return box_cell[b*W*L + i*W + j]; }
		IloNumVar R(size_t b) const { return box_rotated[b]; }
		
		IloNumVar X(size_t b, size_t i, size_t j) { return box_corner[b*W*L + i*W + j]; }
		IloNumVar C(size_t b, size_t i, size_t j) { return box_cell[b*W*L + i*W + j]; }
		IloNumVar R(size_t b) { return box_rotated[b]; }
		
		virtual void add_objective();
		
	public:
		box_wrapper_rotate();
		box_wrapper_rotate(const box_wrapper_rotate& bw);
		virtual ~box_wrapper_rotate();
		
		void init(const gifts& gs, length max_L = -1);
		void solution(const gifts& boxes_to_wrap, wrapped_boxes& wb) const;
};

