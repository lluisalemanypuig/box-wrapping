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
	
	bool heuristic_dec
	(
		Options& ops,
		int stop_at,
		bool stop_time, double stop_when,
		bool verbose,
		const gifts& input_data,
		wrapped_boxes& BEST, bool& error
	)
	{
		gifts data_copy = input_data;
		
		/// DECREASING ORDER
		
		// by area
		data_copy.sort_area_dec();
		if (verbose) {
			cout << "Decreasing area preprocessing" << endl;
			cout << data_copy << endl;
		}
		bool area = heur_private::call_solver(ops, data_copy, stop_at, stop_time, stop_when, verbose, BEST, error);
		
		// by width
		data_copy.sort_width_dec();
		if (verbose) {
			cout << "Decreasing width preprocessing" << endl;
			cout << data_copy << endl;
		}
		bool width = heur_private::call_solver(ops, data_copy, stop_at, stop_time, stop_when, verbose, BEST, error);
		
		return area or width;
	}
	
}

