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

#include "box_wrapper_mw.hpp"
#include "ui_boxwrappermainwindow.h"

BoxWrapperMainWindow::BoxWrapperMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::BoxWrapperMainWindow)
{
	ui->setupUi(this);

	ui->loadInputButton->box_org = ui->organizer;
	ui->loadInputButton->input_label = ui->inputNameLabel;
	ui->loadInputButton->save_button = ui->saveButton;
	ui->loadInputButton->defaultdir = "../inputs/material";
	ui->loadInputButton->filter = "BWP instance (*.in)";

	ui->loadOutputButton->box_org = ui->organizer;
	ui->loadOutputButton->input_label = ui->inputNameLabel;
	ui->loadOutputButton->save_button = ui->saveButton;
	ui->loadOutputButton->defaultdir = "../outputs/hand-made";
	ui->loadOutputButton->filter = "BWP output (*.out)";

	ui->saveButton->box_org = ui->organizer;

	ui->organizer->length_label = ui->lengthLabel;
	ui->organizer->init();
}

BoxWrapperMainWindow::~BoxWrapperMainWindow()
{
	delete ui;
}
