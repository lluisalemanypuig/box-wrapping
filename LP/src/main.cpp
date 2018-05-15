/// C includes
#include <string.h>

/// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

/// CPLEX
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

/// Custom includes
#include "utils/wrapped_boxes.hpp"
#include "utils/parse_string.hpp"
#include "utils/input_data.hpp"
#include "utils/time.hpp"
#include "solvers/box_solver.hpp"
#include "solvers/box_wrapper_simple.hpp"
#include "solvers/box_wrapper_rotate.hpp"
#include "solvers/box_wrapper_optim.hpp"

void print_usage() {
	cout << "BOX WRAPPING PROBLEM - Linear Programming Solver" << endl;
	cout << endl;
	cout << "This software uses the library CPLEX (tested under version 12.7)" << endl;
	cout << "to solve the Box Wrapping Problem (BWP). It admits several options" << endl;
	cout << "and are described below. Most of them are optional but it requires" << endl;
	cout << "the specification of an input file containing the data, the solver" << endl;
	cout << "that will be used, and its mode of execution, in case it has any." << endl;
	cout << endl;
	cout << "Parameters:" << endl;
	cout << "    [-h, --help]: show the usage" << endl;
	cout << "    [-i, --input]: specify the input file" << endl;
	cout << "    [-o, --output]: specify the output file." << endl;
	cout << "        Only the first solution will be stored." << endl;
	cout << "    [-v, --verbose]: tell the CPLEX solver to output its progress." << endl;
	cout << "        Other messages will also be displayed" << endl;
	cout << endl;
	cout << "Solvers:" << endl;
	cout << "    (1) --simple: initial version of a solver in Gecode for the BWP" << endl;
	cout << "    (2) --rotate: simple solver extended to allow rotations" << endl;
	cout << "    (3) --optim:  solver that tries to minimise roll length used." << endl;
	cout << "        The solutions found, however, may not be optimal." << endl;
	cout << endl;
}

int main(int argc, char *argv[]) {
	string infile = "none";
	string outfile = "none";
	
	char fatal_error = 0;
	
	bool simple = false;
	bool rotate = false;
	bool optim = false;
	bool verbose = false;
	
	// process arguments
	for (int i = 1; i < argc; ++i) {
		/// MAIN ARGUMENTS
		if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0) {
			print_usage();
			return 0;
		}
		else if (strcmp(argv[i], "-i") == 0 or strcmp(argv[i], "--input") == 0) {
			infile = string(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "-o") == 0 or strcmp(argv[i], "--output") == 0) {
			outfile = string(argv[i + 1]);
			++i;
		}
		
		/// SOLVERS
		else if (strcmp(argv[i], "--simple") == 0) {
			simple = true;
		}
		else if (strcmp(argv[i], "--rotate") == 0) {
			rotate = true;
		}
		else if (strcmp(argv[i], "--optim") == 0) {
			optim = true;
		}
		else if (strcmp(argv[i], "-v") == 0 or strcmp(argv[i], "--verbose") == 0) {
			verbose = true;
		}
		else {
			cerr << "Error: option '" << string(argv[i]) << "' not recognised" << endl;
			return 1;
		}
		
		if (fatal_error == 1) {
			return 1;
		}
	}
	
	/// <ERROR CONTROL>
	if (infile == "none") {
		cerr << "Error: input file not specified." << endl;
		cerr << "    Use [-h, --help] to see the usage for details." << endl;
		return 1;
	}
	
	int solvers_selected = simple + rotate + optim;
	if (solvers_selected == 0) {
		cerr << "Error: no solver was selected. Choose exactly one." << endl;
		return 1;
	}
	if (solvers_selected > 1) {
		cerr << "Error: more than one was selected. Choose exactly one." << endl;
		return 1;
	}
	/// </ERROR CONTROL>
	
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
	
	box_solver *bs;
	if (simple) {
		bs = new box_wrapper_simple();
	}
	else if (rotate) {
		bs = new box_wrapper_rotate();
	}
	
	double begin = timing::now();
	
	bs->init(INPUT);
	bs->set_verbose(verbose);
	bs->solve();
	
	double end = timing::now();
	
	if (bs->found_solution()) {
		wrapped_boxes SOL;
		bs->solution(INPUT, SOL);
		
		#ifdef BOX_DEBUG
		string msg;
		bool sane = SOL.is_sane(msg);
		if (not sane) {
			cerr << "Solution is not sane!" << endl;
			cerr << msg << endl;
		}
		#endif
		
		if (verbose) {
			cout << SOL << endl;
			cout << "In " << timing::elapsed_time(begin, end) << " seconds" << endl;
		}
		
		if (outfile != "none") {
			ofstream fout;
			fout.open(outfile.c_str());
			
			// terminate in error if output file could not be found
			if (not fout.is_open()) {
				cerr << "Error: could not open output file '" << outfile << "'" << endl;
				return 1;
			}
			
			SOL.store(fout);
			fout.close();
		}
	}
	else {
		if (verbose) {
			cout << "No solution found" << endl;
		}
	}
	
	delete bs;
	
	return 0;
}

