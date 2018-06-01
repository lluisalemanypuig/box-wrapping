#pragma once

// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

// Custom includes
#include "utils/global_info.hpp"
#include "clause_encoder.hpp"
using namespace utils;

namespace encoder {

class quadratic_encoder : public clause_encoder {
	private:
		quadratic_encoder();
		~quadratic_encoder();
		
	public:
		static quadratic_encoder& get_encoder() {
			static quadratic_encoder ec;
			return ec;
		}
		
		void amo(const clause& C, ostream& out) const;
};

} // -- namespace encoder

