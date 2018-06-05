#include "to_binary.hpp"

namespace utils {

	string to_binary(unsigned int k) {
		if (k == 0) return "0";
		if (k == 1) return "1";
		
		string dig;
		if ((k&0x1) == 1) dig = "1";
		else dig = "0";
		
		string kh = to_binary(k >> 1);
		return kh + dig;
	}

} // -- namespace utils

