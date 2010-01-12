/*! \file TFNode.cpp
	\brief TFNode source file.

	Contains the source file for the TFNode class, which represents a single node of the transfer function.
*/

#include "TFNode.h"

TFNode::TFNode(void)
{
	x = 0;
	r = g = b = a = 0.0;
}

TFNode::TFNode(GLuint x, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	this->x = x;
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

TFNode::~TFNode(void)
{
}

bool TFNode::operator < (const TFNode &node)
{
	return this->x < node.x;
}
