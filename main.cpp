/*! \file main.cpp
	\brief Main application source file.

	Contains the source code for the application, which simply creates a MainWindow and shows it.
*/

#include <QtGui/QApplication>
#include "mainwindow.h"

//! Main function.
/*!
	Creates a MainWindow and shows it.
	\param argc The number of command line arguments.
	\param argv The command line arguments.
	\return 0 in a successful program exit.
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
