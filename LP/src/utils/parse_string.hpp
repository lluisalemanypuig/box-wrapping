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
