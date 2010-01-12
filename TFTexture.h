/*! \file TFTexture.h
	\brief TFTexture header file.

	Contains the declarations for the TFTexture class, which represents the transfer function
	both as a series of TFNodes and as the texture generated from them.
*/

#pragma once

#include <QDebug>
#include "TFNode.h"
#include "qgl.h"

//! The transfer function.
/*!
	This class represents the transfer function as an ordered collection of TFNodes.
	It also generates the texture for the transfer function used by the shaders.
*/
class TFTexture
{
public:

	//! Constructor.
	/*!
		Initialises the transfer function with a few default values.
		\param parent The widget using this transfer function.
		\sa ~TFTexture()
	*/
	TFTexture(QWidget *parent);

	//! Default destructor.
	/*!
		\sa TFTexture()
	*/
	~TFTexture(void);

	//! Adds a node to the transfer function.
	/*!
		Wrapper function for the underlying map's insert().
		\param node The node to add.
		\sa removeNode(), clear()
	*/
	void addNode(TFNode node);

	//! Removes a node from the transfer function.
	/*!
		Wrapper function for the underlying map's erase().
		\param x The x-coordinate of the node to remove.
		\sa addNode(), clear()
	*/
	void removeNode(GLuint x);

	//! Clears the transfer function.
	/*!
		Wrapper function for the underlying map's clear().
		\sa addNode(), removeNode()
	*/
	void clear(void);

	//! Returns an iterator at the first node.
	/*!
		Wrapper function for the underlying map's begin().
		\return The iterator at the beginning of the map.
		\sa end()
	*/
	std::map<int, TFNode>::iterator begin();

	//! Returns an iterator at the last node.
	/*!
		Wrapper function for the underlying map's end().
		\return The iterator at the end of the map.
		\sa begin()
	*/
	std::map<int, TFNode>::iterator end();

	//! Sets the frame buffer and texture used to generate the transfer function.
	/*!
		\param fbo The OpenGL id of the frame buffer to use.
		\param texture The OpenGL id of the texture to use.
	*/
	void setFBO(GLuint fbo, GLuint texture);

	//! Generates the texture that stores the transfer function.
	/*!
		Draws a number of quads, interpolating linearly between the colours and opacities of each pair of nodes.
	*/
	void generate(void);

	//! Sets the histogram used in the transfer function display.
	/*!
		\param histogram The histogram data.
	*/
	void setHistogram(int* histogram);

	//! The histogram data.
	float histogram[256];

private:
	
	//! The map containing the nodes of the transfer function.
	std::map<int, TFNode> nodes;

	//! The widget using this transfer function.
	QWidget *parent;

	//! The OpenGL id of the frame buffer used to generate the transfer function texture.
	GLuint fbo;

	//! The OpenGL id of the texture used.
	GLuint texture;
};

