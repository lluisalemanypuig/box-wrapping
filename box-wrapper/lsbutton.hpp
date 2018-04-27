#pragma once

/// C++ includes
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/// Qt includes
#include <QFileDialog>
#include <QPushButton>

/// Custom includes
#include "box_organizer.hpp"

class LSButton : public QPushButton
{
	Q_OBJECT

	private:
		BoxOrganizer *box_org;

	public:
		LSButton(QWidget *p = 0);

		void set_ref_organizer(BoxOrganizer *r);

	public slots:
		void load();
		void save();

};
