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
#include <fstream>
#include <string>
using namespace std;

/// Qt includes
#include <QFileDialog>
#include <QPushButton>
#include <QLabel>

/// Custom includes
#include "box_organizer.hpp"
#include "savebutton.hpp"

class LoadButton : public QPushButton
{
	Q_OBJECT

	private:
		string instance_name;

	private:
		void read_input(ifstream& fin);
		void read_output(ifstream& fin);

	public:
		// references to other widgets... ugly solution?
		BoxOrganizer *box_org;
		QLabel *input_label;
		SaveButton *save_button;
		string defaultdir;
		string filter;

	public:
		LoadButton(QWidget *p = 0);

		const string& get_instance_name() const;

	public slots:
		void load();

};
