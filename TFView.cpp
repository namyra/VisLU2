/*! \file TFView.cpp
	\brief TFView source file.

	Contains the source code for the TFView class, which is the GUI element that displays the transfer function.
*/

#include "TFView.h"
#include <QMouseEvent>
#include <QGraphicsItem>
#include <algorithm>

TFView::TFView(QGraphicsScene *scene, TFTexture *tf) : QGraphicsView(scene)
{
	this->tf = tf;
}

TFView::~TFView(void)
{
}

void TFView::clear()
{
	QList<QGraphicsItem *> allItems = this->items();

	foreach(QGraphicsItem* i, allItems)
	{
		scene()->removeItem(i);
	}
}

void TFView::clearTF()
{
	tf->clear();
	tf->generate();
	drawTF();
}

void TFView::drawTF()
{
	qint32 sceneWidth = viewport()->width();
	qint32 sceneHeight = viewport()->height();
	 
	clear();
	 
	std::map<int, TFNode>::iterator node;
	TFNode thisNode, prevNode;
	
	//for (unsigned int i = 1; i < 256; i++) {
	//	QPen pen(QColor(i, 0.25, 255 - i, 255), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	//	scene()->addLine(static_cast<double>(i) / 256 * sceneWidth, sceneHeight, static_cast<double>(i) / 256 * sceneWidth, sceneHeight * (1 - tf->histogram[i]), pen);
	//}

	QPen pen2(Qt::black, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);

	for(node = tf->begin(); node != tf->end(); node++)
	{
		thisNode = node->second;

		QGraphicsRectItem *rect = scene()->addRect(0, 0, 9, 9, QPen(), QBrush(QColor(thisNode.r * 255, thisNode.g * 255, thisNode.b * 255)));
		rect->setPos(((float)thisNode.x / 2048 * sceneWidth - 4) , (1 - thisNode.a) * sceneHeight - 4);
		 
		if(node == tf->begin())
		{
			prevNode = node->second;
			continue;
		}
		 
		QGraphicsLineItem *line = scene()->addLine((float)prevNode.x / 2048 * sceneWidth, (1 - prevNode.a) * sceneHeight, (float)thisNode.x / 2048 * sceneWidth, (1 - thisNode.a) * sceneHeight, pen2);
		line->setZValue(-1);

		prevNode = node->second;
	}
}

void TFView::mousePressEvent(QMouseEvent *event)
{
	qint32 sceneWidth = viewport()->width();
	qint32 sceneHeight = viewport()->height();
	 
	QPointF pos = mapToScene(event->pos());
		 
	if(event->button() == Qt::LeftButton)
	{
		if(scene()->itemAt(pos) && scene()->itemAt(pos)->type() == 3)
		{
			dragStartPosition = event->pos();
		}
		else
		{

			QColor color = QColorDialog::getColor(Qt::green, this);
			if (color.isValid()) {
				float r = color.redF();
				float g = color.greenF();
				float b = color.blueF();

				tf->addNode(TFNode(pos.x() / sceneWidth * 2048, r, g, b, 1 - pos.y() / sceneHeight));
				tf->generate();
				drawTF();
			}
		}
	}
	else if(event->button() == Qt::RightButton)
	{
		if(scene()->itemAt(pos) && scene()->itemAt(pos)->type() == 3)
		{
			tf->removeNode((scene()->itemAt(pos)->pos().x() + 4) / sceneWidth * 2048);
			tf->generate();
			drawTF();
		}
	}
}