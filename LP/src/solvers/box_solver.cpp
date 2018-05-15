#include "box_solver.hpp"

box_solver::box_solver() : is_solved(false) { }
box_solver::~box_solver() {}

/* FIND SOLUTION */

void box_solver::init(const gifts& data, length max_L) {
	// Initialise CPLEX variables needed to create the model
	env = IloEnv();
	model = IloModel(env);
	cplex = IloCplex(model);
	
	if (max_L < 0) {
		L = inf_t<length>();
	}
	else {
		L = max_L;
	}
	
	N = data.total_boxes;
	W = data.W;
	L = min(L, data.get_max_length_s());
	
	_init(data);
}

void box_solver::solve() {
	is_solved = cplex.solve();
}

/* MODIFIERS */

void box_solver::set_verbose(bool v) {
	if (v) {
		cplex.setOut(cout);
		cplex.setWarning(cerr);
	}
	else {
		cplex.setOut(env.getNullStream());
		cplex.setWarning(env.getNullStream());
	}
}

/* GETTERS */

bool box_solver::found_solution() const {
	return is_solved;
}

