#include "box_wrapper_optim.hpp"

/* PRIVATE */

void box_wrapper_optim::objective1(const gifts& data) {
	/// (1). Minimise the sum of each box's length
	
	// initialise arrays
	box_lengths = IloNumVarArray(env, N, 0, IloInfinity, ILOINT);
	
	// first, what are the lengths?
	for (size_t b = 0; b < N; ++b) {
		model.add(
			IloIfThen(env, (R(b) == 0), (L(b) == data.all_boxes[b].l))
		);
		model.add(
			IloIfThen(env, (R(b) == 1), (L(b) == data.all_boxes[b].w))
		);
	}
	
	model.add(
		IloMinimize(env, IloSum(box_lengths))
	);
}

void box_wrapper_optim::objective2(const gifts& data) {
	/// (2). Minimise the maximum top-left corner length coordinate
	
	
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

