/// C includes
#include <string.h>

/// C++ includes
#include <iostream>
using namespace std;

void print_usage() {
	cout << "BOX WRAPPING PROBLEM - Constraint Programming" << endl;
	cout << "Parameters:" << endl;
	cout << "    [-h, --help]: show the usage" << endl;
}

int main(int argc, char *argv[]) {
	
	
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0) {
			print_usage();
			return 0;
		}
	}
	
	
}

