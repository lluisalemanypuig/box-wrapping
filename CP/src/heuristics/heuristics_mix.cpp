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
	
	bool heuristic_mix
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
		
		/// INCREASING ORDER
		// by area
		data_copy.sort_area_inc();
		if (verbose) {
			cout << "Increasing area preprocessing" << endl;
			cout << data_copy << endl;
		}
		bool sol_area_inc = heur_private::call_solver(ops, data_copy, 3, stop_time, stop_when, verbose, BEST, error);
		// by width
		data_copy.sort_width_inc();
		if (verbose) {
			cout << "Increasing width preprocessing" << endl;
			cout << data_copy << endl;
		}
		bool sol_width_inc = heur_private::call_solver(ops, data_copy, 3, stop_time, stop_when, verbose, BEST, error);
		
		/// DECREASING ORDER
		// by area
		data_copy.sort_area_dec();
		if (verbose) {
			cout << "Decreasing area preprocessing" << endl;
			cout << data_copy << endl;
		}
		bool sol_area_dec = heur_private::call_solver(ops, data_copy, 3, stop_time, stop_when, verbose, BEST, error);
		// by width
		data_copy.sort_width_dec();
		if (verbose) {
			cout << "Decreasing width preprocessing" << endl;
			cout << data_copy << endl;
		}
		bool sol_width_dec = heur_private::call_solver(ops, data_copy, 3, stop_time, stop_when, verbose, BEST, error);
		
		/// RANDOM
		srand(time(NULL)); // seed rng only once
		bool sol_rand = false;
		
		for (int i = 0; i < rand_times; ++i) {
			data_copy.scramble();
			if (verbose) {
				cout << "----------------------------" << endl;
				cout << "Random preprocessing " << i + 1 << ")" << endl;
				cout << data_copy << endl;
			}
			bool solr = heur_private::call_solver(ops, data_copy, stop_at, stop_time, stop_when, verbose, BEST, error);
			sol_rand = sol_rand or solr;
		}
		
		return sol_area_inc or sol_width_inc or sol_area_dec or sol_width_dec or sol_rand;
	}
	
}

