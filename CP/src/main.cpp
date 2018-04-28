/// C includes
#include <string.h>

/// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

/// Gecode
#include <gecode/search.hh>
using namespace Gecode;
using namespace Search;

/// Custom includes
#include "utils/wrapped_boxes.hpp"
#include "utils/parse_string.hpp"
#include "utils/input_data.hpp"
#include "utils/time.hpp"
#include "heuristics/heuristics.hpp"
#include "solvers/box_wrapper_simple.hpp"
#include "solvers/box_wrapper_rotate.hpp"
#include "solvers/box_wrapper_optim.hpp"

void print_usage() {
	cout << "BOX WRAPPING PROBLEM - Constraint Programming Solver" << endl;
	cout << endl;
	cout << "This software uses the library Gecode (tested under version 6.0.0)" << endl;
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
	cout << endl;
	cout << "Solvers:" << endl;
	cout << "    (1) --simple: initial version of a solver in Gecode for the BWP" << endl;
	cout << "    (2) --rotate: simple solver extended to allow rotations" << endl;
	cout << "    (3) --optim:  solver that tries to minimise roll length used." << endl;
	cout << "        The solutions found, however, may not be optimal." << endl;
	cout << "    (4) --heuris-incr: launches the solver that minimises roll length with different" << endl;
	cout << "        preprocessing strategies of the input data:" << endl;
	cout << "          -> area-increasingly sorted (boxes increasingly sorted by area)" << endl;
	cout << "          -> width-increasingly sorted (boxes increasingly sorted by width)" << endl;
	cout << "    (5) --heuris-decr: launches the solver that minimises roll length with different" << endl;
	cout << "        preprocessing strategies of the input data:" << endl;
	cout << "          -> area-decreasingly sorted (boxes decreasingly sorted by area)" << endl;
	cout << "          -> width-decreasingly sorted (boxes decreasingly sorted by width)" << endl;
	cout << "    (6) --heuris-rand: launches the solver that minimises roll length with different" << endl;
	cout << "        preprocessing strategies of the input data:" << endl;
	cout << "          -> randomly permuted input (boxes randomly permuted)" << endl;
	cout << "        When one execution is completed, the next will try to find solutions" << endl;
	cout << "        with shorter roll length than the best found in the previous." << endl;
	cout << "        This strategy will be executed as many times as specified in the parameter" << endl;
	cout << "        -N (see options for solvers (6), (7)" << endl;
	cout << "    (7) --heuris-mix: launches the solver that minimises roll length with different" << endl;
	cout << "        preprocessing strategies of the input data:" << endl;
	cout << "          -> area-increasingly sorted (boxes increasingly sorted by area)" << endl;
	cout << "          -> area-decreasingly sorted (boxes decreasingly sorted by area)" << endl;
	cout << "          -> width-increasingly sorted (boxes increasingly sorted by width)" << endl;
	cout << "          -> width-decreasingly sorted (boxes decreasingly sorted by width)" << endl;
	cout << "          -> randomly permuted input (boxes randomly permuted)" << endl;
	cout << "        When one strategy is completed, the next will try to find solutions" << endl;
	cout << "        with shorter roll length than the best found in the previous." << endl;
	cout << "        The first four strategies will be executed only once, while the last (random)" << endl;
	cout << "        will be executed as many times as specified in the parameter -N (see options" << endl;
	cout << "        for solvers (6), (7)" << endl;
	cout << endl;
	cout << "Options for solvers (1), (2), (3)" << endl;
	cout << "    * Modes of execution for solvers" << endl;
	cout << "        --enumerate: print the solutions found" << endl;
	cout << "            This is the default mode of execution" << endl;
	cout << "        --count-only: do not print the solutions" << endl;
	cout << endl;
	cout << "    * Terminating options" << endl;
	cout << "        --stop-at: stop finding solutions after n solutions were found" << endl;
	cout << "            Default: find all solutions" << endl;
	cout << "        --stop-when: stop finding solutions after s seconds have passed" << endl;
	cout << "            Default: never stop" << endl;
	cout << endl;
	cout << "    * Input data preprocessing" << endl;
	cout << "        --scramble: makes a random permutation of the input boxes" << endl;
	cout << "        --seed: seeds the random number generator to avoid repeating" << endl;
	cout << "            permutations between independent executions of the program" << endl;
	cout << endl;
	cout << "Option for solvers (4), (5)" << endl;
	cout << "    --stop-at: specify the amount of solutions per execution each solver" << endl;
	cout << "        is allowed to find. Once the amount is reached, the solver terminates." << endl;
	cout << "        Default: find all solutions" << endl;
	cout << "    --stop-when: specify the amount of time, in seconds, each" << endl;
	cout << "        execution can last up to" << endl;
	cout << "        Default: never stop" << endl;
	cout << "    [-v, --verbose]: display progress of heuristic." << endl;
	cout << "        Default: do not display progress" << endl;
	cout << endl;
	cout << "Option for solvers (6), (7)" << endl;
	cout << "    * the options for solvers (4), (5)" << endl;
	cout << "    [-Nr, --n-rand]: specify the number of times of the execution of the" << endl;
	cout << "        solver when using random permutations of the input data." << endl;
	cout << "        Default: 10" << endl;
	cout << endl;
	cout << "Options for all solvers" << endl;
	cout << "    --n-threads: use parallelisation to find solutions." << endl;
	cout << "        Default: 1 (sequential search)" << endl;
	cout << endl;
}

template<template<class> class S, class T>
void print_stats(const S<T>& G_SOLVER, size_t n_sols) {
	cerr << "Summary:" << endl;
	cerr << "    num solutions: " << n_sols << endl;
	cerr << "    Statistics:" << endl;
	Statistics stats = G_SOLVER.statistics();
	cerr << "        fail= " << stats.fail << endl;
	cerr << "        node= " << stats.node << endl;
	cerr << "        depth= " << stats.depth << endl;
	cerr << "        restart= " << stats.restart << endl;
	cerr << "        nogood= " << stats.nogood << endl;
}

template<template<class> class S, class T>
int solver_enum
(
	const Options& ops, const gifts& data, int stop_at,
	vector<wrapped_boxes>& sols, bool& error
)
{
	T *boxes = new T(data);
	S<T> G_SOLVER(boxes, ops);
	delete boxes;
	
	int n_sols = 0;
	T *sol;
	
	while (n_sols < stop_at and (sol = G_SOLVER.next()) and not error) {
		wrapped_boxes wb;
		sol->to_wrapped_boxes(data, wb);
		sols.push_back(wb);
		
		#if defined BOX_DEBUG
		string msg;
		bool sane = wb.is_sane(msg);
		if (not sane) {
			cout << "Solution not sane!" << endl;
			cout << wb << endl << endl;
			cout << msg << endl;
			error = true;
		}
		#endif
		
		delete sol;
		++n_sols;
	}
	
	cerr << endl;
	print_stats<S>(G_SOLVER, n_sols);
	
	return n_sols;
}

template<template<class> class S, class T>
int solver_count
(
	const Options& ops, const gifts& data,
	int stop_at, bool& error
)
{
	T *boxes = new T(data);
	S<T> G_SOLVER(boxes, ops);
	delete boxes;
	
	int n_sols = 0;
	T *sol;
	
	while (n_sols < stop_at and (sol = G_SOLVER.next()) and not error) {
		#if defined BOX_DEBUG
		wrapped_boxes wb;
		sol->to_wrapped_boxes(data, wb);
		
		string msg;
		bool sane = wb.is_sane(msg);
		if (not sane) {
			cout << wb << endl;
			cout << msg << endl;
			error = true;
		}
		#endif
		
		delete sol;
		++n_sols;
	}
	
	cerr << endl;
	print_stats<S,T>(G_SOLVER, n_sols);
	
	return n_sols;
}

template<template<class> class S, class T>
int print_or_count
(
	Options& ops, bool enumerate, const gifts& data,
	int stop_at,
	vector<wrapped_boxes>& solutions, bool& error
)
{
	if (enumerate) {
		return solver_enum<S,T>(ops, data, stop_at, solutions, error);
	}
	
	return solver_count<S,T>(ops, data, stop_at, error);
}

void choose_solver
(
	bool simple, bool rotate, bool optim, const gifts& data,
	bool enumerate, int n_threads, int stop_at, bool stop_time, double stop_when,
	vector<wrapped_boxes>& solutions, int& n_sols,
	bool& error
)
{
	Options ops = Options::def;
	ops.threads = n_threads;
	if (stop_time) {
		ops.stop = Stop::time( (unsigned long int)(1000.0*stop_when) );
	}
	
	if (simple) {
		n_sols =
			print_or_count<DFS, box_wrapper_simple>
			(ops, enumerate, data, stop_at, solutions, error);
	}
	else if (rotate) {
		n_sols =
			print_or_count<DFS, box_wrapper_rotate>
			(ops, enumerate, data, stop_at, solutions, error);
	}
	else if (optim) {
		n_sols =
			print_or_count<BAB, box_wrapper_optim>
			(ops, enumerate, data, stop_at, solutions, error);
	}
}

int main(int argc, char *argv[]) {
	string infile = "none";
	string outfile = "none";
	
	char fatal_error = 0;
	bool enumerate = true;
	
	bool simple = false;
	bool rotate = false;
	bool optim = false;
	bool heuris_inc = false;
	bool heuris_dec = false;
	bool heuris_rand = false;
	bool heuris_mix = false;
	
	int stop_at = numeric_limits<int>::max();
	bool stop_time = false;
	double stop_when = 99999.9;
	int n_threads = 1;
	
	bool scramble = false;
	bool seed = false;
	
	int n_exe_rand = 10;
	bool heuris_verbose = false;
	
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
		else if (strcmp(argv[i], "--heuris-inc") == 0) {
			heuris_inc = true;
		}
		else if (strcmp(argv[i], "--heuris-dec") == 0) {
			heuris_dec = true;
		}
		else if (strcmp(argv[i], "--heuris-rand") == 0) {
			heuris_rand = true;
		}
		else if (strcmp(argv[i], "--heuris-mix") == 0) {
			heuris_mix = true;
		}
		
		/// EXECUTION MODES
		else if (strcmp(argv[i], "--enumerate") == 0) {
			enumerate = true;
		}
		else if (strcmp(argv[i], "--count-only") == 0) {
			enumerate = false;
		}
		else if (strcmp(argv[i], "-Nr") == 0 or strcmp(argv[i], "--n-rand") == 0) {
			parse::parse_long("parsing # times random", argv[i + 1], &n_exe_rand, &fatal_error);
			++i;
		}
		else if (strcmp(argv[i], "-v") == 0 or strcmp(argv[i], "--verbose") == 0) {
			heuris_verbose = true;
		}
		
		/// TERMINATING OPTIONS
		else if (strcmp(argv[i], "--stop-at") == 0) {
			parse::parse_long("parsing max number solutions", argv[i + 1], &stop_at, &fatal_error);
			++i;
		}
		else if (strcmp(argv[i], "--stop-when") == 0) {
			parse::parse_double("parsing max time", argv[i + 1], &stop_when, &fatal_error);
			stop_time = true;
			++i;
		}
		
		/// EFFICIENCY OPTIONS
		else if (strcmp(argv[i], "--n-threads") == 0) {
			parse::parse_long("parsing number of threads", argv[i + 1], &n_threads, &fatal_error);
			++i;
		}
		
		/// INPUT DATA PREPROCESSING
		else if (strcmp(argv[i], "--scramble") == 0) {
			scramble = true;
		}
		else if (strcmp(argv[i], "--seed") == 0) {
			seed = true;
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
	
	int solvers_selected = simple + rotate + optim + heuris_inc + heuris_dec + heuris_rand + heuris_mix;
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
	
	// variables for exploration
	vector<wrapped_boxes> solutions;
	int n_sols;
	bool error = false;
	
	double begin = timing::now();
	if (simple or rotate or optim) {
		if (scramble) {
			INPUT.scramble(seed);
		}
		
		choose_solver
		(
			simple, rotate, optim, INPUT,
			enumerate, n_threads, stop_at, stop_time, stop_when,
			solutions, n_sols,
			error
		);
	}
	else if (heuris_inc or heuris_dec or heuris_rand or heuris_mix) {
		solutions = vector<wrapped_boxes>(1);
		
		heuristics::heuristic_chooser
		(
			heuris_inc, heuris_dec, heuris_rand, heuris_mix,
			n_threads, n_exe_rand,
			stop_at, stop_time, stop_when,
			heuris_verbose,
			INPUT,
			solutions[0], n_sols,
			error
		);
	}
	double end = timing::now();
	
	cout << "In " << timing::elapsed_time(begin, end, n_threads) << " seconds (average)" << endl;
	
	// some solution is not sane, terminate program in error
	if (error) {
		return 1;
	}
	
	if (enumerate) {
		cout << "--------------------" << endl;
		for (int i = 0; i < n_sols; ++i) {
			cout << i + 1 << ")" << endl;
			cout << solutions[i] << endl;
			cout << "--------------------" << endl;
		}
	}
	
	if (outfile != "none") {
		ofstream fout;
		fout.open(outfile.c_str());
		
		// terminate in error if output file could not be found
		if (not fout.is_open()) {
			cerr << "Error: could not open output file '" << outfile << "'" << endl;
			return 1;
		}
		
		// find the best solution only if there is actually one
		if (solutions.size() > 0) {
			wrapped_boxes BEST;
			for (const wrapped_boxes& sol : solutions) {
				if (sol < BEST) {
					BEST = sol;
				}
			}
			BEST.store(fout);
			cout << BEST << endl;
		}
		
		fout.close();
	}
	
	return 0;
}

