/*! \file TFTexture.cpp
	\brief TFTexture source file.

	Contains the source code for the TFTexture class, which represents the transfer function
	both as a series of TFNodes and as the texture generated from them.
*/

#include "TFTexture.h"
#include <algorithm>

TFTexture::TFTexture(QWidget *parent)
{
	this->parent = parent;

	nodes = std::map<int, TFNode>();

	addNode(TFNode(0, 0.0, 0.0, 0.7, 0.0));
	addNode(TFNode(2047, 0.8, 0, 0, 1));
}

TFTexture::~TFTexture(void)
{
}

void TFTexture::addNode(TFNode node)
{
	nodes.insert(std::pair<int, TFNode>(node.x, node));
}

void TFTexture::removeNode(GLuint x)
{
	nodes.erase(x);
}

std::map<int, TFNode>::iterator TFTexture::begin()
{
	return nodes.begin();
}

std::map<int, TFNode>::iterator TFTexture::end()
{
	return nodes.end();
}

void TFTexture::clear(void)
{
	nodes.clear();
}

void TFTexture::setFBO(GLuint fbo, GLuint texture)
{
	this->fbo = fbo;
	this->texture = texture;
}

void TFTexture::setHistogram(int* histogram)
{
	int histHeight = 0;
	for (int i = 1; i < 256; i++) {
		if (histogram[i] > histHeight)
			histHeight = histogram[i];
	}
	for (int i = 1; i < 256; i++) {
		this->histogram[i] = static_cast<float>(histogram[i]) / histHeight;
	}
}


void TFTexture::generate(void)
{
	glPushMatrix();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	 
	glViewport(0, 0, 2048, 2048);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, 2048.0, 0.0, 2048.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	 
	glBegin(GL_QUADS);
	 
	std::map<int, TFNode>::iterator node;
	TFNode thisNode, prevNode;
	 
	for(node = nodes.begin(); node != nodes.end(); node++)
	{
		if(node == nodes.begin())
		{
			prevNode = node->second;
			continue;
		}

		thisNode = node->second;

		glColor4f(prevNode.r, prevNode.g, prevNode.b, prevNode.a);
		glVertex3f(prevNode.x, 0, -1);
		glColor4f(thisNode.r, thisNode.g, thisNode.b, thisNode.a);
		glVertex3f(thisNode.x, 0, -1);
		glColor4f(thisNode.r, thisNode.g, thisNode.b, thisNode.a);
		glVertex3f(thisNode.x, 2048, -1);
		glColor4f(prevNode.r, prevNode.g, prevNode.b, prevNode.a);
		glVertex3f(prevNode.x, 2048, -1);
		 
		prevNode = node->second;
	}

	glEnd();
	 
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	 
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	 
	glViewport(0, 0, parent->width(), parent->height());
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glPopMatrix();
}
