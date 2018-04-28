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

	public:
		BoxOrganizer(QWidget *w = 0);

		void mousePressEvent(QMouseEvent *e);
		void mouseMoveEvent(QMouseEvent *e);
		void paintGL();

		void set_max_width(int W);
		void add_box(int w, int h);
		void set_boxes(const vector<coord>& tls, const vector<coord>& dims);
		void clear_boxes();

		void get_box_corners(int& L, vector<coord>& tls, vector<coord>& brs) const;
};
