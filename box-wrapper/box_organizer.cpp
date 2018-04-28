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
	int w = width();
	int h = height();
	float xcoord, ycoord, nxcoord, nycoord;
	int x, y;

	// vertical lines
	w = width();
	for (int x = 0; x < w; x += sqw) {
		xcoord = (2.0/w)*x - 1;

		glColor3f(1,1,1);
		glBegin(GL_LINES);
			glVertex2f(xcoord, 1);
			glVertex2f(xcoord, -1);
		glEnd();
	}

	// horizontal lines
	h = height();
	for (y = 0; y < h; y += sqh) {
		ycoord = (2.0/h)*y - 1;

		glColor3f(1,1,1);
		glBegin(GL_LINES);
			glVertex2f(1, ycoord);
			glVertex2f(-1, ycoord);
		glEnd();
	}

	// diagonal lines
	xcoord = (2.0/w)*maxW - 1;
	for (y = 0; y < h; y += sqh) {
		ycoord = (2.0/h)*y - 1;
		int Y = y + width() - maxW;
		nycoord = (2.0/h)*Y - 1;

		glColor3f(1,1,1);
		glBegin(GL_LINES);
			glVertex2f(xcoord, ycoord);
			glVertex2f(1, nycoord);
		glEnd();
	}

	for (x = maxW + sqw; x < w; x += sqw) {
		xcoord = (2.0/w)*x - 1;

		int Y = width() - x;
		nycoord = (2.0/h)*Y - 1;

		glColor3f(1,1,1);
		glBegin(GL_LINES);
			glVertex2f(xcoord, -1);
			glVertex2f(1, nycoord);
		glEnd();
	}
}

/// PUBLIC

BoxOrganizer::BoxOrganizer(QWidget *w) : QOpenGLWidget(w) {
	round = 0;
	selected_box = -1;
	maxW = 20*sqw;

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

	boxes_tl[selected_box].first = x/sqw;
	boxes_tl[selected_box].second = y/sqh;
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

	dims.push_back(coord(w,h));
	boxes_tl.push_back(tl);
}

void BoxOrganizer::set_boxes(const vector<coord>& tls, const vector<coord>& ds) {
	boxes_tl = tls;
	dims = ds;
}

void BoxOrganizer::clear_boxes() {
	round = 0;
	dims.clear();
	boxes_tl.clear();
}

void BoxOrganizer::get_box_corners(int& L, vector<coord>& tls, vector<coord>& brs) const {
	L = 0;
	tls = boxes_tl;
	for (size_t i = 0; i < tls.size(); ++i) {
		brs.push_back( coord( tls[i].first + dims[i].first, tls[i].second + dims[i].second ) );
		L = max(L, tls[i].second + dims[i].second);
	}
}
