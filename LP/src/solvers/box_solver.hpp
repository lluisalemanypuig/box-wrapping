#pragma once

// CPLEX
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

// Custom includes
#include "utils/wrapped_boxes.hpp"
#include "utils/definitions.hpp"
#include "utils/input_data.hpp"
#include "utils/logger.hpp"

class box_solver {
	private:
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
		
		virtual void _init(const gifts& gs) = 0;
		
	public:
		box_solver();
		virtual ~box_solver();
		
		/* FIND SOLUTION */
		
		void init(const gifts& gs, length max_L = -1);
		void solve();
		virtual void solution(const gifts& boxes_to_wrap, wrapped_boxes& wb) const = 0;
		
		/* MODIFIERS */
		
		void set_verbose(bool v = true);
		
		/* GETTERS */
		
		bool found_solution() const;
};

