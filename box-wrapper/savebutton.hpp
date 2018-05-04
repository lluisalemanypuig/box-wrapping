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

class SaveButton : public QPushButton
{
	Q_OBJECT

	private:
		string instance_name;

	public:
		// references to other widgets... ugly solution?
		BoxOrganizer *box_org;

	public:
		SaveButton(QWidget *p = 0);

		void set_instance_name(const string& iname);
		const string& get_instance_name() const;

	public slots:
		void save();

};
