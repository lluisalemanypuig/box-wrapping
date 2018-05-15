#include "box_solver.hpp"

box_solver::box_solver() : is_solved(false) { }
box_solver::~box_solver() {}

/* FIND SOLUTION */

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

