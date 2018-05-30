#include "clause.hpp"

namespace encoder {

/* PRIVATE */

clause::clause() {}
clause::~clause() {}

/* PUBLIC */

clause& clause::operator+= (literal lit) {
	vars.push_back(lit);
}

int clause::operator[] (int k) const {
	return vars[k];
}

clause_it clause::begin() { return vars.begin(); }
clause_cit clause::begin() const { return vars.begin(); }

clause_it clause::end() { return vars.end(); }
clause_cit clause::end() const { return vars.end(); }

size_t size() clause::const { return vars.size(); }

void clause::split(int nv, clause& c1, clause& c2) const {
	assert(vars.size() > nv);
	
	c1.vars = vector<literal>(vars.begin(), vars.begin() + nv);
	c2.vars = vector<literal>(vars.begin() + nv, vars.end());
}
	
} // -- namespace solver

