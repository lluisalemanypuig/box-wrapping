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

#pragma once

/// C++ includes
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

/// Qt includes
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QWidget>
#include <QLabel>

typedef pair<int,int> coord;

class BoxOrganizer : public QOpenGLWidget
{
	Q_OBJECT

	private:
		const int sqw = 20;
		const int sqh = 20;

	private:
		struct color {
			float r,g,b;

			color() {}
			color(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
		};

		struct vec2f {
			float x,y;

			vec2f() {}
			vec2f(float _x, float _y) : x(_x), y(_y) {}
			vec2f(const vec2f& v) : x(v.x), y(v.y) {}
		};

		// arrays for our grid
		vector<vec2f> vertical, horizontal, diagonal;

		// boxes' coordinates, and colour
		vector<coord> boxes_tl;
		vector<coord> dims;
		map<size_t, color> box_col;

		// used to place boxes correctly when first added
		int round;

		// actual width of the roll. Merely used to draw the grid
		int maxW;
		// length of the roll used. Used to display a bar
		int maxL;

		// index of the select box (-1 if no box is selected)
		int selected_box;

	private:
		int find_box(int x, int y) const;
		void draw_box(int i) const;
		void draw_grid() const;
		void get_max_length();

		void make_vert_hor_lines();
		void make_diagonal_lines();

	public:
		QLabel *length_label;

	public:
		BoxOrganizer(QWidget *w = 0);

		void init();

		void mousePressEvent(QMouseEvent *e);
		void mouseMoveEvent(QMouseEvent *e);
		void paintGL();

		void set_max_width(int W);
		void set_max_length(int L);
		void add_box(int w, int h);
		void set_boxes(const vector<coord>& tls, const vector<coord>& dims);
		void clear_boxes();

		void get_box_corners(int& L, vector<coord>& tls, vector<coord>& brs) const;
};
