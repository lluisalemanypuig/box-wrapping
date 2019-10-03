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

#include "clause.hpp"

namespace encoder {

/* PRIVATE */

clause::clause() {}
clause::~clause() {}

/* PUBLIC */

clause& clause::operator+= (literal lit) {
	vars.push_back(lit);
	return *this;
}

int clause::operator[] (int k) const {
	return vars[k];
}

clause::clause_it clause::begin() { return vars.begin(); }
clause::clause_cit clause::begin() const { return vars.begin(); }

clause::clause_it clause::end() { return vars.end(); }
clause::clause_cit clause::end() const { return vars.end(); }

size_t clause::size() const { return vars.size(); }

void clause::split(int nv, clause& c1, clause& c2) const {
	assert(vars.size() > nv);
	
	c1.vars = vector<literal>(vars.begin(), vars.begin() + nv);
	c2.vars = vector<literal>(vars.begin() + nv, vars.end());
}
	
} // -- namespace solver

