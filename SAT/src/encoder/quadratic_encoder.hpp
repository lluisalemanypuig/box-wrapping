/*********************************************************************
 * Box Wrapping solver
 * Copyright (C) 2018  Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

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

