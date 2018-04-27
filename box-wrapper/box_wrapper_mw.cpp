#include "box_wrapper_mw.hpp"
#include "ui_boxwrappermainwindow.h"

BoxWrapperMainWindow::BoxWrapperMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::BoxWrapperMainWindow)
{
	ui->setupUi(this);

	ui->loadButton->box_org = ui->organizer;
	ui->loadButton->input_label = ui->inputNameLabel;
	ui->loadButton->save_button = ui->saveButton;

	ui->saveButton->box_org = ui->organizer;
}

BoxWrapperMainWindow::~BoxWrapperMainWindow()
{
	delete ui;
}
