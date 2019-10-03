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

// C++ includes
#include <iostream>
#include <fstream>

class null_stream {
	public:
		void open(const char *, const std::ios_base::openmode& ) { }
		
		// this is the type of std::cout
		typedef std::basic_ostream<char, std::char_traits<char> > cout_type;

		// this is the function signature of std::endl
		typedef cout_type& (*standard_endl)(cout_type&);

		// define an operator<< to take in std::endl
		null_stream& operator<< (const standard_endl&)
		{ return *this; }
		
		// operator<< for any printable type
		template<class t_printable>
		null_stream& operator<< (const t_printable&)
		{ return *this; }
};

class cout_stream {
	public:
		void open(const char *, const std::ios_base::openmode& ) { }

		// this is the type of std::cout
		typedef std::basic_ostream<char, std::char_traits<char> > cout_type;

		// this is the function signature of std::endl
		typedef cout_type& (*standard_endl)(cout_type&);

		// define an operator<< to take in std::endl
		cout_stream& operator<< (const standard_endl&) {
			std::cout << endl;
			return *this;
		}

		// operator<< for any printable type
		template<class t_printable>
		cout_stream& operator<< (const t_printable& t) {
			std::cout << t;
			return *this;
		}
};

class cerr_stream {
	public:
		void open(const char *, const std::ios_base::openmode& ) { }

		// this is the type of std::cout
		typedef std::basic_ostream<char, std::char_traits<char> > cerr_type;

		// this is the function signature of std::endl
		typedef cerr_type& (*standard_endl)(cerr_type&);

		// define an operator<< to take in std::endl
		cerr_stream& operator<< (const standard_endl&) {
			std::cerr << endl;
			return *this;
		}

		// operator<< for any printable type
		template<class t_printable>
		cerr_stream& operator<< (const t_printable& t) {
			std::cerr << t;
			return *this;
		}
};

template<class out_stream = std::ofstream>
class logger {
	private:
		out_stream fout;
		bool opened;
		
		logger() {
			opened = false;
		}
		
	public:
		out_stream& log() {
			return fout;
		}
		
		static logger& get_logger(const std::string& o = "log.txt") {
			static logger<out_stream> L;
			
			if (not L.opened) {
				L.fout.open(o.c_str(), std::ios_base::app);
				L.opened = true;
			}
			
			return L;
		}
		
		logger (const logger& L)			= delete;
		void operator= (const logger& L)	= delete;
};

