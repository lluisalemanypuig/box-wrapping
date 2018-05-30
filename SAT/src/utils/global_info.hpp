#pragma once

namespace utils {

class global_info {
	private:
		int n_model_vars;
		int n_ext_vars;
		int n_clauses;
		
		global_info();
		~global_info();
		
	public:
		static global_info& get_info() {
			static global_info gI;
			return gI;
		}
		
		/* MODIFIERS */
		
		void set_model_vars(int n);
		void add_vars(int n);
		void add_clauses(int n);
		
		/* GETTERS */
		
		int get_new_var();
		int get_model_vars() const;
		int get_total_vars() const;
		int get_total_clauses() const;
};
	
} // -- namespace utils

