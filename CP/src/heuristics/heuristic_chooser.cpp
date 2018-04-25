#include "heuristics.hpp"

namespace heuristics {
	
	void heuristic_chooser
	(
		bool heuris_inc, bool heuris_dec, bool heuris_rand, bool heuris_mix,
		size_t n_threads, size_t rand_times,
		size_t stop_at, bool stop_time, double stop_when,
		bool verbose,
		const gifts& input_data,
		wrapped_boxes& solution, size_t& n_sols,
		bool& error
	)
	{
		Options ops = Options::def;
		ops.threads = n_threads;
		
		if (heuris_inc) {
			n_sols = heuristic_inc(ops, stop_at, stop_time, stop_when, verbose, input_data, solution, error);
		}
		else if (heuris_dec) {
			n_sols = heuristic_dec(ops, stop_at, stop_time, stop_when, verbose, input_data, solution, error);
		}
		else if (heuris_rand) {
			n_sols = heuristic_rand(ops, rand_times, stop_at, stop_time, stop_when, verbose, input_data, solution, error);
		}
		else if (heuris_mix) {
			n_sols = heuristic_mix(ops, rand_times, stop_at, stop_time, stop_when, verbose, input_data, solution, error);
		}
	}
	
}
