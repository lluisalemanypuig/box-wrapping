// C includes
#include <string.h>

// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

// Custom includes
#include "utils/wrapped_boxes.hpp"
#include "utils/parse_string.hpp"
#include "utils/input_data.hpp"
#include "utils/time.hpp"
using namespace utils;

void print_usage() {
	cout << "BOX WRAPPING PROBLEM - Satisfiability (CNF) Solver" << endl;
	cout << endl;
	cout << "This software uses SAT solver lingeling [1] to solve the Box Wrapping" << endl;
	cout << "Problem (BWP). It admits several options and are described below. Most" << endl;
	cout << "of them are optional but it requires the specification of an input file" << endl;
	cout << "containing the data, the solver that will be used, and its mode of execution," << endl;
	cout << "in case it has any." << endl;
	cout << "This will only produce the clauses in conjunctive normal form required" << endl;
	cout << "to solve the problem. The format of the clauses output were tested only" << endl;
	cout << "for lingeling." << endl;
	cout << "The constraints are output to the standard output." << endl;
	cout << "[1]: https://github.com/arminbiere/lingeling" << endl;
	cout << endl;
	cout << "Parameters:" << endl;
	cout << "    [-h, --help]: show the usage" << endl;
	cout << "    [-i, --input]: specify the input file" << endl;
	cout << endl;
}

int main(int argc, char *argv[]) {
	string infile = "none";
	
	// process arguments
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0) {
			print_usage();
			return 0;
		}
		else if (strcmp(argv[i], "-i") == 0 or strcmp(argv[i], "--input") == 0) {
			infile = string(argv[i + 1]);
			++i;
		}
		else {
			cerr << "Error: option '" << string(argv[i]) << "' not recognised" << endl;
			return 1;
		}
	}
	
	/// <ERROR CONTROL>
	if (infile == "none") {
		cerr << "Error: input file not specified." << endl;
		cerr << "    Use [-h, --help] to see the usage for details." << endl;
		return 1;
	}
	
	/// <READ INPUT DATA>
	ifstream fin;
	fin.open(infile.c_str());
	if (not fin.is_open()) {
		cerr << "Error: could not open file '" << infile << "'" << endl;
		return 1;
	}
	
	gifts INPUT;
	fin >> INPUT;
	fin.close();
	/// </READ INPUT DATA>
	
	INPUT.fill_fields();
	
	
	return 0;
}

