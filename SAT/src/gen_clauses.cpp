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
#include "encoder/clause_encoder.hpp"
#include "encoder/quadratic_encoder.hpp"
#include "encoder/logarithmic_encoder.hpp"
#include "encoder/heule_encoder.hpp"
using namespace encoder;
using namespace utils;

// -----------------------------
// HELPER FUNCTIONS
void open_stream(const string& output_file, ofstream& of, streambuf **buf) {
	if (output_file != "none") {
		of.open(output_file, ios_base::app);
		*buf = of.rdbuf();

		ifstream temp_fin;
		temp_fin.open(output_file.c_str());
		temp_fin.close();
	}
	else {
		*buf = cout.rdbuf();
	}
}
// -- HELPER FUNCTIONS
// -----------------------------

// number of variables
int nXvars;	// for corners
int nCvars;	// for cells
int nRvars;	// for rotations

size_t N;	// number of boxes
width W;	// roll's width
length L;	// maximum length

enum solver {
	none, simple, rotate
};

int X(int b, int i, int j) {
	return b*W*L + i*W + j + 1;
}

int C(int b, int i, int j) {
	return nXvars + b*W*L + i*W + j + 1;
}

int R(int b) {
	return nXvars + nCvars + b + 1;
}

clause_encoder& make_encoder(const encoder_type& et) {
	switch (et) {
		case encoder_type::logarithmic:
			return logarithmic_encoder::get_encoder();
		case encoder_type::heule:
			return heule_encoder::get_encoder();
		default:
			return quadratic_encoder::get_encoder();
	}
}

void simple_solver(const gifts& data, ostream& out, clause_encoder& CE) {
	
	// (1). one corner per box
	for (size_t b = 0; b < N; ++b) {
		clause cl;
		for (length i = 0; i < L; ++i) {
			for (width j = 0; j < W; ++j) {
				cl += X(b,i,j);
			}
		}
		CE.exactly_one(cl, out);
	}
	
	// (2). at most one box per cell
	for (length i = 0; i < L; ++i) {
		for (width j = 0; j < W; ++j) {
			
			clause cl;
			for (size_t b = 0; b < N; ++b) {
				cl += C(b,i,j);
			}
			CE.amo(cl, out);
			
		}
	}
	
	// (3). if box is placed somewhere then the 'spanning' cells are occupied
	for (size_t b = 0; b < N; ++b) {
		length b_length = data.all_boxes[b].l;
		width b_width = data.all_boxes[b].w;
		
		// for each cell to be occupied by box 'b' (without rotation)
		for (length i = 0; i < L; ++i) {
			for (width j = 0; j < W; ++j) {
				
				// ignore the cases where part of the box is out of bounds
				if (i + b_length - 1 >= L) continue;
				if (j + b_width - 1 >= W) continue;
				
				for (length ii = i; ii <= i + b_length - 1; ++ii) {
					for (width jj = j; jj <= j + b_width - 1; ++jj) {
						
						out << -X(b,i,j) << " " << C(b,i,j) << " 0" << endl;
						
					}
				}
				
			}
		}
	}
	
	// (5). Cannot place the top-left corner of a box at cell (i,j) if
	// it will end up out of bounds
	for (int b = 0; b < N; ++b) {
		length b_length = data.all_boxes[b].l;
		width b_width = data.all_boxes[b].w;
		
		// for each cell to be occupied by box 'b' (without rotation)
		for (length i = 0; i < L; ++i) {
			for (width j = 0; j < W; ++j) {
				
				// if the box is WITHIN limits then ignore
				if (i + b_length - 1 <= L - 1 and j + b_width - 1 <= W - 1) continue;
				
				out << -X(b,i,j) << " 0" << endl;
			}
		}
	}
	
}

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
	cout << "    [-o, --output]: specify where to store the clauses in CNF" << endl;
	cout << "Solvers:" << endl;
	cout << "    --solver: choose solver. Possible values are" << endl;
	cout << "        simple: do not consider rotations for boxes" << endl;
	cout << "        rotate: allow rotations for boxes" << endl;
	cout << "Encoder:" << endl;
	cout << "    --amo-encoder: choose the encoder for \"at most one\" constraints. Values:" << endl;
	cout << "        quadratic (default)" << endl;
	cout << "        logarithmic" << endl;
	cout << "        heule" << endl;
	cout << endl;
}

int main(int argc, char *argv[]) {
	string infile = "none";
	string outfile = "none";
	solver S = solver::none;
	encoder_type E = encoder_type::quadratic;
	
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
		else if (strcmp(argv[i], "--amo-encoder") == 0) {
			string enc = string(argv[i + 1]);
			if (enc == "quadratic") {
				E = encoder_type::quadratic;
			}
			else if (enc == "logarithmic") {
				E = encoder_type::logarithmic;
			}
			else if (enc == "heule") {
				E = encoder_type::heule;
			}
			else {
				cerr << "Error: invalid value for option --amo-encoder: '" << enc << "'" << endl;
				return 1;
			}
			++i;
		}
		else {
			cerr << "Error: option '" << string(argv[i]) << "' not recognised" << endl;
			return 1;
		}
	}
	
	// <error control>
	if (infile == "none") {
		cerr << "Error: input file not specified." << endl;
		cerr << "    Use [-h, --help] to see the usage for details." << endl;
		return 1;
	}
	if (S == solver::none) {
		cerr << "Error: solver not specified." << endl;
		cerr << "    Use [-h, --help] to see the usage for details." << endl;
		return 1;
	}
	
	// <read input data>
	ifstream fin;
	fin.open(infile.c_str());
	if (not fin.is_open()) {
		cerr << "Error: could not open input file '" << infile << "'" << endl;
		return 1;
	}
	// <open output>
	ofstream fout;
	fout.open(outfile.c_str());
	if (not fout.is_open()) {
		cerr << "Error: could not open output file '" << outfile << "'" << endl;
		return 1;
	}
	
	gifts INPUT;
	fin >> INPUT;
	fin.close();
	
	// initialise variables
	INPUT.fill_fields();
	
	N = INPUT.total_boxes;
	L = INPUT.get_max_length_s();
	W = INPUT.W;
	
	nXvars = N*W*L;
	nCvars = N*W*L;
	nRvars = N;
	
	global_info& gI = global_info::get_info();
	gI.set_model_vars(nXvars + nCvars + nRvars);
	
	clause_encoder& ce = make_encoder(E);
	
	ofstream of;
	streambuf *buf;
	open_stream(outfile, of, &buf);
	ostream out(buf);
	
	// write clauses
	
	
	
	// last line
	out << "p cnf " << gI.get_total_vars() << " " << gI.get_total_clauses() << endl;
	return 0;
}

