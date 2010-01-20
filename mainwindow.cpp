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

	labelNumArrows = new QLabel("Number of Arrows");
	sbNumArrows = new QSpinBox();
	sbNumArrows->setMinimum(10);
	sbNumArrows->setMaximum(50);
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

	labelNumLines = new QLabel("Number of Streamlines");
	sbNumLines = new QSpinBox();
	sbNumLines->setMinimum(10);
	sbNumLines->setMaximum(50);
    connect(sbNumLines, SIGNAL(valueChanged(int)), glWidget, SLOT(setNumLines(int)));
	sbNumLines->setValue(20);

	labelNumSteps = new QLabel("Number of Iteration Steps");
	sbNumSteps = new QSpinBox();
	sbNumSteps->setObjectName("numStepBox");
	sbNumSteps->setMinimum(1);
	sbNumSteps->setMaximum(5000);
    connect(sbNumSteps, SIGNAL(valueChanged(int)), glWidget, SLOT(setNumSteps(int)));
	sbNumSteps->setValue(200);

	labelStepSize = new QLabel("Step Size");
	sbStepSize = new QDoubleSpinBox();
	sbStepSize->setObjectName("stepSizeBox");
	sbStepSize->setMinimum(0.01);
	sbStepSize->setMaximum(1);
	sbStepSize->setSingleStep(0.01);
    connect(sbStepSize, SIGNAL(valueChanged(double)), glWidget, SLOT(setStepSize(double)));
	sbStepSize->setValue(0.25);

	checkLockedSteps = new QCheckBox("Ratio Locked", widget);
	connect(checkLockedSteps, SIGNAL(toggled(bool)), glWidget, SLOT(lockStepSize(bool)));
	checkLockedSteps->setChecked(true);

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
    linesGroupLayout->addWidget(checkLockedSteps, 6, 1);
    linesGroup->setLayout(linesGroupLayout);

	checkPong = new QCheckBox("Enabled", widget);
	connect(checkPong, SIGNAL(toggled(bool)), glWidget, SLOT(togglePong(bool)));
	checkPong->setChecked(false);

	pauseButton = new QPushButton("&Pause", widget);

	connect(pauseButton, SIGNAL(clicked()), glWidget, SLOT(pausePong()));

	resetButton = new QPushButton("Reset", widget);
	connect(resetButton, SIGNAL(clicked()), glWidget, SLOT(resetPong()));

	pongGroup = new QGroupBox("Game");
    QVBoxLayout *pongGroupLayout = new QVBoxLayout;
	pongGroupLayout->addWidget(checkPong);
	pongGroupLayout->addWidget(pauseButton);
	pongGroupLayout->addWidget(resetButton);
	pongGroup->setLayout(pongGroupLayout);

	transferScene = new QGraphicsScene;
	transferView = new TFView(transferScene, glWidget->transferFunction());
	transferView->show();
	transferView->setMinimumHeight(100);
	transferView->setMaximumHeight(100);

	clearButton = new QPushButton("Clear", widget);
	connect(clearButton, SIGNAL(clicked()), transferView, SLOT(clearTF()));

	tfGroup = new QGroupBox("Transfer Function");
    QVBoxLayout *tfGroupLayout = new QVBoxLayout;
	tfGroupLayout->addWidget(transferView);
	tfGroupLayout->addWidget(clearButton);
	tfGroup->setLayout(tfGroupLayout);

	sideBar = new QGroupBox;
    QVBoxLayout *sideBarLayout = new QVBoxLayout;
	sideBarLayout->addWidget(arrowGroup);
	sideBarLayout->addWidget(linesGroup);
	sideBarLayout->addWidget(pongGroup);
	sideBarLayout->addWidget(tfGroup);
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
