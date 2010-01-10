/*! \file TFNode.h
	\brief TFNode header file.

	Contains the declarations for the TFNode class, which represents a single node of the transfer function.
*/


#pragma once

#include "common.h"

//! A node of the transfer function.
/*!
	A simple class to wrap the x coordinate, colour, and opacity of a single transfer function node.
*/
class TFNode
{
public:

	//! Default constructor.
	/*!
		Constructs a node with all values set to 0.
		\sa TFNode(), ~TFNode()
	*/
	TFNode(void);

	//! Constructor.
	/*!
		Constructs a node with the provided values.
		\param x The node's x-coordinate.
		\param r The red component of the node's colour.
		\param g The green component of the node's colour.
		\param b The blue component of the node's colour.
		\param a The alpha component of the node's colour.
		\sa TFNode(GLuint, GLfloat, GLfloat, GLfloat, GLfloat), ~TFNode()
	*/
	TFNode(GLuint x, GLfloat r, GLfloat g, GLfloat b, GLfloat a);

	//! Default destructor.
	/*!
		\sa TFNode()
	*/
	~TFNode(void);

	//! Less-than comparison operator.
	/*!
		Allows for ordering of the nodes. One node is less than another if its x coordinate is lower.
		\param node The other node to compare this one with.
		\return \a true if this node's x-coordinate is lower than the other node's, \a false otherwise.
	*/
	bool operator<(const TFNode &node);

	//! The node's x-coordinate.
	GLuint x;

	//! The red component of the node's colour.
	GLfloat r;

	//! The green component of the node's colour.
	GLfloat g;

	//! The blue component of the node's colour.
	GLfloat b;

	//! The alpha component of the node's colour.
	GLfloat a;
};

