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

enum solver {
	none, simple, rotate
};

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
				cerr << "Error: invalid value for option --encoder: '" << enc << "'" << endl;
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
	if (outfile == "none") {
		cerr << "Error: output file not specified." << endl;
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
	
	INPUT.fill_fields();
	
	// write clauses
	
	
	
	return 0;
}

