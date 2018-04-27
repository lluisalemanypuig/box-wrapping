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

class LSButton : public QPushButton
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
		LSButton *save_button;

	public:
		LSButton(QWidget *p = 0);

		void set_instance_name(const string& iname);

		const string& get_instance_name() const;

	public slots:
		void load();
		void save();

};
