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
	int n_extra_vars = n_bits;
	
	mpz_t binary;
	mpz_init(binary);
	binaries = vector<string>(n_vars);
	
	for (size_t i = 0; i < n_vars; ++i) {
		mpz_set_ui(binary, i);
		char *buf = nullptr;
		buf = mpz_get_str(buf, 2, binary);
		
		string small_bin = string(buf);
		binaries[i] = string(n_bits - small_bin.size(), '0') + small_bin;
		
		free(buf);
	}
	
	mpz_clear(binary);
}

void logarithmic_encoder::amo(const clause& C) const {
	global_info& gI = global_info::get_info();
	int vars_so_far = gI.get_total_vars();
	gI.add_vars(n_bits);
	
	const int k = C.size();
	
	for (literal lit : C) {
		const string& binary = binaries[get_var(lit) - 1];
		
		for (int b = 0; b < binary.size(); ++b) {
			
			if (binary[n_bits - 1 - b] == '0') {
				cout << -lit << " " << -(vars_so_far + b + 1);
			}
			else {
				cout << -lit << " " <<   vars_so_far + b + 1;
			}
			cout << " 0" << endl;
		}
	}
	
	gI.add_clauses(C.size()*n_bits);
}

};

} // -- namespace encoder

