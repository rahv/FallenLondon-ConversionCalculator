#include "MainWindow.h"
#include <QtGui/QApplication>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	setlocale(LC_NUMERIC,"C");
	MainWindow* window = new MainWindow();
	window->setWindowTitle( window->windowTitle() );
	window->show();
	int returncode = app.exec();
	delete window;

	return returncode;
}
