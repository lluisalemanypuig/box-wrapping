#include "clause_encoder.hpp"

namespace encoder {

/* PRIVATE */

clause_encoder::clause_encoder() { }
clause_encoder::~clause_encoder() { }

/* PUBLIC */

void clause_encoder::alo(const clause& C) const {
	for (literal lit : C) {
		cout << lit << " ";
	}
	cout << 0 << endl;
	
	global_info& gI = global_info::get_info();
	gI.add_clauses(1);
}

void clause_encoder::exactly_one(const clause& C) const {
	alo(C);
	amo(C);
}

} // -- namespace encoder

