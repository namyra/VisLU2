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

	checkArrowPlot = new QCheckBox("Arrow Plot", widget);
	connect(checkArrowPlot, SIGNAL(toggled(bool)), glWidget, SLOT(toggleArrowPlot(bool)));
	checkArrowPlot->setChecked(true);

	checkStreamlines = new QCheckBox("Streamlines", widget);
	connect(checkStreamlines, SIGNAL(toggled(bool)), glWidget, SLOT(toggleStreamlines(bool)));
	checkStreamlines->setChecked(true);

    rkButtons = new QButtonGroup;
    eulerButton = new QRadioButton("Euler");
    rkButton = new QRadioButton("Runge-Kutta");
    rkButtons->addButton(eulerButton, 0);
    rkButtons->addButton(rkButton, 1);

	connect(rkButton, SIGNAL(toggled(bool)), glWidget, SLOT(setRK(bool)));
    rkButton->setChecked(true);

    rkGroup = new QGroupBox;
    QHBoxLayout *rkGroupLayout = new QHBoxLayout;
    rkGroupLayout->addWidget(eulerButton);
    rkGroupLayout->addWidget(rkButton);
    rkGroup->setLayout(rkGroupLayout);

	pauseButton = new QPushButton("Pause", widget);
	connect(pauseButton, SIGNAL(clicked()), glWidget, SLOT(pausePong()));

	resetButton = new QPushButton("Reset", widget);
	connect(resetButton, SIGNAL(clicked()), glWidget, SLOT(resetPong()));

	transferScene = new QGraphicsScene;
	transferView = new TFView(transferScene, glWidget->transferFunction());
	transferView->show();
	transferView->setMaximumHeight(150);

	clearButton = new QPushButton("Clear", widget);
	connect(clearButton, SIGNAL(clicked()), transferView, SLOT(clearTF()));

	sideBar = new QGroupBox;
    QVBoxLayout *sideBarLayout = new QVBoxLayout;
	sideBarLayout->addWidget(checkArrowPlot);
	sideBarLayout->addWidget(checkStreamlines);
	sideBarLayout->addWidget(rkGroup);
	sideBarLayout->addWidget(resetButton);
	sideBarLayout->addWidget(pauseButton);
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
