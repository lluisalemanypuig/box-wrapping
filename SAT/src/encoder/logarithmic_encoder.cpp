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

#include "logarithmic_encoder.hpp"

namespace encoder {

/* PRIVATE */
logarithmic_encoder::logarithmic_encoder() : clause_encoder() {
	created = false;
	n_bits = 0;
}
logarithmic_encoder::~logarithmic_encoder() { }

/* PUBLIC */

void logarithmic_encoder::make_binaries() {
	global_info& gI = global_info::get_info();
	int n_vars = gI.get_model_vars();
	
	n_bits = ceil(log(1.0*n_vars)/log(2.0));
	binaries = vector<string>(n_vars);
	
	for (int i = 0; i < n_vars; ++i) {
		string small_bin = to_binary(i);
		
		// leading zeros + minimal binary representation
		binaries[i] = string(n_bits - small_bin.size(), '0') + small_bin;
	}
}

void logarithmic_encoder::amo(const clause& C, ostream& out) const {
	global_info& gI = global_info::get_info();
	int vars_so_far = gI.get_total_vars();
	
	for (literal lit : C) {
		const string& binary = binaries[get_var(lit) - 1];
		
		for (int b = 0; b < n_bits; ++b) {
			
			if (binary[n_bits - 1 - b] == '0') {
				out << -lit << " " << -(vars_so_far + b + 1);
			}
			else {
				out << -lit << " " <<   vars_so_far + b + 1;
			}
			out << " 0" << endl;
		}
	}
	
	gI.add_clauses(C.size()*n_bits);
	gI.add_vars(n_bits);
}

} // -- namespace encoder

