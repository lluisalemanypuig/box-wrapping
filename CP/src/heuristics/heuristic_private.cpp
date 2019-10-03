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
namespace heur_private {
	
	bool call_solver
	(
		Options& ops, const gifts& data,
		int stop_at,
		bool stop_time, double stop_when,
		bool verbose,
		wrapped_boxes& BEST, bool& error
	)
	{
		if (stop_time) {
			ops.stop = Stop::time( (unsigned long int)(1000.0*stop_when) );
		}
		
		int n_sols;
		box_wrapper_optim *boxes = nullptr;
		box_wrapper_optim *sol;
		
		boxes = new box_wrapper_optim(data, BEST.get_roll_length() - 1);
		BAB<box_wrapper_optim> INC_SOLV(boxes, ops);
		delete boxes;
		
		n_sols = 0;
		while (n_sols < stop_at and (sol = INC_SOLV.next()) and not error) {
			wrapped_boxes wb;
			sol->to_wrapped_boxes(data, wb);
			
			#ifdef BOX_DEBUG
			string msg;
			bool sane = wb.is_sane(msg);
			if (not sane) {
				cout << "Solution not sane!" << endl;
				cout << wb << endl << endl;
				cout << msg << endl;
				error = true;
			}
			#endif
			
			if (wb < BEST) {
				BEST = wb;
			}
			
			if (verbose) {
				cout << n_sols + 1 << ")" << endl;
				cout << wb << endl;
				cout << endl;
			}
			
			++n_sols;
		}
		
		return n_sols > 0;
	}
	
}
}

