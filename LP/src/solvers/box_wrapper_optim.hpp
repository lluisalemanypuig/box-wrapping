#pragma once

// C includes
#include <assert.h>

// C++ includes
#include <iostream>
using namespace std;

// CPLEX
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

// Custom includes
#include "box_wrapper_rotate.hpp"
#include "utils/wrapped_boxes.hpp"
#include "utils/definitions.hpp"
#include "utils/input_data.hpp"
#include "utils/logger.hpp"

class box_wrapper_optim : public box_wrapper_rotate {
	private:
		void objective1(const gifts& gs);
		void objective2(const gifts& gs);
	
	protected:
		/* DOES NOT WORK */
		// box_lengths[b] = the length chosen for this box
		IloNumVarArray box_lengths;
		IloNumVar Le(size_t b) const { return box_lengths[b]; }
		IloNumVar Le(size_t b) { return box_lengths[b]; }
		
		/* SEEMS TO WORK - BUT IT IS SLOW */
		IloNumVarArray box_coordinates;
		IloNumVar max_length;
		IloNumVar S(size_t b) const { return box_coordinates[b]; }
		IloNumVar S(size_t b) { return box_coordinates[b]; }
		
		void add_objective(const gifts& gs);
		
	public:
		box_wrapper_optim();
		box_wrapper_optim(const box_wrapper_optim& bw);
		~box_wrapper_optim();
		
};
