#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstdlib>
#include <map>
#include <vector>
using namespace std;

typedef pair<int, int> P;
typedef map<P, int>    M;
typedef pair<P, P>     PP;
typedef pair<P, int>   PI;
typedef vector<PP>     VPP;
typedef vector<PI>     VPI;
typedef vector<VPI>    VVPI;

const PI UNDEF(P(0, 0), 0);
const int MAX_COLORS = 256;
const int empty_cell = 0;	// colour of empty cell

ifstream in_ins, in_sol;
int  width, length;
M    order, order0, color;
VPP  sol;
VVPI board;
bool ok;

ostream& operator << (ostream& out, const P& p) {
	out << "(" << p.first << ", " << p.second << ")";
	return out;
}

bool within_bounds(P point) {
	return
		0 <= point.first  and point.first  < width and
		0 <= point.second and point.second < length;
}

bool check_solution() {
	order0 = order;
	board = VVPI(length, VPI(width, UNDEF));

	for (auto p : sol) {
		P tl = p.first;
		if (not within_bounds(tl)) {
			cout << "Error: position " << tl << " is out of bounds" << endl;
			return false;
		}
		P br = p.second;
		if (not within_bounds(br)) {
			cout << "Error: position " << br << " is out of bounds" << endl;
			return false;
		}
		
		int dx = br.first  - tl.first + 1;
		int dy = br.second - tl.second + 1;
		
		P rec (dx, dy);
		P recR(dy, dx);
		
		if (order.count(rec) == 0 and order.count(recR) == 0) {
			cout << "Error: no rectangle of dimensions "
				 << "(" << rec.first << "x" << rec.second << ")"
				 << " or of dimensions "
				 << "(" << recR.first << "x" << recR.second << ")"
				 << " defined by positions top-left corner "
				 << tl << " and bottom-right corner "
				 << br << " does not match any in input data" << endl;
			
			if (dx <= 0 or dy <= 0) {
				return false;
			}
		}
		
		P rectangle;
		if (order.count(rec) > 0) {
			--order[rec];
			rectangle = rec;
		}
		else if (order.count(recR) > 0) {
			--order[recR];
			rectangle = recR;
		}
		
		if (order[rectangle] < 0) {
			cout << "Error: too many rectangles of dimensions "
				 << "(" << rectangle.first << "x" << rectangle.second << ")"
				 << endl;
			
			return false;
		}
		
		for (int i = tl.second; i <= br.second; ++i) {
			for (int j = tl.first; j <= br.first; ++j) {
				if (board[i][j] != UNDEF) {
					cout << "Error:"
						 << " rectangle defined by top-left corner " << tl
						 << " and bottom-right corner " << br << " overlaps"
						 << " rectangle of dimensions " << board[i][j].first.first
						 << "x" << board[i][j].first.second << " at position "
						 << P(j, i) << endl;
					
					return false;
				}
				if (order[rectangle] < 0) {
					board[i][j] = PI(rectangle, 0);
				}
				else {
					board[i][j] = PI(rectangle, order[rectangle]);
				}
			}
		}
	}

	for (auto p : order) {
		if (p.second > 0) {
			cout << "Error: at least one rectangle of dimensions "
				 << p.first.first << "x" << p.first.second
				 << " is missing" << endl;
			
			return false;
		}
	}
	
	cout << "OK" << endl;
	return true;
}

string set_color(int c) {
	assert(0 <= c and c < MAX_COLORS);
	return string("\033[m\033[48;5;" + std::to_string(c) + "m");
}

string empty_cell_color() {
	return string("\033[m\033[48;5;" + std::to_string(empty_cell) + "m");
}

string unset_color() {
	return "\033[0m";
}

void display_solution() {
	if (order.size() > MAX_COLORS) {
		cout << "Cannot display solution: too many colors" << endl;
		exit(1);
	}
	
	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < width; ++j) {
			if (board[i][j].second > 'z' - 'a' + 1) {
				cout << "Cannot display solution: too many rectangles of the same dimension" << endl;
				exit(1);
			}
		}
	}
	
	for (int i = 0; i < length; ++i) {
		cout << "     ";
		for (int j = 0; j < width; ++j) {
			if (board[i][j] == UNDEF) {
				cout << empty_cell_color() << '.' << unset_color();
			}
			else {
				int c = color[board[i][j].first];
				cout << set_color(c) << char('a' + board[i][j].second)
					 << unset_color();
			}
		}
		cout << endl;
	}
	cout << endl << endl;
	
	cout << "Input data:" << endl << endl;
	for (auto p : color) {
		cout << set_color(p.second) << p.first.first << "x"
			 << p.first.second << unset_color()
			 << ":" << order0[p.first] << endl;
	}
}

void read_instance(char* file) {
	in_ins.open(file);
	in_ins >> width;
	int n, x, y;
	int c = empty_cell + 1;
	while (in_ins >> n >> x >> y) {
		order[P(x, y)] = n;
		color[P(x, y)] = c;
		++c;
	}
	in_ins.close();

	// cout << "W: " << width << endl;
	// for (auto i = order.begin(); i != order.end(); ++i) {
	//   cout << i->first << " => " << i->second << endl;
	// }
}


void read_solution(char* file) {
	in_sol.open(file);
	in_sol >> length;
	
	int xtl, ytl, xbr, ybr;
	while (in_sol >> xtl >> ytl >> xbr >> ybr) {
		sol.push_back(PP(P(xtl, ytl), P(xbr, ybr)));
	}
	
	in_sol.close();

	// cout << "L: " << length << endl;
	// for (auto p : sol)
	//   cout << p.first << " - " << p.second << endl;
}


int main(int argc, char** argv) {
	// Write help message.
	if (
		argc == 1 or
		(argc == 2 and (string(argv[1]) == "-h" or string(argv[1]) == "--help"))
	)
	{
		cout << "Makes a sanity check of a solution to the Box Wrapping Problem" << endl;
		cout << "Usage: " << argv[0] << " <file.in> <file.out>" << endl;
		exit(0);
	}
	assert(argc == 3);
	
	read_instance(argv[1]);
	read_solution(argv[2]);
	
	bool sane = check_solution();
	if (not sane) {
		cout << "Solution not sane!" << endl;
		return 1;
	}
	
	display_solution();
	return 0;
}

