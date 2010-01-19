/*! \file mainwindow.cpp
	\brief MainWindow source file.

	Contains the source code for the MainWindow class, which is the GUI for the application window.
*/

#include <QtGui>

#include "mainwindow.h"

MainWindow::MainWindow()
{
	QWidget *widget = new QWidget;
	setCentralWidget(widget);

    glWidget = new GLWidget(50, widget);

	checkArrowPlot = new QCheckBox("Enabled", widget);
	connect(checkArrowPlot, SIGNAL(toggled(bool)), glWidget, SLOT(toggleArrowPlot(bool)));
	checkArrowPlot->setChecked(true);

	checkArrowScale = new QCheckBox("Scale Arrows", widget);
	connect(checkArrowScale, SIGNAL(toggled(bool)), glWidget, SLOT(toggleArrowScale(bool)));
	checkArrowScale->setChecked(true);

	sbNumArrows = new QSpinBox();
	sbNumArrows->setMinimum(10);
	sbNumArrows->setMaximum(50);
	labelNumArrows = new QLabel("Number of Arrows");
    connect(sbNumArrows, SIGNAL(valueChanged(int)), glWidget, SLOT(setNumArrows(int)));
	sbNumArrows->setValue(20);

	arrowGroup = new QGroupBox("Arrow Plot");
    QGridLayout *arrowGroupLayout = new QGridLayout;
    arrowGroupLayout->addWidget(checkArrowPlot, 1, 1);
    arrowGroupLayout->addWidget(checkArrowScale, 2, 1);
    arrowGroupLayout->addWidget(labelNumArrows, 3, 1);
    arrowGroupLayout->addWidget(sbNumArrows, 3, 2);
    arrowGroup->setLayout(arrowGroupLayout);

	checkStreamlines = new QCheckBox("Enabled", widget);
	connect(checkStreamlines, SIGNAL(toggled(bool)), glWidget, SLOT(toggleStreamlines(bool)));
	checkStreamlines->setChecked(true);

    rkButtons = new QButtonGroup;
    eulerButton = new QRadioButton("Euler");
    rkButton = new QRadioButton("Runge-Kutta");
    rkButtons->addButton(eulerButton, 0);
    rkButtons->addButton(rkButton, 1);
	connect(rkButton, SIGNAL(toggled(bool)), glWidget, SLOT(setRK(bool)));
    rkButton->setChecked(true);

	sbNumLines = new QSpinBox();
	sbNumLines->setMinimum(10);
	sbNumLines->setMaximum(50);
	labelNumLines = new QLabel("Number of Streamlines");
    connect(sbNumLines, SIGNAL(valueChanged(int)), glWidget, SLOT(setNumLines(int)));
	sbNumLines->setValue(20);

	sbNumSteps = new QSpinBox();
	sbNumSteps->setMinimum(50);
	sbNumSteps->setMaximum(5000);
	sbNumSteps->setSingleStep(10);
	labelNumSteps = new QLabel("Number of Steps");
    connect(sbNumSteps, SIGNAL(valueChanged(int)), glWidget, SLOT(setNumSteps(int)));
	sbNumSteps->setValue(200);

	sbStepSize = new QDoubleSpinBox();
	sbStepSize->setMinimum(0.01);
	sbStepSize->setMaximum(1);
	sbNumSteps->setSingleStep(0.05);
	labelStepSize = new QLabel("Step Size");
    connect(sbStepSize, SIGNAL(valueChanged(float)), glWidget, SLOT(setStepSize(float)));
	sbStepSize->setValue(0.25);

	linesGroup = new QGroupBox("Streamlines");
    QGridLayout *linesGroupLayout = new QGridLayout;
    linesGroupLayout->addWidget(checkStreamlines, 1, 1);
    linesGroupLayout->addWidget(eulerButton, 2, 1);
    linesGroupLayout->addWidget(rkButton, 2, 2);
    linesGroupLayout->addWidget(labelNumLines, 3, 1);
	linesGroupLayout->addWidget(sbNumLines, 3, 2);
    linesGroupLayout->addWidget(labelNumSteps, 4, 1);
    linesGroupLayout->addWidget(sbNumSteps, 4, 2);
    linesGroupLayout->addWidget(labelStepSize, 5, 1);
    linesGroupLayout->addWidget(sbStepSize, 5, 2);
    linesGroup->setLayout(linesGroupLayout);

	pauseButton = new QPushButton("Pause", widget);
	connect(pauseButton, SIGNAL(clicked()), glWidget, SLOT(pausePong()));

	resetButton = new QPushButton("Reset", widget);
	connect(resetButton, SIGNAL(clicked()), glWidget, SLOT(resetPong()));

	pongGroup = new QGroupBox("Game");
    QVBoxLayout *pongGroupLayout = new QVBoxLayout;
	pongGroupLayout->addWidget(pauseButton);
	pongGroupLayout->addWidget(resetButton);
	pongGroup->setLayout(pongGroupLayout);

	transferScene = new QGraphicsScene;
	transferView = new TFView(transferScene, glWidget->transferFunction());
	transferView->show();
	transferView->setMaximumHeight(150);

	clearButton = new QPushButton("Clear", widget);
	connect(clearButton, SIGNAL(clicked()), transferView, SLOT(clearTF()));

	sideBar = new QGroupBox;
    QVBoxLayout *sideBarLayout = new QVBoxLayout;
	sideBarLayout->addWidget(arrowGroup);
	sideBarLayout->addWidget(linesGroup);
	sideBarLayout->addWidget(pongGroup);
	sideBarLayout->addWidget(transferView);
	sideBarLayout->addWidget(clearButton);
    sideBarLayout->insertStretch(0);
    sideBar->setFixedWidth(300);
    sideBar->setLayout(sideBarLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(sideBar);
    mainLayout->addWidget(glWidget);
    widget->setLayout(mainLayout);
	
	openAct = new QAction(tr("&Open"), this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip(tr("Load a dataset"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(loadDataset()));

	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openAct);

	bool foundOne;
	do
	{
		foundOne = false;
		QList<QWidget *> allChildWidgets = this->findChildren<QWidget *>();
		allChildWidgets.prepend( this );

		foreach(QWidget* w, allChildWidgets)
		{
			if(w->testAttribute(Qt::WA_PendingResizeEvent))
			{
				QResizeEvent e(w->size(), QSize());
				QApplication::sendEvent(w, &e);
				w->setAttribute(Qt::WA_PendingResizeEvent, false);
				// hack: make QTabWidget think it's visible; no layout otherwise
				w->setAttribute(Qt::WA_WState_Visible, true);
				foundOne = true;
			}
		}
		// Process LayoutRequest events, in particular
		qApp->sendPostedEvents();
		 
		if(!foundOne)
		{
			// Reset visible flag, to avoid crashes in qt
			foreach(QWidget* w, allChildWidgets)
				w->setAttribute(Qt::WA_WState_Visible, false);
		}
	} while(foundOne);
	 
	qint32 sceneWidth = transferView->viewport()->width();
	qint32 sceneHeight = transferView->viewport()->height();

	transferView->setSceneRect(QRectF(0, 0, sceneWidth, sceneHeight));

    setMinimumSize(800, 600);
    setWindowTitle("SimpleVis");

	transferView->drawTF();
}

MainWindow::~MainWindow()
{

}

void MainWindow::loadDataset()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Load Dataset"), "", tr("Flow Geometry (*.gri)"));
	if(!fileName.isNull())
		glWidget->loadDataSet(fileName.toStdString());
}
