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
	protected:
		void add_objective();
		
	public:
		box_wrapper_optim();
		box_wrapper_optim(const box_wrapper_optim& bw);
		~box_wrapper_optim();
		
};

