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
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <stdio.h>
#include <math.h>

// Custom includes
#include "definitions.hpp"

namespace parse {
	
	/* In both functions context is a string containing a VERY short
	 * description of the situation in which we are parsing the string
	 * str. E.g., ff 'str' must contain the number of threads, context
	 * should ideally containg 'parsing the number of threads'.
	 */
	
	void parse_long(const char *context, const char *str, int *v, char *should_exit);
	void parse_double(const char *context, const char *str, double *s, char *should_exit);
	
}

