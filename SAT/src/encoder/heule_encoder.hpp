#pragma once

// C++ includes
#include <iostream>
using namespace std;

// Custom includes
#include "utils/global_info.hpp"
#include "clause_encoder.hpp"

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
		
		void amo(const clause& C) const;
};

} // -- namespace encoder

