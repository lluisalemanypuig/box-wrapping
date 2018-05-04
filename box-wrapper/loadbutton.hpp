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
