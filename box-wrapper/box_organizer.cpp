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

#include "box_organizer.hpp"

/// PRIVATE

int BoxOrganizer::find_box(int x, int y) const {
	int p = -1;
	int i = 0;
	bool found = false;
	while (i < boxes_tl.size() and not found) {
		int xtl = boxes_tl[i].first;
		int ytl = boxes_tl[i].second;

		if (sqw*xtl <= x and x <= sqw*(xtl + dims[i].first)) {
			if (sqh*ytl <= y and y <= sqh*(ytl + dims[i].second)) {
				found = true;
				p = i;
			}
		}

		++i;
	}

	return p;
}

void BoxOrganizer::get_max_length() {
	maxL = 0;
	for (size_t i = 0; i < boxes_tl.size(); ++i) {
		maxL = max(maxL, boxes_tl[i].second + dims[i].second);
	}
}

void BoxOrganizer::make_vert_hor_lines() {
	vertical.clear();
	horizontal.clear();

	// make horizontal and vertical lines for our grid
	// vertical lines
	int w = width();
	for (int x = 0; x < w; x += sqw) {
		float xcoord = (2.0/w)*x - 1;
		vertical.push_back(vec2f(xcoord,  1.0));
		vertical.push_back(vec2f(xcoord, -1.0));
	}

	// horizontal lines
	int h = height();
	for (int y = 0; y < h; y += sqh) {
		float ycoord = (2.0/h)*y - 1;
		horizontal.push_back(vec2f( 1.0, ycoord));
		horizontal.push_back(vec2f(-1.0, ycoord));
	}
}

void BoxOrganizer::make_diagonal_lines() {
	diagonal.clear();

	const int w = width();
	const int h = height();

	float xcoord, ycoord;
	float nxcoord, nycoord;

	nxcoord = 1.0;

	// diagonal lines
	for (int y = 0; y < h; y += sqh) {
		xcoord = (2.0/w)*maxW - 1;
		ycoord = (2.0/h)*y - 1;
		nycoord = (2.0/h)*(y + w - maxW) - 1;

		diagonal.push_back(vec2f(xcoord, ycoord));
		diagonal.push_back(vec2f(nxcoord, nycoord));
	}

	ycoord = -1.0;

	for (int x = maxW + sqw; x < w; x += sqw) {
		xcoord = (2.0/w)*x - 1.0;
		nycoord = (2.0/h)*(w - x) - 1.0;

		diagonal.push_back(vec2f(xcoord, ycoord));
		diagonal.push_back(vec2f(nxcoord, nycoord));
	}
}

void BoxOrganizer::draw_box(int i) const {
	float w = width();
	float h = height();

	float xtl = sqw*boxes_tl[i].first;
	float ytl = sqh*boxes_tl[i].second;
	float xbr = sqw*(boxes_tl[i].first + dims[i].first);
	float ybr = sqh*(boxes_tl[i].second + dims[i].second);

	float xtlnorm = (2.0/w)*xtl - 1;
	float ytlnorm = (2.0/h)*ytl - 1;
	float xbrnorm = (2.0/w)*xbr - 1;
	float ybrnorm = (2.0/h)*ybr - 1;

	const color& c = box_col.find(i)->second;
	glColor3f(c.r,c.g,c.b);
	glRectf(xtlnorm,ytlnorm, xbrnorm,ybrnorm);
}

void BoxOrganizer::draw_grid() const {
	glColor3f(1,1,1);

	// vertical lines
	for (int i = 0; i < vertical.size(); i += 2) {
		glBegin(GL_LINES);
			glVertex2f(vertical[i].x, vertical[i].y);
			glVertex2f(vertical[i + 1].x, vertical[i + 1].y);
		glEnd();
	}

	// horizontal lines
	for (int i = 0; i < horizontal.size(); i += 2) {
		glBegin(GL_LINES);
			glVertex2f(horizontal[i].x, horizontal[i].y);
			glVertex2f(horizontal[i + 1].x, horizontal[i + 1].y);
		glEnd();
	}

	// diagonal lines
	for (int i = 0; i < diagonal.size(); i += 2) {
		glBegin(GL_LINES);
			glVertex2f(diagonal[i].x, diagonal[i].y);
			glVertex2f(diagonal[i + 1].x, diagonal[i + 1].y);
		glEnd();
	}

	// max length bar
	const int h = height();

	float xtlnorm = -1;
	float ytlnorm = (2.0/h)*maxL*sqh - 1;
	float xbrnorm = 1.0;
	float ybrnorm = (2.0/h)*(maxL*sqh + sqh) - 1;
	float incr = (xbrnorm - xtlnorm)/20.0;
	glColor3f(1.0,0.0,0.0);
	glLineWidth(10.0);
	for (float xcoord = xtlnorm; xcoord <= xbrnorm; xcoord += incr) {
		float nxcoord = xcoord + 0.05;
		glBegin(GL_LINES);
			glVertex2f(xcoord, ytlnorm);
			glVertex2f(nxcoord, ybrnorm);
		glEnd();
	}
	glLineWidth(1.0);
}

/// PUBLIC

BoxOrganizer::BoxOrganizer(QWidget *parent) : QOpenGLWidget(parent) {
	length_label = nullptr;
	round = 0;
	selected_box = -1;
	maxW = 20*sqw; // all roll available
	maxL = 0; // no roll used

	box_col[ 0] = color(1,0,0);
	box_col[ 1] = color(0,1,0);
	box_col[ 2] = color(0,0,1);
	box_col[ 3] = color(1,0,1);
	box_col[ 4] = color(1,0.78,0);
	box_col[ 5] = color(0,1,1);
	box_col[ 6] = color(0.9,0.4,1);
	box_col[ 7] = color(0.5,0.5,0);
	box_col[ 8] = color(1,0,0.5);
	box_col[ 9] = color(1,0.5,0);
	box_col[10] = color(0.25,0,0.75);
	box_col[11] = color(1,0,0);
	box_col[12] = color(0.3,0,0.7);
	box_col[13] = color(0,0,0);
	box_col[14] = color(0.2,0,0.4);
	box_col[15] = color(0.3,0,0);
	box_col[16] = color(0,0,0);
	box_col[17] = color(0.5,0,0.7);
	box_col[18] = color(0.5,0,0.1);
	box_col[19] = color(0.1,0,0);
	box_col[20] = color(0,0.8,0);
	box_col[21] = color(0.9,0,0);
	box_col[22] = color(0,0.48,0);
	box_col[23] = color(0,0.7,0.36);
	box_col[24] = color(0,0.31,0);
	box_col[25] = color(0,0.7,0.8);
}

void BoxOrganizer::init() {
	make_vert_hor_lines();
	make_diagonal_lines();
}

void BoxOrganizer::mousePressEvent(QMouseEvent *e) {
	int x = e->pos().x();
	int y = height() - e->pos().y();

	selected_box = find_box(x,y);

	if (selected_box == -1) {
		// nothing to do
		return;
	}

	if (e->button() == Qt::LeftButton) {
		// nothing to do *here*
	}
	else if (e->button() == Qt::RightButton) {
		swap(dims[selected_box].first, dims[selected_box].second);
		get_max_length();

		length_label->setText(QString::fromStdString(std::to_string(maxL)));

		repaint();
	}
}

void BoxOrganizer::mouseMoveEvent(QMouseEvent *e) {
	int x = e->pos().x();
	int y = height() - e->pos().y();

	if (x < 0 or y < 0) {
		// invalid mouse pos
		return;
	}

	if (selected_box == -1) {
		// nothing to do because no box is selected
		return;
	}

	int sel_x = boxes_tl[selected_box].first;
	int sel_y = boxes_tl[selected_box].second;
	if (x/sqw == sel_x and y/sqh == sel_y) {
		return;
	}

	boxes_tl[selected_box].first = x/sqw;
	boxes_tl[selected_box].second = y/sqh;
	get_max_length();

	length_label->setText(QString::fromStdString(std::to_string(maxL)));

	repaint();
}

void BoxOrganizer::paintGL() {
	// clear the screen
	glColor3f(0,0,0);
	glRectf(-1,-1, 1,1);

	draw_grid();

	for (size_t i = 0; i < boxes_tl.size(); ++i) {
		draw_box(i);
	}
}

void BoxOrganizer::set_max_width(int W) {
	maxW = W*sqw;
	make_diagonal_lines();
}

void BoxOrganizer::set_max_length(int L) {
	maxL = L;
	length_label->setText(QString::fromStdString(std::to_string(maxL)));
}

void BoxOrganizer::add_box(int w, int h) {
	coord tl(0,0);
	if (boxes_tl.size() > 0) {
		tl = boxes_tl.back();
		tl.first = 5*round;
		tl.second += dims.back().second + 1;
	}

	if (tl.second + h >= 30) {
		++round;
		tl.first = 5*round;
		tl.second = 0;
	}

	maxL = max(maxL, tl.second + h);

	dims.push_back(coord(w,h));
	boxes_tl.push_back(tl);
	length_label->setText(QString::fromStdString(std::to_string(maxL)));
}

void BoxOrganizer::set_boxes(const vector<coord>& tls, const vector<coord>& ds) {
	boxes_tl = tls;
	dims = ds;
}

void BoxOrganizer::clear_boxes() {
	maxL = 0;
	maxW = 0;
	round = 0;
	dims.clear();
	boxes_tl.clear();
}

void BoxOrganizer::get_box_corners(int& L, vector<coord>& tls, vector<coord>& brs) const {
	L = maxL;
	tls = boxes_tl;
	for (size_t i = 0; i < tls.size(); ++i) {
		brs.push_back( coord( tls[i].first + dims[i].first, tls[i].second + dims[i].second ) );
	}
}
