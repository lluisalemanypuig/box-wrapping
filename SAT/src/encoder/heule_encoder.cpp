#include "heule_encoder.hpp"

namespace encoder {

/* PRIVATE */

heule_encoder::heule_encoder() : clause_encoder() { }
heule_encoder::~heule_encoder() { }

/* PUBLIC */

void heule_encoder::amo(const clause& C, ostream& out) const {
	if (C.size() <= 3) {
		quadratic_encoder& qe = quadratic_encoder::get_encoder();
		qe.amo(C, out);
		return;
	}
	
	clause c1, c2;
	C.split(2, c1, c2);
	
	global_info& gI = global_info::get_info();
	
	var new_var = gI.get_new_var();
	
	c1 +=  new_var; amo(c1, out);
	c2 += -new_var; amo(c2, out);
}

} // -- namespace encoder

