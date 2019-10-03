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

