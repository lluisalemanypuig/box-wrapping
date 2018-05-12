#include "box_wrapper_simple.hpp"

/* PUBLIC */

box_wrapper_simple::box_wrapper_simple(const gifts& data, length max_L) {
	if (max_L < 0) {
		upper_bound_L = inf_t<int>();
	}
	else {
		upper_bound_L = max_L;
	}
	
	const int N = data.total_boxes;
	const width W = data.W;
	const length L = min(upper_bound_L, data.get_max_length_s());
	
	// initialise arrays
	box_cell = BoolVarArray(*this, N*W*L, 0, 1);
	box_corner = BoolVarArray(*this, N*W*L, 0, 1);
	
	/// CONSTRAINTS
	
	for (int b = 0; b < N; ++b) {
		/// (1). Exactly one corner per box
		rel(*this, sum(box_corner.slice(b*W*L, 1, W*L)) == 1);
		
		/// (2). At most one box per cell
		for (length i = 0; i < L; ++i) {
			for (width j = 0; j < W; ++j) {
				rel(*this, sum(box_cell.slice(b*W*L + i*W + j, W*L, N)) <= 1);
			}
		}
	}
	
	/// (3). Placing the top-left corner of a box at (i,j)
	/// makes the box occupy several cells of the roll.
	for (int b = 0; b < N; ++b) {
		length b_length = data.all_boxes[b].l;
		width b_width = data.all_boxes[b].w;
		
		// for each cell to be occupied by box 'b' (without rotation)
		for (length i = 0; i < L; ++i) {
			for (width j = 0; j < W; ++j) {
				
				// ignore the cases where part of the box is out of bounds
				if (i + b_length - 1 >= L) continue;
				if (j + b_width - 1 >= W) continue;
				
				for (length ii = i; ii <= i + b_length - 1; ++ii) {
					for (width jj = j; jj <= j + b_width - 1; ++jj) {
						
						rel(*this,
							(box_corner[b*W*L + i*W + j] == 1)
							>>
							(box_cell[b*W*L + ii*W + jj] == 1)
						);
					}
				}
			}
		}
	}
	
	/// (4). Cannot place the top-left corner of a box at
	/// cell (i,j) if it will end up out of bounds
	for (int b = 0; b < N; ++b) {
		length b_length = data.all_boxes[b].l;
		width b_width = data.all_boxes[b].w;
		
		// for each cell to be occupied by box 'b' (without rotation)
		for (length i = 0; i < L; ++i) {
			for (width j = 0; j < W; ++j) {
				
				// if the box is WITHIN limits then ignore
				if (i + b_length - 1 <= L - 1 and j + b_width - 1 <= W - 1) continue;
				
				rel(*this, box_corner[b*W*L + i*W + j] == 0);
			}
		}
	}
	
	// Do not add the variables in array box_cell: if a variable
	// is not assigned consider it as a 0 "no box in that cell".
	// This way we avoid the solver assigning 1's wherever it can
	// (whether it is on the slice for box 1, or for box 2, ...)
	branch(*this, box_corner, BOOL_VAR_NONE(), BOOL_VAL_MAX());
}

box_wrapper_simple::box_wrapper_simple(box_wrapper_simple& bw) : Space(bw) {
	box_cell.update(*this, bw.box_cell);
	box_corner.update(*this, bw.box_corner);
	upper_bound_L = bw.upper_bound_L;
}

Space *box_wrapper_simple::copy() {
	return new box_wrapper_simple(*this);
}

void box_wrapper_simple::to_wrapped_boxes(const gifts& data, wrapped_boxes& wb) const {
	const int N = data.total_boxes;
	const width W = data.W;
	const length L = min(upper_bound_L, data.get_max_length_s());
	
	wb.init(N, L, W);
	
	for (int b = 0; b < N; ++b) {
		bool found = false;
		length pl = L + 1;
		width pw = W + 1;
		
		for (length i = 0; i < L; ++i) {
			for (width j = 0; j < W; ++j) {
				
				// for corner finding
				if (not found) {
					BoolVar BX(box_corner[b*W*L + i*W + j]);
					if (BX.val() == 1) {
						pl = i;
						pw = j;
						found = true;
					}
				}
				
				// for roll copying
				BoolVar BC(box_cell[b*W*L + i*W + j]);
				if (not BC.none() and BC.val() == 1) {
					wb.set_box_cell(b + 1, cell(i, j));
				}
			}
		}
		
		wb.set_tl_box_corner(b, corner(pl,pw));
		wb.set_br_box_corner(b,
			corner(
				pl + data.all_boxes[b].l - 1,
				pw + data.all_boxes[b].w - 1
			)
		);
	}
}

