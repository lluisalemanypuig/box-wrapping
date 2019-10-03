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

#include "heuristics.hpp"

namespace heuristics {
	
	void heuristic_chooser
	(
		bool heuris_inc, bool heuris_dec, bool heuris_rand, bool heuris_mix,
		int n_threads, int rand_times,
		int stop_at, bool stop_time, double stop_when,
		bool verbose,
		const gifts& input_data,
		wrapped_boxes& solution, int& n_sols,
		bool& error
	)
	{
		Options ops = Options::def;
		ops.threads = n_threads;
		
		if (heuris_inc) {
			n_sols = heuristic_inc(ops, stop_at, stop_time, stop_when, verbose, input_data, solution, error);
		}
		else if (heuris_dec) {
			n_sols = heuristic_dec(ops, stop_at, stop_time, stop_when, verbose, input_data, solution, error);
		}
		else if (heuris_rand) {
			n_sols = heuristic_rand(ops, rand_times, stop_at, stop_time, stop_when, verbose, input_data, solution, error);
		}
		else if (heuris_mix) {
			n_sols = heuristic_mix(ops, rand_times, stop_at, stop_time, stop_when, verbose, input_data, solution, error);
		}
	}
	
}

