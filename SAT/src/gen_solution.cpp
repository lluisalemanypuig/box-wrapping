// C includes
#include <string.h>

// C++ includes
#include <iostream>
using namespace std;

// Custom includes
#include "utils/parse_string.hpp"
#include "utils/wrapped_boxes.hpp"
#include "utils/definitions.hpp"
#include "utils/input_data.hpp"
#include "encoder/clause.hpp"
using namespace utils;
using namespace encoder;

void print_usage() {
	cout << "BOX WRAPPING PROBLEM - Satisfiability (CNF) Solution builder" << endl;
	cout << endl;
	cout << "This software takes as input the processed output of a SAT solver" << endl;
	cout << "that consists of a list of Boolean variables, either set to true" << endl;
	cout << "(positive integer) or to false (negative integer). Each variable" << endl;
	cout << "is part of an instance of the Box Wrapping Problem (BWP) which must" << endl;
	cout << "the input file of this builder. All these variables are read from" << endl;
	cout << "the __standard input__" << endl;
	cout << endl;
	cout << "Note: this program does not follow the standard for return values" << endl;
	cout << "(0 success, 1 error, 2 misuse of shell builtins). Instead, the value 0" << endl;
	cout << "is reinterpreted as general error, and any other positive value as the" << endl;
	cout << "roll's length of the solution." << endl;
	cout << endl;
	cout << "Parameters:" << endl;
	cout << "    [-h, --help]: show the usage" << endl;
	cout << "    --boxes: specify file with BWP instance" << endl;
	cout << "    --variables: specify file with the Boolean assignment" << endl;
	cout << "    [-o, --output]: specify where to store the clauses in CNF" << endl;
	cout << "    [-v, --verbose]: output the solution read on standard output" << endl;
	cout << "    --max-L: give an upper bound on the roll's length" << endl;
	cout << "Solvers:" << endl;
	cout << "    --solver: choose solver. Possible values are" << endl;
	cout << "        simple: do not consider rotations for boxes" << endl;
	cout << "        rotate: allow rotations for boxes" << endl;
	cout << endl;
}

int main(int argc, char *argv[]) {
	string bwp_file = "none";
	string vars_file = "none";
	string outfile = "none";
	solver S = solver::none;
	bool verbose = false;
	length max_L = inf_t<length>();
	
	// process arguments
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0) {
			print_usage();
			return 0;
		}
		else if (strcmp(argv[i], "--boxes") == 0) {
			bwp_file = string(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--variables") == 0) {
			vars_file = string(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "-o") == 0 or strcmp(argv[i], "--output") == 0) {
			outfile = string(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--solver") == 0) {
			string sol = string(argv[i + 1]);
			if (sol == "simple") {
				S = solver::simple;
			}
			else if (sol == "rotate") {
				S = solver::rotate;
			}
			else {
				cerr << "Error: invalid value for option --solver: '" << sol << "'" << endl;
				return 1;
			}
			++i;
		}
		else if (strcmp(argv[i], "-v") == 0 or strcmp(argv[i], "--verbose") == 0) {
			verbose = true;
		}
		else if (strcmp(argv[i], "--max-L") == 0) {
			char fatal = 0;
			parse::parse_long("parsing length upper bound", argv[i + 1], &max_L, &fatal);
			++i;
			
			if (fatal) {
				return 1;
			}
		}
		else {
			cerr << "Error: option '" << string(argv[i]) << "' not recognised" << endl;
			return 0;
		}
	}
	
	// <error control>
	if (bwp_file == "none") {
		cerr << "Error: input file not specified." << endl;
		cerr << "    Use [-h, --help] to see the usage for details." << endl;
		return 0;
	}
	if (outfile == "none") {
		cerr << "Error: output file not specified." << endl;
		cerr << "    Use [-h, --help] to see the usage for details." << endl;
		return 0;
	}
	if (vars_file == "none") {
		cerr << "Error: file with the variables not specified." << endl;
		cerr << "    Use [-h, --help] to see the usage for details." << endl;
		return 0;
	}
	if (S == solver::none) {
		cerr << "Error: solver not specified." << endl;
		cerr << "    Use [-h, --help] to see the usage for details." << endl;
		return 0;
	}
	
	// <read input data>
	ifstream fin;
	fin.open(bwp_file.c_str());
	if (not fin.is_open()) {
		cerr << "Error: could not open BWP instance file '" << bwp_file << "'" << endl;
		return 0;
	}
	gifts INPUT;
	fin >> INPUT;
	fin.close();
	
	fin.open(vars_file.c_str());
	if (not fin.is_open()) {
		cerr << "Error: could not open variables file '" << vars_file << "'" << endl;
		return 0;
	}
	
	// number of variables
	int nXvars;	// for corners
	int nCvars;	// for cells
	int nRvars;	// for rotations

	size_t N = INPUT.total_boxes;	// number of boxes
	width W = INPUT.W;	// roll's width
	length L;
	
	if (S == solver::simple) {
		L = INPUT.get_max_length_s();
	}
	else if (S == solver::rotate) {
		L = INPUT.get_max_length_ro();
	}
	
	L = min(L, max_L);
	
	if (S == solver::simple) {
		nXvars = N*W*L;
		nCvars = N*W*L;
		nRvars = 0;
	}
	else if (S == solver::rotate) {
		nXvars = N*W*L;
		nCvars = N*W*L;
		nRvars = N;
	}
	
	wrapped_boxes solution(N,L,W);
	
	if (S == solver::simple) {
		// we only need to read the top-left corner variables
		for (int c = 0; c < nXvars; ++c) {
			literal litX;
			fin >> litX;
			
			if (litX > 0) {
				var k = get_var(litX) - 1;
			
				int b = k/(W*L);
				int kp = k - b*W*L;
				int i = kp/W;
				int j = kp%W;
				
				length b_length = INPUT.all_boxes[b].l;
				length b_width = INPUT.all_boxes[b].w;
				
				solution.set_tl_box_corner(b, corner(i,j));
				solution.set_br_box_corner(b,
					corner(
						i + b_length - 1,
						j + b_width - 1
					)
				);
				
				for (length ii = i; ii < i + b_length; ++ii) {
					for (width jj = j; jj < j + b_width; ++jj) {
						solution.set_box_cell(b + 1, cell(ii,jj));
					}
				}
			}
		}
	}
	else if (S == solver::rotate) {
		// read all variables to have access to the rotation variables
		vector<literal> all_lits(nXvars + nCvars + nRvars);
		size_t i = 0;
		literal lit;
		while (i < all_lits.size() and fin >> lit and lit != 0) {
			all_lits[i] = lit;
			++i;
		}
		
		assert(i >= all_lits.size());
		
		// for each top-left corner variable
		for (int c = 0; c < nXvars; ++c) {
			literal litX = all_lits[c];
			
			if (litX > 0) {
				var k = get_var(litX) - 1;
				
				int b = k/(W*L);
				int kp = k - b*W*L;
				int i = kp/W;
				int j = kp%W;
				
				// if the box is rotated
				bool is_rotated = (all_lits[nXvars + nCvars + b] > 0);
				length b_length = INPUT.all_boxes[b].l;
				length b_width = INPUT.all_boxes[b].w;
				
				if (is_rotated) {
					swap(b_length, b_width);
				}
				
				solution.set_tl_box_corner(b, corner(i,j));
				solution.set_br_box_corner(b,
					corner(
						i + b_length - 1,
						j + b_width - 1
					)
				);
				
				for (length ii = i; ii < i + b_length; ++ii) {
					for (width jj = j; jj < j + b_width; ++jj) {
						solution.set_box_cell(b + 1, cell(ii,jj));
					}
				}
			}
		}
	}
	
	if (verbose) {
		cout << solution << endl;
		string msg;
		if (not solution.is_sane(msg)) {
			cerr << endl;
			cerr << "Solution is not sane!" << endl;
			cerr << "    " << msg << endl;
			return 0;
		}
	}
	
	ofstream fout;
	fout.open(outfile.c_str());
	if (not fout.is_open()) {
		cerr << "Error: could not open output file '" << outfile << "'" << endl;
		return 0;
	}
	solution.store(fout);
	fout.close();
	
	return solution.get_roll_length();
}

