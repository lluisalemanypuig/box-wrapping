#include "logarithmic_encoder.hpp"

namespace encoder {

/* PRIVATE */
logarithmic_encoder::logarithmic_encoder() : clause_encoder() {
	created = false;
	n_bits = 0;
}
logarithmic_encoder::~logarithmic_encoder() { }

/* PUBLIC */

void logarithmic_encoder::make_binaries() {
	global_info& gI = global_info::get_info();
	int n_vars = gI.get_model_vars();
	
	n_bits = ceil(log(1.0*n_vars)/log(2.0));
	binaries = vector<string>(n_vars);
	
	for (int i = 0; i < n_vars; ++i) {
		string small_bin = to_binary(i);
		
		// leading zeros + minimal binary representation
		binaries[i] = string(n_bits - small_bin.size(), '0') + small_bin;
	}
}

void logarithmic_encoder::amo(const clause& C, ostream& out) const {
	global_info& gI = global_info::get_info();
	int vars_so_far = gI.get_total_vars();
	
	for (literal lit : C) {
		const string& binary = binaries[get_var(lit) - 1];
		
		for (int b = 0; b < n_bits; ++b) {
			
			if (binary[n_bits - 1 - b] == '0') {
				out << -lit << " " << -(vars_so_far + b + 1);
			}
			else {
				out << -lit << " " <<   vars_so_far + b + 1;
			}
			out << " 0" << endl;
		}
	}
	
	gI.add_clauses(C.size()*n_bits);
	gI.add_vars(n_bits);
}

} // -- namespace encoder

