/*********************************************************************
 * Box Wrapping solver
 * Copyright (C) 2018  Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

#include "box_solver.hpp"

namespace solver {

/* PRIVATE */

void box_solver::config_cplex() {
	IloCplex::ParameterSet paramSet(cplex.getParameterSet());
	if (time_lim > 0) {
		paramSet.setParam(IloCplex::TiLim, time_lim);
	}
	paramSet.setParam(IloCplex::Threads, n_threads);
	cplex.setParameterSet(paramSet);
	
	if (verbose) {
		cplex.setOut(cout);
		cplex.setWarning(cerr);
	}
	else {
		cplex.setOut(env.getNullStream());
		cplex.setWarning(env.getNullStream());
	}
}

/* PUBLIC */

box_solver::box_solver() : is_solved(false) {
	verbose = false;
	time_lim = -1;
	n_threads = 1;
}
box_solver::~box_solver() {
	env.end();
}

/* FIND SOLUTION */

void box_solver::init(const gifts& data, length max_L) {
	// Initialise CPLEX variables needed to create the model
	env.end();
	
	env = IloEnv();
	model = IloModel(env);
	cplex = IloCplex(model);
	
	config_cplex();
	
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
	verbose = v;
}

void box_solver::set_time_limit(double s) {
	time_lim = s;
}

void box_solver::set_n_threads(int nt) {
	n_threads = nt;
	if (n_threads < 0) {
		n_threads = 1;
	}
}

/* GETTERS */

bool box_solver::found_solution() const {
	return is_solved;
}

double box_solver::get_exe_time() const {
	return cplex.getTime();
}

IloCplex::Status box_solver::get_status() const {
	return cplex.getCplexStatus();
}

} // -- namespace solver

