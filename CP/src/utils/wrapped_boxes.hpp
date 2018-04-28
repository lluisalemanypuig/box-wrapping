#pragma once

/// C++ includes
#include <iostream>
#include <utility>
#include <fstream>
#include <iomanip>
#include <vector>
#include <limits>
using namespace std;

/// Custom includes
#include "definitions.hpp"

class wrapped_boxes {
	private:
		width W;
		length L, roll_length;
		int N;
		
		// roll[i][j] = k <-> box k occupies cell k
		vector<vector<int> > roll;
		
		// tl_corner[k] = (i,j) <-> top-left corner of box k is (i,j)
		vector<corner> tl_corner;
		
		// br_corner[k] = (i,j) <-> bottom-right corner of box k is (i,j)
		vector<corner> br_corner;
	
	private:
		length box_length(int box_idx) const;
		width box_width(int box_idx) const;
	
		void display_box_cells(ostream& os) const;
		void display_box_corners(ostream& os) const;
	
	public:
		wrapped_boxes();
		wrapped_boxes(int N, length L, width W);
		~wrapped_boxes();
		
		void init(int N, length L, width W);
		
		/// OPERATORS
		
		wrapped_boxes& operator= (const wrapped_boxes& wb);
		bool operator< (const wrapped_boxes& wb) const;
		
		inline friend
		ostream& operator<< (ostream& os, const wrapped_boxes& wb) {
			wb.display_box_cells(os);
			os << endl;
			wb.display_box_corners(os);
			return os;
		}
		
		// c: (i, j), where
		//    0 <= i <= L - 1, with L the total length of the roll
		//    0 <= j <= W - 1, with W the (fixed) width of the roll
		void set_tl_box_corner(int k, const corner& c);
		void set_br_box_corner(int k, const corner& c);
		void set_box_cell(int k, const cell& c);
		void set_roll_length(length L);
		
		/// GETTERS
		
		// makes a sanity check
		bool is_sane(string& error_msg) const;
		
		// returns the roll's length used
		length get_roll_length() const;
		
		/// I/O
		
		// stores in a file following the format
		// spceified in the project's statement
		void store(ofstream& fout) const;
};

