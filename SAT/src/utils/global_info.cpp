#include "global_info.hpp"

namespace utils {

/* PRIVATE */

global_info::global_info() {
	n_model_vars = 0;
	n_ext_vars = 0;
	n_clauses = 0;
}
global_info::~global_info() { }

/* PUBLIC */

void global_info::set_model_vars(int n) { n_model_vars = n; }
void global_info::add_vars(int n)		{ n_ext_vars += n; }
void global_info::add_clauses(int n)	{ n_clauses += n; }

int global_info::get_new_var() {
	++n_ext_vars;
	return n_model_vars + n_ext_vars;
}

int global_info::get_model_vars() const		{ return n_model_vars; }
int global_info::get_total_vars() const		{ return n_model_vars + n_ext_vars; }
int global_info::get_total_clauses() const	{ return n_clauses; }

} // -- namespace utils

