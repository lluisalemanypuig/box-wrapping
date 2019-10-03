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

// C includes
#include <stdlib.h>
#include <time.h>

// C++ includes
#include <vector>
using namespace std;

// Gecode
#include <gecode/search.hh>
using namespace Gecode;
using namespace Search;

// Custom includes
#include "solvers/box_wrapper_optim.hpp"
#include "utils/wrapped_boxes.hpp"
#include "utils/input_data.hpp"
#include "utils/definitions.hpp"
using namespace utils;
using namespace solver;

namespace heuristics {
	
	namespace heur_private {
		bool call_solver
		(
			Options& ops, const gifts& data,
			int stop_at,
			bool stop_time, double stop_when,
			bool verbose,
			wrapped_boxes& BEST, bool& error
		);
	}
	
	bool heuristic_inc
	(
		Options& search_options,
		int stop_at,
		bool stop_time, double stop_when,
		bool verbose,
		const gifts& input_data,
		wrapped_boxes& solution, bool& error
	);
	
	bool heuristic_dec
	(
		Options& search_options,
		int stop_at,
		bool stop_time, double stop_when,
		bool verbose,
		const gifts& input_data,
		wrapped_boxes& solution, bool& error
	);
	
	bool heuristic_rand
	(
		Options& search_options,
		int rand_times, int stop_at,
		bool stop_time, double stop_when,
		bool verbose,
		const gifts& input_data,
		wrapped_boxes& solution, bool& error
	);
	
	bool heuristic_mix
	(
		Options& search_options,
		int rand_times, int stop_at,
		bool stop_time, double stop_when,
		bool verbose,
		const gifts& input_data,
		wrapped_boxes& solution, bool& error
	);
	
	void heuristic_chooser
	(
		bool heuris_inc, bool heuris_dec, bool heuris_rand, bool heuris_mix,
		int n_threads, int rand_times,
		int stop_at, bool stop_time, double stop_when,
		bool verbose,
		const gifts& input_data,
		wrapped_boxes& solution, int& n_sols,
		bool& error
	);
	
}

