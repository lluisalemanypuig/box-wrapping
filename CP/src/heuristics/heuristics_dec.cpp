#include "heuristics.hpp"

namespace heuristics {
	
	bool heuristic_dec
	(
		Options& ops,
		size_t stop_at,
		bool stop_time, double stop_when,
		bool verbose,
		const gifts& input_data,
		wrapped_boxes& BEST, bool& error
	)
	{
		gifts data_copy = input_data;
		
		/// DECREASING ORDER
		
		// by area
		data_copy.sort_area_dec();
		if (verbose) {
			cout << "Decreasing area preprocessing" << endl;
			cout << data_copy << endl;
		}
		bool area = heur_private::call_solver(ops, data_copy, stop_at, stop_time, stop_when, verbose, BEST, error);
		
		// by width
		data_copy.sort_width_dec();
		if (verbose) {
			cout << "Decreasing width preprocessing" << endl;
			cout << data_copy << endl;
		}
		bool width = heur_private::call_solver(ops, data_copy, stop_at, stop_time, stop_when, verbose, BEST, error);
		
		return area or width;
	}
	
}

