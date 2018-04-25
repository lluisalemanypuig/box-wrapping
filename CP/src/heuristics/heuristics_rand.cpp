#include "heuristics.hpp"

namespace heuristics {
	
	bool heuristic_rand
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
		
		/// RANDOM ORDER
		
		// seed rng only once
		srand(time(NULL));
		bool sol = false;
		
		for (size_t i = 0; i < rand_times; ++i) {
			data_copy.scramble();
			if (verbose) {
				cout << "----------------------------" << endl;
				cout << "Random preprocessing " << i + 1 << ")" << endl;
				cout << data_copy << endl;
			}
			bool solr = heur_private::call_solver(ops, data_copy, stop_at, stop_time, stop_when, verbose, BEST, error);
			sol = sol or solr;
		}
		
		return sol;
	}
	
}

