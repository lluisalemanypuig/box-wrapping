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

#include "global_info.hpp"

namespace utils {

/* PRIVATE */

global_info::global_info() {
	n_model_vars = 0;
	n_ext_vars = 0;
	n_clauses = 0;
}
global_info::~global_info() { }

/* PUBLIC */

void global_info::set_model_vars(int n) { n_model_vars = n; }
void global_info::add_vars(int n)		{ n_ext_vars += n; }
void global_info::add_clauses(int n)	{ n_clauses += n; }

int global_info::get_new_var() {
	++n_ext_vars;
	return n_model_vars + n_ext_vars;
}

int global_info::get_model_vars() const		{ return n_model_vars; }
int global_info::get_total_vars() const		{ return n_model_vars + n_ext_vars; }
int global_info::get_total_clauses() const	{ return n_clauses; }

} // -- namespace utils

