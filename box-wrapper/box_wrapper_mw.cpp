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
