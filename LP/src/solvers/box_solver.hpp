#pragma once

// Custom includes
#include "utils/wrapped_boxes.hpp"
#include "utils/definitions.hpp"
#include "utils/input_data.hpp"
#include "utils/logger.hpp"

class box_solver {
	private:
	protected:
		bool is_solved;
		
	public:
		box_solver();
		~box_solver();
		
		virtual void init(const gifts& gs, length max_L = -1) = 0;
		virtual void solve() = 0;
		virtual void solution(const gifts& boxes_to_wrap, wrapped_boxes& wb) const = 0;
		
		bool found_solution() const;
};

