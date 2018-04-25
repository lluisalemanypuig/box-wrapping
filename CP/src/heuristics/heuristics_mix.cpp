#include "heuristics.hpp"

namespace heuristics {
	
	bool heuristic_mix
	(
		Options& ops,
		uint32 rand_times, uint32 stop_at,
		bool stop_time, double stop_when,
		bool verbose,
		const gifts& input_data,
		wrapped_boxes& BEST, bool& error
	)
	{
		gifts data_copy = input_data;
		
		/// INCREASING ORDER
		// by area
		data_copy.sort_area_inc();
		if (verbose) {
			cout << "Increasing area preprocessing" << endl;
			cout << data_copy << endl;
		}
		bool sol_area_inc = heur_private::call_solver(ops, data_copy, 3, stop_time, stop_when, verbose, BEST, error);
		// by width
		data_copy.sort_width_inc();
		if (verbose) {
			cout << "Increasing width preprocessing" << endl;
			cout << data_copy << endl;
		}
		bool sol_width_inc = heur_private::call_solver(ops, data_copy, 3, stop_time, stop_when, verbose, BEST, error);
		
		/// DECREASING ORDER
		// by area
		data_copy.sort_area_dec();
		if (verbose) {
			cout << "Decreasing area preprocessing" << endl;
			cout << data_copy << endl;
		}
		bool sol_area_dec = heur_private::call_solver(ops, data_copy, 3, stop_time, stop_when, verbose, BEST, error);
		// by width
		data_copy.sort_width_dec();
		if (verbose) {
			cout << "Decreasing width preprocessing" << endl;
			cout << data_copy << endl;
		}
		bool sol_width_dec = heur_private::call_solver(ops, data_copy, 3, stop_time, stop_when, verbose, BEST, error);
		
		/// RANDOM
		srand(time(NULL)); // seed rng only once
		bool sol_rand = false;
		
		for (size_t i = 0; i < rand_times; ++i) {
			data_copy.scramble();
			if (verbose) {
				cout << "----------------------------" << endl;
				cout << "Random preprocessing " << i + 1 << ")" << endl;
				cout << data_copy << endl;
			}
			bool solr = heur_private::call_solver(ops, data_copy, stop_at, stop_time, stop_when, verbose, BEST, error);
			sol_rand = sol_rand or solr;
		}
		
		return sol_area_inc or sol_width_inc or sol_area_dec or sol_width_dec or sol_rand;
	}
	
}

