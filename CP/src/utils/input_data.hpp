#pragma once

/// C includes
#include <stdlib.h>
#include <time.h>

/// C++ includes
#include <algorithm>
#include <fstream>
#include <utility>
#include <vector>
using namespace std;

/// Custom includes
#include "definitions.hpp"

/// <<
/// BOX class

class box {
	private:
	public:
		width w;
		length l;
	
	public:
		box();
		box(width w, length l);
		box(const box& b);
		~box();
		
		/// OPERATORS
		
		box& operator= (const box& b);
		
		inline friend
		istream& operator>> (istream& is, box& b) {
			is >> b.w >> b.l;
			return is;
		}
		
		inline friend
		ostream& operator<< (ostream& os, const box& b) {
			os << b.w << " " << b.l;
			return os;
		}
		
		/// GETTERS
		
		bool is_square() const;
};

/// BOX class
/// >>

/// <<
/// GIFTS class

class gifts {
	private:
	public:
		width W;
		
		length sum_lengths;		// for the 'simple' solver
		length sum_max_sides;	// for the 'rotate' and 'optim' solvers
		
		vector<box> all_boxes;
		size_t total_boxes;
	
	public:
		gifts();
		gifts(const gifts& gs);
		~gifts();
		
		/// OPERATORS
		
		gifts& operator= (const gifts& gs);
		
		inline friend
		istream& operator>> (istream& is, gifts& gs) {
			is >> gs.W;
			size_t n;
			while (is >> n) {
				box B;
				is >> B;
				gs.all_boxes.insert(gs.all_boxes.end(), n, B);
			}
			gs.fill_fields();
			return is;
		}
		
		inline friend
		ostream& operator<< (ostream& os, const gifts& gs) {
			os << "    W=" << gs.W << endl;
			os << "    sum lengths=   " << gs.sum_lengths << endl;
			os << "    sum max sides= " << gs.sum_max_sides << endl;
			size_t idx = 1;
			for (const box& b : gs.all_boxes) {
				os << "    box " << idx << ": " << b << endl;
				++idx;
			}
			return os;
		}
		
		/// MODIFIERS
		
		// makes 'summary' of input data
		void fill_fields();
		
		// makes a permutation of the input boxes
		void scramble(bool seed = false);
		
		// sort increasingly/decreasingly by area
		void sort_area_inc();
		void sort_area_dec();
		
		void sort_width_inc();
		void sort_width_dec();
		
		/// GETTERS
		
		// return the maximum length appropriate for the 'simple' solver
		length get_max_length_s() const;
		
		// return the maximum length appropriate for the 'rotate' and
		// 'optim' solvers
		length get_max_length_ro() const;
		
};

/// GIFTS class
/// >>
