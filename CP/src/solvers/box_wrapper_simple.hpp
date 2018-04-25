#pragma once

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

class box_wrapper_simple : public Space {
	protected:
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
		box_wrapper_simple(const gifts& gs, length max_L = -1);
		box_wrapper_simple(box_wrapper_simple& bw);
		
		virtual Space *copy();
		
		void to_wrapped_boxes(const gifts& boxes_to_wrap, wrapped_boxes& wb) const;
};

