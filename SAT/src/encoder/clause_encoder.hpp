#pragma once

// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

// Custom includes
#include "utils/global_info.hpp"
#include "clause.hpp"
using namespace utils;

namespace encoder {

enum encoder_type {
	none, quadratic, logarithmic, heule
};

class clause_encoder {
	protected:
		clause_encoder();
		virtual ~clause_encoder();
		
	public:
		virtual void amo(const clause& C, ostream& out) const = 0;
		
		void alo(const clause& C, ostream& out) const;
		void exactly_one(const clause& C, ostream& out) const;
		
		clause_encoder(const clause_encoder& ec) = delete;
		void operator= (const clause_encoder& L) = delete;
};

} // -- namespace encoder

