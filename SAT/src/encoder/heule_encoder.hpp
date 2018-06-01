#pragma once

// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

// Custom includes
#include "utils/global_info.hpp"
#include "quadratic_encoder.hpp"
#include "clause_encoder.hpp"
using namespace utils;

namespace encoder {

class heule_encoder : public clause_encoder {
	private:
		heule_encoder();
		~heule_encoder();
	
	public:
		static heule_encoder& get_encoder() {
			static heule_encoder ec;
			return ec;
		}
		
		void amo(const clause& C, ostream& out) const;
};

} // -- namespace encoder

