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

#pragma once

namespace utils {

class global_info {
	private:
		int n_model_vars;
		int n_ext_vars;
		int n_clauses;
		
		global_info();
		~global_info();
		
	public:
		static global_info& get_info() {
			static global_info gI;
			return gI;
		}
		
		/* MODIFIERS */
		
		void set_model_vars(int n);
		void add_vars(int n);
		void add_clauses(int n);
		
		/* GETTERS */
		
		int get_new_var();
		int get_model_vars() const;
		int get_total_vars() const;
		int get_total_clauses() const;
};
	
} // -- namespace utils

