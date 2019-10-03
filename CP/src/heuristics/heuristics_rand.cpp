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
	
	bool heuristic_rand
	(
		Options& ops,
		int rand_times, int stop_at,
		bool stop_time, double stop_when,
		bool verbose,
		const gifts& input_data,
		wrapped_boxes& BEST, bool& error
	)
	{
		gifts data_copy = input_data;
		
		/// RANDOM ORDER
		
		// seed rng only once
		srand(time(NULL));
		bool sol = false;
		
		for (int i = 0; i < rand_times; ++i) {
			data_copy.scramble();
			if (verbose) {
				cout << "----------------------------" << endl;
				cout << "Random preprocessing " << i + 1 << ")" << endl;
				cout << data_copy << endl;
			}
			bool solr = heur_private::call_solver(ops, data_copy, stop_at, stop_time, stop_when, verbose, BEST, error);
			sol = sol or solr;
		}
		
		return sol;
	}
	
}

