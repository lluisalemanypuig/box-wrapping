#pragma once

// C includes
#include <gmp.h>

// C++ includes
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// Custom includes
#include "utils/global_info.hpp"
#include "clause_encoder.hpp"

namespace encoder {

class logarithmic_encoder : public clause_encoder {
	private:
		vector<string> binaries;
		bool created;
		int n_bits;
		
	private:
		logarithmic_encoder();
		~logarithmic_encoder();
		
		void make_binaries();
	
	public:
		static logarithmic_encoder& get_encoder() {
			static logarithmic_encoder ec;
			if (not ec.created) {
				ec.created = true;
				ec.make_binaries();
			}
			return ec;
		}
		
		void amo(const clause& C) const;
};

} // -- namespace encoder

