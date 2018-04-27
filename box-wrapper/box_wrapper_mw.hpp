#pragma once

/// Qt includes
#include <QMainWindow>

namespace Ui {
class BoxWrapperMainWindow;
}


class BoxWrapperMainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		explicit BoxWrapperMainWindow(QWidget *parent = 0);
		~BoxWrapperMainWindow();

	private:
		Ui::BoxWrapperMainWindow *ui;
};

