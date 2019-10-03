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

// C includes
#include <assert.h>

// C++ includes
#include <vector>
using namespace std;

namespace encoder {

typedef int literal;
typedef size_t var;

class clause {
	private:
		vector<literal> vars;
		
	public:
		clause();
		~clause();
		
		clause& operator+= (literal lit);
		int operator[] (int k) const;
		
		typedef vector<literal>::iterator clause_it;
		typedef vector<literal>::const_iterator clause_cit;
		
		clause_it begin();
		clause_cit begin() const;
		
		clause_it end();
		clause_cit end() const;
		
		size_t size() const;
		void split(int nv, clause& c1, clause& c2) const;
};

static inline
var get_var(literal lit) {
	return (lit > 0 ? lit : -lit);
}

} // -- namespace solver

