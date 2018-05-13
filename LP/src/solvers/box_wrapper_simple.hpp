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

class box_wrapper_simple {
	protected:
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
		
		// CPLEX variables
		IloEnv env;
		IloModel model;
		IloCplex cplex;
		
		// upper bound on the roll's length
		length L;
		// roll's width. Fixed value
		width W;
	
	protected:
		const IloNumVar cX(size_t b, size_t i, size_t j) const { return box_corner[b*W*L + i*W + j]; }
		IloNumVar X(size_t b, size_t i, size_t j) { return box_corner[b*W*L + i*W + j]; }
		const IloNumVar cC(size_t b, size_t i, size_t j) const { return box_cell[b*W*L + i*W + j]; }
		IloNumVar C(size_t b, size_t i, size_t j) { return box_cell[b*W*L + i*W + j]; }
		
	public:
		box_wrapper_simple();
		box_wrapper_simple(const box_wrapper_simple& bw);
		
		void init(const gifts& gs, length max_L = -1);
		void solve();
		void to_wrapped_boxes(const gifts& boxes_to_wrap, wrapped_boxes& wb) const;
};

