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

// C includes
#include <assert.h>

// C++ includes
#include <iostream>
using namespace std;

// Gecode
#include <gecode/minimodel.hh>
#include <gecode/search.hh>
#include <gecode/int.hh>
using namespace Gecode;

// Custom includes
#include "box_wrapper_rotate.hpp"
#include "utils/wrapped_boxes.hpp"
#include "utils/definitions.hpp"
#include "utils/input_data.hpp"
#include "utils/logger.hpp"
using namespace utils;

namespace solver {

class box_wrapper_optim : public box_wrapper_rotate {
	protected:
		gifts data;
		
	public:
		box_wrapper_optim(const gifts& gs, length max_L = -1);
		box_wrapper_optim(box_wrapper_optim& bw);
		virtual ~box_wrapper_optim();
		
		virtual Space *copy();
		
		// imposes that the length of roll used is as small as possible
		virtual void constrain(const Space& _b);
};

} // -- namespace solver

