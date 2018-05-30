#pragma once

// C++ includes
#include <utility>
#include <limits>
using namespace std;

namespace utils {

typedef int length;
typedef int width;
typedef int area;

typedef pair<int, int> corner;
typedef pair<int, int> cell;

template<typename T> T inf_t() { return numeric_limits<T>::max(); }

} // -- namespace utils

