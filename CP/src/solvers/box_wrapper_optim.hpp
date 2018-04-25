#pragma once

/// C includes
#include <assert.h>

/// C++ includes
#include <iostream>
using namespace std;

/// Gecode
#include <gecode/minimodel.hh>
#include <gecode/search.hh>
#include <gecode/int.hh>
using namespace Gecode;

/// Custom includes
#include "box_wrapper_rotate.hpp"
#include "utils/wrapped_boxes.hpp"
#include "utils/definitions.hpp"
#include "utils/input_data.hpp"
#include "utils/logger.hpp"

class box_wrapper_optim : public box_wrapper_rotate {
	protected:
		gifts data;
		
	public:
		box_wrapper_optim(const gifts& gs, length max_L = -1);
		box_wrapper_optim(box_wrapper_optim& bw);
		virtual ~box_wrapper_optim();
		
		virtual Space *copy();
		
		// imposes that the length of roll used is as small as possible
		virtual void constrain(const Space& _b);
};

