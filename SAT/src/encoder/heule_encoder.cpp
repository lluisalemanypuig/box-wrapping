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

#include "heule_encoder.hpp"

namespace encoder {

/* PRIVATE */

heule_encoder::heule_encoder() : clause_encoder() { }
heule_encoder::~heule_encoder() { }

/* PUBLIC */

void heule_encoder::amo(const clause& C, ostream& out) const {
	if (C.size() <= 3) {
		quadratic_encoder& qe = quadratic_encoder::get_encoder();
		qe.amo(C, out);
		return;
	}
	
	clause c1, c2;
	C.split(2, c1, c2);
	
	global_info& gI = global_info::get_info();
	
	var new_var = gI.get_new_var();
	
	c1 +=  new_var; amo(c1, out);
	c2 += -new_var; amo(c2, out);
}

} // -- namespace encoder

