#include "box_wrapper_optim.hpp"

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

box_wrapper_optim::box_wrapper_optim() : box_wrapper_rotate() {
	
}

box_wrapper_optim::box_wrapper_optim(const box_wrapper_optim& bw) : box_wrapper_rotate(bw) {
	
}

box_wrapper_optim::~box_wrapper_optim() {
}

