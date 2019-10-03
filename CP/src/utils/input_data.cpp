/*********************************************************************
 * Box Wrapping solver
 * Copyright (C) 2018  Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

#include "input_data.hpp"

namespace utils {

/* BOX class */
box::box() : w(0), l(0) { }
box::box(width _w, length _l) : w(_w), l(_l) { }
box::box(const box& b) {
	*this = b;
}
box::~box() { }

box& box::operator= (const box& b) {
	w = b.w;
	l = b.l;
	return *this;
}

bool box::is_square() const {
	return w == l;
}

/* GIFTS class */

gifts::gifts() : W(0) { }
gifts::gifts(const gifts& gs) {
	*this = gs;
}
gifts::~gifts() { }

gifts& gifts::operator= (const gifts& gs) {
	W = gs.W;
	sum_lengths = gs.sum_lengths;
	sum_max_sides = gs.sum_max_sides;
	all_boxes = gs.all_boxes;
	total_boxes = gs.total_boxes;
	return *this;
}

void gifts::fill_fields() {
	total_boxes = all_boxes.size();
	sum_lengths = 0;
	sum_max_sides = 0;
	for (const box& BOX : all_boxes) {
		sum_lengths += BOX.l;
		sum_max_sides += max(BOX.l, BOX.w);
	}
}

void gifts::scramble(bool seed) {
	if (seed) {
		srand(time(NULL));
	}
	
	random_shuffle(all_boxes.begin(), all_boxes.end());
}

void gifts::sort_area_inc() {
	auto comp = [](const box& b1, const box& b2) -> bool {
		return b1.w*b1.l < b2.w*b2.l;
	};
	sort(all_boxes.begin(), all_boxes.end(), comp);
}

void gifts::sort_area_dec() {
	auto comp = [](const box& b1, const box& b2) -> bool {
		return b1.w*b1.l < b2.w*b2.l;
	};
	// use reverse iterators
	sort(all_boxes.rbegin(), all_boxes.rend(), comp);
}

void gifts::sort_width_inc() {
	auto comp = [](const box& b1, const box& b2) -> bool {
		return b1.w < b2.w;
	};
	sort(all_boxes.begin(), all_boxes.end(), comp);
}

void gifts::sort_width_dec() {
	auto comp = [](const box& b1, const box& b2) -> bool {
		return b1.w < b2.w;
	};
	// use reverse iterators
	sort(all_boxes.rbegin(), all_boxes.rend(), comp);
}

length gifts::get_max_length_s() const {
	return sum_lengths;
}

length gifts::get_max_length_ro() const {
	return sum_max_sides;
}

} // -- namespace utils

