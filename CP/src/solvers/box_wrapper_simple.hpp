#pragma once

// C++ includes
#include <iostream>
using namespace std;

// Gecode
#include <gecode/minimodel.hh>
#include <gecode/search.hh>
#include <gecode/int.hh>
using namespace Gecode;

// Custom includes
#include "utils/wrapped_boxes.hpp"
#include "utils/definitions.hpp"
#include "utils/input_data.hpp"
#include "utils/logger.hpp"

class box_wrapper_simple : public Space {
	protected:
		// box_cell[b][i][j] = 1 <-> cell (i, j) is occupied by box b
		BoolVarArray box_cell;
		
		// box_corner[b][i][j] = 1 <-> top-left corner of box b
		// is at cell (i,j) with
		//     1 <= j <= W - w_b (width)
		//     1 <= i <= L - l_b (length)
		// where
		//     w_b is the width of box b
		//     l_b is the length of box b
		BoolVarArray box_corner;
		
		// upper bound on the roll's length
		length L;
		// roll's width. Fixed value.
		width W;
	
	protected:
		inline BoolVar X(size_t b, size_t i, size_t j) const { return box_corner[b*W*L + i*W + j]; }
		inline BoolVar C(size_t b, size_t i, size_t j) const { return box_cell[b*W*L + i*W + j]; }
		
		inline BoolVar X(size_t b, size_t i, size_t j) { return box_corner[b*W*L + i*W + j]; }
		inline BoolVar C(size_t b, size_t i, size_t j) { return box_cell[b*W*L + i*W + j]; }
	
	public:
		box_wrapper_simple(const gifts& gs, length max_L = -1);
		box_wrapper_simple(box_wrapper_simple& bw);
		
		virtual Space *copy();
		
		void to_wrapped_boxes(const gifts& boxes_to_wrap, wrapped_boxes& wb) const;
};

