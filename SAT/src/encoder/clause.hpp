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

