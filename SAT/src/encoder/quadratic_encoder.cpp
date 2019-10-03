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

#include "quadratic_encoder.hpp"

namespace encoder {

/* PRIVATE */

quadratic_encoder::quadratic_encoder() : clause_encoder() { }
quadratic_encoder::~quadratic_encoder() { }
		
/* PUBLIC */

void quadratic_encoder::amo(const clause& C, ostream& out) const {
	const int k = C.size();
	int nC = 0;
	
	for (int i = 0; i < k; ++i) {
		for (int j = i + 1; j < k; ++j) {
			out << -C[i] << " " << -C[j] << " 0" << endl;
			
			++nC;
		}
	}
	
	global_info& gI = global_info::get_info();
	gI.add_clauses(nC);
}

} // -- namespace encoder

