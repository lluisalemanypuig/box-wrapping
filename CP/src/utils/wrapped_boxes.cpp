#include "wrapped_boxes.hpp"

/// PRIVATE

length wrapped_boxes::box_length(size_t b) const {
	return br_corner[b].first - tl_corner[b].first + 1;
}

width wrapped_boxes::box_width(size_t b) const {
	return br_corner[b].second - tl_corner[b].second + 1;
}

void wrapped_boxes::display_box_cells(ostream& os) const {
	// compute columns length (for output purposes)
	vector<size_t> col_width(W, 0);
	for (width j = 0; j < W; ++j) {
		for (length i = 0; i < L; ++i) {
			size_t val_length = std::to_string(roll[i][j]).size();
			col_width[j] = max(col_width[j], val_length);
		}
		size_t header_length = std::to_string(j).size();
		col_width[j] = max(col_width[j], header_length);
	}
	
	/// actually output info
	
	// output header
	size_t header_length = 0;
	size_t W_length = std::to_string(W).size();
	size_t L_length = std::to_string(L).size();
	
	os << setw(L_length) << " " << " | W" << endl;
	
	os << setw(L_length) << "L" << " | ";
	header_length += W_length + 3;
	for (width j = 0; j < W; ++j) {
		os << setw(col_width[j]) << j << " ";
		header_length += col_width[j] + 1;
	}
	os << endl;
	os << string(header_length, '-') << endl;
	
	// output matrix
	for (length i = 0; i < L; ++i) {
		os << setw(L_length) << i << " | ";
		for (width j = 0; j < W; ++j) {
			if (roll[i][j] == 0) {
				os << setw(col_width[j]) << "." << " ";
			}
			else {
				os << setw(col_width[j]) << roll[i][j] << " ";
			}
		}
		os << endl;
	}
}

void wrapped_boxes::display_box_corners(ostream& os) const {
	length roll_length_used = 0;
	
	os << "Boxes' corners:"
	   << setw(std::to_string(N).size()) << " "
	   << "     (w,l)" << endl;
	
	for (size_t b = 0; b < N; ++b) {
		roll_length_used = max(roll_length_used, br_corner[b].first);
		
		os << "    Box " << setw(std::to_string(N).size()) << b + 1 << " is at " << endl
		   << "        top left     (" << tl_corner[b].second << "," << tl_corner[b].first << ")" << endl
		   << "        bottom right (" << br_corner[b].second << "," << br_corner[b].first << ")" << endl;
		
		if (b < N - 1) {
			os << endl;
		}
	}
	++roll_length_used;
	
	os << endl;
	os << "    Roll length used: " << roll_length_used;
}

/// PUBLIC

wrapped_boxes::wrapped_boxes() {
	init(0,0,0);
}
wrapped_boxes::wrapped_boxes(size_t _N, length _L, width _W) {
	init(_N, _L, _W);
}
wrapped_boxes::~wrapped_boxes() { }

void wrapped_boxes::init(size_t _N, length _L, width _W) {
	N = _N;
	L = _L;
	W = _W;
	roll_length = numeric_limits<size_t>::max();
	
	roll = vector<vector<size_t> >(L, vector<size_t>(W, 0));
	tl_corner = vector<corner>(N);
	br_corner = vector<corner>(N);
}

/// OPERATORS

wrapped_boxes& wrapped_boxes::operator= (const wrapped_boxes& wb) {
	W = wb.W;
	L = wb.L;
	N = wb.N;
	roll_length = wb.roll_length;
	
	roll = wb.roll;
	tl_corner = wb.tl_corner;
	br_corner = wb.br_corner;
	return *this;
}

bool wrapped_boxes::operator< (const wrapped_boxes& wb) const {
	return roll_length < wb.roll_length;
}

void wrapped_boxes::set_tl_box_corner(size_t k, const corner& c) {
	tl_corner[k] = c;
}

void wrapped_boxes::set_br_box_corner(size_t k, const corner& c) {
	br_corner[k] = c;
	if (roll_length == numeric_limits<size_t>::max()) {
		roll_length = c.first + 1;
	}
	else {
		roll_length = max(roll_length, c.first + 1);
	}
}

void wrapped_boxes::set_box_cell(size_t k, const cell& c) {
	roll[c.first][c.second] = k;
}

bool wrapped_boxes::is_sane(string& error_msg) const {
	
	/// 1. All boxes must have a corner
	/// 2. A top-left corner of a box must be placed
	/// within bounds for that box
	/// 3. If a box has its corner at (i,j) then
	/// the spanning cells must have that box
	for (size_t b = 0; b < N; ++b) {
		length tl_pl = tl_corner[b].first;
		width tl_pw = tl_corner[b].second;
		
		/// (1)
		if (tl_pl == L + 1 or tl_pw == W + 1) {
			error_msg  = "Error: top-left corner for box " + std::to_string(b + 1)
					   + " not found.";
			return false;
		}
		
		length br_pl = br_corner[b].first;
		width br_pw = br_corner[b].second;
		
		/// (2)
		if (br_pl >= L or br_pw >= W) {
			error_msg  = "Error: bottom-right corner for box " + std::to_string(b + 1)
					   + " at (l,w)=("
					   + std::to_string(br_pl)
					   + ","
					   + std::to_string(br_pw)
					   + ") is out of bounds\n";
			return false;
		}
		
		/// (3)
		for (length ii = tl_pl; ii <= br_pl; ++ii) {
			for (width jj = tl_pw; jj <= br_pw; ++jj) {
				
				size_t box_idx = roll[ii][jj];
				
				// some box at (ii,jj) -> it should be 'b'
				if (box_idx != 0 and box_idx != b + 1) {
					
					error_msg  = "Error: incorrect box at (l,w)=("
							   + std::to_string(ii)
							   + ","
							   + std::to_string(jj)
							   + ").\n"
							   + "    Box found:    " + std::to_string(box_idx) + "\n"
							   + "    Supposed box: " + std::to_string(b + 1) + "\n";
					return false;
				}
			}
		}
	}
	
	/// 4. All boxes occupy as many cells as they have area
	vector<size_t> area_box(N, 0);
	for (length i = 0; i < L; ++i) {
		for (width j = 0; j < W; ++j) {
			size_t box_idx = roll[i][j];
			if (box_idx == 0) continue; // no box
			
			area_box[box_idx - 1] += 1;
		}
	}
	
	for (size_t b = 0; b < N; ++b) {
		length b_length = br_corner[b].first - tl_corner[b].first + 1;
		width b_width = br_corner[b].second - tl_corner[b].second + 1;
		area A = b_length*b_width;
		
		if (area_box[b] != A) {
			
			error_msg = "Error: box " + std::to_string(b + 1) + " occupies";
			if (area_box[b] < A) {
				error_msg += " less ";
			}
			else {
				error_msg += " more ";
			}
			error_msg += "area than it should.\n";
			error_msg += "    Occupied: " + std::to_string(area_box[b]) + "\n";
			error_msg += "    Box area: " + std::to_string(A) + "\n";
			
			return false;
		}
	}
	
	// solution is sane
	return true;
}

length wrapped_boxes::get_roll_length() const {
	return roll_length;
}

void wrapped_boxes::store(ofstream& fout) const {
	// a line containing the roll's length used
	fout << roll_length << endl;
	
	// the coordinates of the top-left and bottom-right
	// corners of all boxes
	for (size_t b = 0; b < N; ++b) {
		fout << tl_corner[b].second << " " << tl_corner[b].first;
		fout << "\t\t";
		fout << br_corner[b].second << " " << br_corner[b].first;
		fout << endl;
	}
}

