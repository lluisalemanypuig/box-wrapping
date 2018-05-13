#include "box_wrapper_simple.hpp"

/* PUBLIC */

box_wrapper_simple::box_wrapper_simple() {
	
}

box_wrapper_simple::box_wrapper_simple(const box_wrapper_simple& bw) {
	box_cell = bw.box_cell;
	box_corner = bw.box_corner;
	L = bw.L;
	W = bw.W;
}

void box_wrapper_simple::init(const gifts& data, length max_L) {
	// Initialise CPLEX variables needed to create the model
	env = IloEnv();
	model = IloModel(env);
	cplex = IloCplex(model);
	
	if (max_L < 0) {
		L = inf_t<int>();
	}
	else {
		L = max_L;
	}
	
	const int N = data.total_boxes;
	W = data.W;
	L = min(L, data.get_max_length_s());
	
	// initialise arrays
	box_cell = IloNumVarArray(env, N*W*L, 0, 1, ILOINT);
	box_corner = IloNumVarArray(env, N*W*L, 0, 1, ILOINT);
	
	/// CONSTRAINTS
	
	cout << "Building 1... ";
	
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
	
	cout << "Built!" << endl;
	cout << "Building 2... ";
	
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
	
	cout << "Built!" << endl;
	cout << "Building 3... ";
	
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
				
				IloExpr span(env);
				
				for (length ii = i; ii <= i + b_length - 1; ++ii) {
					for (width jj = j; jj <= j + b_width - 1; ++jj) {
						span += C(b,ii,jj);
					}
				}
				
				model.add(
					IloIfThen(env, (X(b,i,j) == 1), (span == b_length*b_width))
				);
				span.end();
			}
		}
	}
	
	cout << "Built!" << endl;
	cout << "Building 4... ";
	
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
				
				model.add(X(b,i,j) == 0);
			}
		}
	}
	
	cout << "Built!" << endl;
	
	
	cplex = IloCplex(model);
}

void box_wrapper_simple::solve() {
	
	cout << "About to solve..." << endl;
	bool solved = cplex.solve();
	
	if (solved) {
		cout << "Is solved? " << (solved ? "Yes" : "No") << endl;
	}
	
}

void box_wrapper_simple::to_wrapped_boxes(const gifts& data, wrapped_boxes& wb) const {
	const int N = data.total_boxes;
	
	wb.init(N, L, W);
	
	for (int b = 0; b < N; ++b) {
		bool found = false;
		length pl = L + 1;
		width pw = W + 1;
		
		for (length i = 0; i < L; ++i) {
			for (width j = 0; j < W; ++j) {
				
				// for corner finding
				if (not found) {
					if (cplex.getValue(cX(b,i,j)) == 1) {
						pl = i;
						pw = j;
						found = true;
					}
				}
				
				// for roll copying
				if (cplex.getValue(cC(b,i,j)) == 1) {
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

