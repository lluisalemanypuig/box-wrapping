#include "heuristics.hpp"

namespace heuristics {
namespace heur_private {
	
	bool call_solver
	(
		Options& ops, const gifts& data,
		int stop_at,
		bool stop_time, double stop_when,
		bool verbose,
		wrapped_boxes& BEST, bool& error
	)
	{
		if (stop_time) {
			ops.stop = Stop::time( (unsigned long int)(1000.0*stop_when) );
		}
		
		int n_sols;
		box_wrapper_optim *boxes = nullptr;
		box_wrapper_optim *sol;
		
		boxes = new box_wrapper_optim(data, BEST.get_roll_length());
		BAB<box_wrapper_optim> INC_SOLV(boxes, ops);
		delete boxes;
		
		n_sols = 0;
		while (n_sols < stop_at and (sol = INC_SOLV.next()) and not error) {
			wrapped_boxes wb;
			sol->to_wrapped_boxes(data, wb);
			
			#ifdef BOX_DEBUG
			string msg;
			bool sane = wb.is_sane(msg);
			if (not sane) {
				cout << "Solution not sane!" << endl;
				cout << wb << endl << endl;
				cout << msg << endl;
				error = true;
			}
			#endif
			
			if (wb < BEST) {
				BEST = wb;
			}
			
			if (verbose) {
				cout << n_sols + 1 << ")" << endl;
				cout << wb << endl;
				cout << endl;
			}
			
			++n_sols;
		}
		
		return n_sols > 0;
	}
	
}
}

