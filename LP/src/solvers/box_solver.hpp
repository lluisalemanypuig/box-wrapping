#pragma once

// CPLEX
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

// Custom includes
#include "utils/wrapped_boxes.hpp"
#include "utils/definitions.hpp"
#include "utils/input_data.hpp"
#include "utils/logger.hpp"
using namespace utils;

namespace solver {

class box_solver {
	private:
		void config_cplex();
		
	protected:
		/* CPLEX variables */
		IloEnv env;
		IloModel model;
		IloCplex cplex;
		
		/* Other variables */
		
		// Is instance solved?
		bool is_solved;
		// Amount of boxes
		int N;
		// upper bound on the roll's length
		length L;
		// roll's width. Fixed value
		width W;
		
		/* ------ */
		// Configuration attributes
		bool verbose;
		double time_lim; // in seconds
		int n_threads;
		
	protected:
		virtual void add_objective(const gifts& gs) = 0;
		virtual void _init(const gifts& gs) = 0;
		
	public:
		box_solver();
		virtual ~box_solver();
		
		/* FIND SOLUTION */
		
		void init(const gifts& gs, length max_L = -1);
		void solve();
		virtual void solution(const gifts& boxes_to_wrap, wrapped_boxes& wb) const = 0;
		
		/* MODIFIERS */
		
		void set_verbose(bool v);
		void set_time_limit(double s);		// time limit in seconds
		void set_n_threads(int n_threads);
		
		/* GETTERS */
		
		bool found_solution() const;
		double get_exe_time() const;
		IloCplex::Status get_status() const;
};

} // -- namespace solver

