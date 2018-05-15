#include "box_wrapper_optim.hpp"

/* PRIVATE */

void box_wrapper_optim::objective1(const gifts& data) {
	/// (1). Minimise the sum of each box's length
	
	// initialise arrays
	box_lengths = IloNumVarArray(env, N, 0, L, ILOINT);
	
	// first, what are the lengths?
	for (size_t b = 0; b < N; ++b) {
		model.add(
			IloIfThen(env, (R(b) == 0), (Le(b) == data.all_boxes[b].l))
		);
		model.add(
			IloIfThen(env, (R(b) == 1), (Le(b) == data.all_boxes[b].w))
		);
	}
	
	model.add(
		IloMinimize(env, IloSum(box_lengths))
	);
}

void box_wrapper_optim::objective2(const gifts& data) {
	/// (2). Minimise the maximum top-left corner length coordinate
	
	// initialise arrays and vars
	box_coordinates = IloNumVarArray(env, N, 0, L, ILOINT);
	max_length = IloNumVar(env, 0, L, ILOINT);
	
	// first, what are the coordinates?
	for (int b = 0; b < N; ++b) {
		for (length i = 0; i < L; ++i) {
			for (width j = 0; j < W; ++j) {
				model.add(
					IloIfThen(env, (X(b,i,j) == 1), (S(b) == i))
				);
			}
		}
		
		model.add(max_length >= S(b));
	}
	
	model.add(
		IloMinimize(env, max_length)
	);
}


/* PROTECTED */

void box_wrapper_optim::add_objective(const gifts& data) {
	objective2(data);
}

/* PUBLIC */

box_wrapper_optim::box_wrapper_optim() : box_wrapper_rotate() {
	
}

box_wrapper_optim::box_wrapper_optim(const box_wrapper_optim& bw) : box_wrapper_rotate(bw) {
	
}

box_wrapper_optim::~box_wrapper_optim() {
}

