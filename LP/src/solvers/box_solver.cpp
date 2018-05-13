#include "box_solver.hpp"

box_solver::box_solver() : is_solved(false) { }
box_solver::~box_solver() {}

bool box_solver::found_solution() const {
	return is_solved;
}
