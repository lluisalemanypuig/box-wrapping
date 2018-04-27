#include "box_wrapper_mw.hpp"
#include "ui_boxwrappermainwindow.h"

BoxWrapperMainWindow::BoxWrapperMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::BoxWrapperMainWindow)
{
	ui->setupUi(this);

	ui->loadButton->set_ref_organizer(ui->organizer);
	ui->loadButton->set_input_label(ui->inputNameLabel);
	ui->saveButton->set_ref_organizer(ui->organizer);
}

BoxWrapperMainWindow::~BoxWrapperMainWindow()
{
	delete ui;
}
