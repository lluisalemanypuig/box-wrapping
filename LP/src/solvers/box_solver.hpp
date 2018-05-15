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
		bool is_solved;
		
		// CPLEX variables
		IloEnv env;
		IloModel model;
		IloCplex cplex;
		
	public:
		box_solver();
		virtual ~box_solver();
		
		/* FIND SOLUTION */
		
		virtual void init(const gifts& gs, length max_L = -1) = 0;
		void solve();
		virtual void solution(const gifts& boxes_to_wrap, wrapped_boxes& wb) const = 0;
		
		/* MODIFIERS */
		
		void set_verbose(bool v = true);
		
		/* GETTERS */
		
		bool found_solution() const;
};

