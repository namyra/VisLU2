/*! \file mainwindow.h
	\brief MainWindow header file.

	Contains the declarations for the MainWindow class, which is the GUI for the application window.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "glwidget.h"
#include "tfview.h"

//! Widget for the main window of the application.
/*!
	Provides the widget for the main window, including all the GUI and the GLWidget that takes care of the actual visualisation.
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	//! Default constructor.
	/*!
		Creates a new MainWindow and sets up the GUI for it.
		\sa createSlider(), ~MainWindow()
	*/
    MainWindow();

	//! Default destructor.
	/*!
		\sa MainWindow()
	*/
    ~MainWindow();

private:
	//! The widget group for the sidebar containing the UI elements.
    QGroupBox *sideBar;

	//! The widget displaying the actual render of the volume data.
    GLWidget *glWidget;

	QCheckBox *checkArrowPlot;
	QCheckBox *checkStreamlines;

	//! The Qt scene used to display the transfer function.
	QGraphicsScene *transferScene;

	//! The widget displaying the transfer function.
	TFView *transferView;

	//! The button to clear the TF view.
	QPushButton *clearButton;

	//! The File menu.
	QMenu *fileMenu;

	//! The Open file action.
	QAction *openAct;

private slots:

	//! Slot for the Open file action.
	/*!
		Opens a file dialog and if a new file is chosen, tells the display widget to use the new file for its volume data.
	*/
	void loadDataset();
};

#endif // MAINWINDOW_H
