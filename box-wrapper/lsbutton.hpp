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
		BoxOrganizer *box_org;
		QLabel *input_label;

	public:
		LSButton(QWidget *p = 0);

		void set_ref_organizer(BoxOrganizer *r);
		void set_input_label(QLabel *l);

	public slots:
		void load();
		void save();

};
