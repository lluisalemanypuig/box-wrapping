#pragma once

/// C includes
#include <assert.h>

/// C++ includes
#include <iostream>
using namespace std;

/// Gecode
#include <gecode/minimodel.hh>
#include <gecode/search.hh>
#include <gecode/int.hh>
using namespace Gecode;

/// Custom includes
#include "utils/wrapped_boxes.hpp"
#include "utils/definitions.hpp"
#include "utils/input_data.hpp"
#include "utils/logger.hpp"

class box_wrapper_rotate : public Space {
	protected:
		// constraints for spanning cells when box is rotable
		// but it is not rotated
		void span_cells_non_rot
		(
			length L, width W,
			size_t b, length i, width j,
			length b_length, width b_width
		);
		
		// constraints for spanning cells when box is rotable
		// and it is rotated
		void span_cells_rot
		(
			length L, width W,
			size_t b, length i, width j,
			length b_length, width b_width
		);
		
		// constraints for spanning cells when box is square
		void span_cells_square
		(
			length L, width W,
			size_t b, length i, width j,
			length b_length, width b_width
		);
		
	protected:
		// box_rotated[k] = 0 <-> box k is placed in the way it is indicated
		// 						  in the input (l,w)
		// box_rotated[k] = 1 <-> box k is rotated (w,l)
		BoolVarArray box_rotated;
		
		// box_cell[k][i][j] = 1 <-> cell (i, j) is occupied by box k
		BoolVarArray box_cell;
		
		// box_corner[k][i][j] = 1 <-> top-left corner of box k
		// is at cell (i,j) with
		//     1 <= j <= W - w_k (width)
		//     1 <= i <= L - l_k (length)
		// where
		//     w_k is the width of box k
		//     l_k is the length of box k
		BoolVarArray box_corner;
		
		// upper bound on the roll's length
		length upper_bound_L;
		
	public:
		box_wrapper_rotate(const gifts& gs, length max_L = -1);
		box_wrapper_rotate(box_wrapper_rotate& bw);
		virtual ~box_wrapper_rotate();
		
		virtual Space *copy();
		
		void to_wrapped_boxes(const gifts& boxes_to_wrap, wrapped_boxes& wb) const;
};

