#pragma once

/// C includes
#include <stdlib.h>
#include <time.h>

/// C++ includes
#include <vector>
using namespace std;

/// Gecode
#include <gecode/search.hh>
using namespace Gecode;
using namespace Search;

/// Custom includes
#include "utils/wrapped_boxes.hpp"
#include "utils/input_data.hpp"
#include "solvers/box_wrapper_optim.hpp"

namespace heuristics {
	
	namespace heur_private {
		bool call_solver
		(
			Options& ops, const gifts& data,
			size_t stop_at,
			bool stop_time, double stop_when,
			bool verbose,
			wrapped_boxes& BEST, bool& error
		);
	}
	
	bool heuristic_inc
	(
		Options& search_options,
		size_t stop_at,
		bool stop_time, double stop_when,
		bool verbose,
		const gifts& input_data,
		wrapped_boxes& solution, bool& error
	);
	
	bool heuristic_dec
	(
		Options& search_options,
		size_t stop_at,
		bool stop_time, double stop_when,
		bool verbose,
		const gifts& input_data,
		wrapped_boxes& solution, bool& error
	);
	
	bool heuristic_rand
	(
		Options& search_options,
		size_t rand_times, size_t stop_at,
		bool stop_time, double stop_when,
		bool verbose,
		const gifts& input_data,
		wrapped_boxes& solution, bool& error
	);
	
	bool heuristic_mix
	(
		Options& search_options,
		size_t rand_times, size_t stop_at,
		bool stop_time, double stop_when,
		bool verbose,
		const gifts& input_data,
		wrapped_boxes& solution, bool& error
	);
	
	void heuristic_chooser
	(
		bool heuris_inc, bool heuris_dec, bool heuris_rand, bool heuris_mix,
		size_t n_threads, size_t rand_times,
		size_t stop_at, bool stop_time, double stop_when,
		bool verbose,
		const gifts& input_data,
		wrapped_boxes& solution, size_t& n_sols,
		bool& error
	);
	
}

