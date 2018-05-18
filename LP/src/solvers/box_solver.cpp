#include "box_solver.hpp"

/* PRIVATE */

void box_solver::config_cplex() {
	IloCplex::ParameterSet paramSet(cplex.getParameterSet());
	if (time_lim > 0) {
		paramSet.setParam(IloCplex::TiLim, time_lim);
	}
	paramSet.setParam(IloCplex::Threads, n_threads);
	cplex.setParameterSet(paramSet);
	
	cout << "config_cplex::should I be verbose? " << (verbose ? "yes" : "no") << endl;
	if (verbose) {
		cplex.setOut(cout);
		cplex.setWarning(cerr);
	}
	else {
		cout << "config_cplex::    Don't be verbose then!" << endl;
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
box_solver::~box_solver() {}

/* FIND SOLUTION */

void box_solver::init(const gifts& data, length max_L) {
	// Initialise CPLEX variables needed to create the model
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
	cout << "set_verbose::should I be verbose? " << (verbose ? "yes" : "no") << endl;
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

