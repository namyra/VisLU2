/*! \file TFView.h
	\brief TFView header file.

	Contains the declarations for the TFView class, which is the GUI element that displays the transfer function.
*/

#pragma once

#include <QDebug>

#include <QGraphicsView>
#include <qcolordialog.h>
#include "tftexture.h"


//! Display widget for the transfer function.
/*!
	Provides the widget that displays the transfer function as a series of coloured nodes connected by dotted lines.
	The x-axis represents the density values, the y-axis the opacity of a node.
	A histogram of the currently loaded volume dataset is shown in the background.
*/
class TFView :
	public QGraphicsView
{
public:

	//! Constructor.
	/*!
		\param scene The Qt scene used by this view.
		\param transferFunction The transfer function to display.
		\sa ~TFView()
	*/
	TFView(QGraphicsScene *scene, TFTexture *transferFunction);

	//! Default destructor.
	/*!
		\sa TFView()
	*/
	~TFView(void);

	//! Event handler called when a mouse button is clicked on the widget.
	/*!
		If the left mouse button is clicked outside an existing node,
		a colour chooser dialog opens and a new node is created at the clicked position with the selected colour.
		If the right mouse button is clicked on a node, the node is deleted.
		If the right mouse button is clicked outside an existing node, the transfer function is cleared.
		Afterwards the view is redrawn and the underlying transfer function is re-generated.
		\param event The mouse event being handled.
		\sa drawTF()
	*/
	void mousePressEvent(QMouseEvent *event);

	//! Helper function that clears the scene.
	/*!
		Removes all items in the scene.
	*/
	void clear();

	//! Generates the scene and draws the view.
	/*!
		First clears the scene, then creates the bars for the histogram, the nodes, and the lines connecting them, then finally updates the widget.
		\sa clear()
	*/
	void drawTF();

private:

	//! The start position of a drag and drop operation.
	QPoint dragStartPosition;

	//! The underlying transfer texture.
	TFTexture *tf;
};
