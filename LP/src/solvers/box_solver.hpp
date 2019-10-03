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

