#include "quadratic_encoder.hpp"

namespace encoder {

/* PRIVATE */

quadratic_encoder::quadratic_encoder() : clause_encoder() { }
quadratic_encoder::~quadratic_encoder() { }
		
/* PUBLIC */

void quadratic_encoder::amo(const clause& C) const {
	const int k = C.size();
	int nC = 0;
	
	for (int i = 0; i < k; ++i) {
		for (int j = i + 1; j < k; ++j) {
			cout << -C[i] << " " << -C[j] << " 0" << endl;
			
			++nC;
		}
	}
	
	global_info& gI = global_info::get_info();
	gI.add_clauses(nC);
}

} // -- namespace encoder

