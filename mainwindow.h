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

	//! The widget group for all arrow plot options.
	QGroupBox *arrowGroup;

	//! The checkbox that enables/disables the arrow plot.
	QCheckBox *checkArrowPlot;

	QLabel *labelNumArrows;
	QSpinBox *sbNumArrows;

	QCheckBox *checkArrowScale;

	//! The widget group for all streamline options.
	QGroupBox *linesGroup;

	//! The checkbox that enables/disables the streamlines.
	QCheckBox *checkStreamlines;

	//! The button group for the buttons to switch between Euler and Runge-Kutta algorithms.
    QButtonGroup *rkButtons;

	//! The button that switches to Euler algorithm.
    QRadioButton *eulerButton;

	//! The button that switches to Runge-Kutta algorithm.
	QRadioButton *rkButton;

	QLabel *labelNumLines;
	QSpinBox *sbNumLines;

	QLabel *labelNumSteps;
	QSpinBox *sbNumSteps;

	QLabel *labelStepSize;
	QDoubleSpinBox *sbStepSize;

	//! The widget group for all pong options.
	QGroupBox *pongGroup;

	//! The button to reset the pong game.
	QPushButton *resetButton;

	//! The button to pause the pong game.
	QPushButton *pauseButton;

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
