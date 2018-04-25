#include "parse_string.hpp"

namespace parse {
	
	void parse_uli(const char *ctxt, const char *str, unsigned long int *v, char *should_exit) {
		if (str == NULL) {
			fprintf(stderr, "Error when %s: input string is empty\n", ctxt);
			*should_exit = 1;
			return;
		}
		
		char *err;
		long s = strtol(str, &err, 10);
		
		/*
		according to the manual:
		
		If  no conversion is performed, zero is returned and the value of
		nptr is stored in the location referenced by endptr.
		*/
		if (err == str) {
			fprintf(stderr, "Error: no conversion of '%s' into unsigned int be performed\n", str);
			*should_exit = 1;
			return;
		}
		
		/*
		according to the manual:
		
		If the correct value would cause overflow, plus or minus LONG_MAX,
		LONG_MIN is returned (according  to  the  sign  of the value),
		and ERANGE is stored in errno.
		
		If the correct value would cause underflow, zero is returned and
		ERANGE is stored in errno.
		*/
		
		if (errno == ERANGE && s == LONG_MAX) {
			fprintf(stderr, "Error: value '%s' causes overflow\n", str);
			*should_exit = 1;
			return;
		}
		if (errno == ERANGE && s == LONG_MIN) {
			fprintf(stderr, "Error: value '%s' causes underflow\n", str);
			*should_exit = 1;
			return;
		}
		
		*should_exit = 0;
		*v = s;
	}

	void parse_double(const char *ctxt, const char *str, double *s, char *should_exit) {
		if (str == NULL) {
			fprintf(stderr, "Error when %s: input string is empty\n", ctxt);
			*should_exit = 1;
			return;
		}
		
		char *err;
		*s = strtod(str, &err);
		
		/*
		according to the manual:
		
		If  no conversion is performed, zero is returned and the value of
		nptr is stored in the location referenced by endptr.
		*/
		if (err == str) {
			fprintf(stderr, "Error: no conversion of '%s' into double could be performed\n", str);
			*should_exit = 1;
			return;
		}
		
		/*
		according to the manual:
		
		If the correct value would cause overflow, plus or minus HUGE_VAL
		(HUGE_VALF, HUGE_VALL) is returned (according  to  the  sign  of
		the value), and ERANGE is stored in errno.
		
		If the correct value would cause underflow, zero is returned and
		ERANGE is stored in errno.
		*/
		if (errno == ERANGE && (*s == HUGE_VAL || *s == HUGE_VALF || *s == HUGE_VALL)) {
			fprintf(stderr, "Error: value '%s' causes overflow\n", str);
			*should_exit = 1;
			return;
		}
		if (errno == ERANGE && *s == 0.0) {
			fprintf(stderr, "Error: value '%s' causes underflow\n", str);
			*should_exit = 1;
			return;
		}
		
		*should_exit = 0;
	}
	
}

