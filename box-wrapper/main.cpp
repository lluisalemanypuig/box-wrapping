#include "box_wrapper_mw.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BoxWrapperMainWindow w;
	w.show();

	return a.exec();
}
