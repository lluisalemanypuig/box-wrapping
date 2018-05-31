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

