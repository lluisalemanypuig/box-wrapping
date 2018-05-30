#pragma once

// C++ includes
#include <iostream>
using namespace std;

// Custom includes
#include "utils/global_info.hpp"
#include "clause.hpp"

namespace encoder {

class clause_encoder {
	protected:
		clause_encoder();
		virtual ~clause_encoder();
		
	public:
		virtual void amo(const clause& C) const = 0;
		
		void alo(const clause& C) const;
		void exactly_one(const clause& C) const;
		
		clause_encoder(const clause_encoder& ec) = delete;
		void operator= (const clause_encoder& L) = delete;
};

} // -- namespace encoder

