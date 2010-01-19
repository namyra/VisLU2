#include "Ball.h"
#include "qgl.h"

Ball::Ball(vec3 pos, vec3 vel)
{
	_pos = pos;
	_vel = vel;
}

Ball::Ball(float x, float y, float velX, float velY)
{
	_pos[0] = x;
	_pos[1] = y;
	_vel[0] = velX;
	_vel[1] = velY;
}

vec3 Ball::pos()
{
	return _pos;
}

float Ball::x()
{
	return _pos[0];
}

float Ball::y()
{
	return _pos[1];
}

void Ball::setPos(vec3 pos)
{
	_pos = pos;
}

void Ball::setPos(float x, float y)
{
	_pos[0] = x;
	_pos[1] = y;
}

void Ball::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(_pos[0], _pos[1], 0);
	glColor3f(0.6, 0.0, 0.8);
	glBegin(GL_QUADS);
		//glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-5, -5);
		//glTexCoord2f(1, 0.0f);
		glVertex2f(5, -5);
		//glTexCoord2f(1, 1);
		glVertex2f(5, 5);
		//glTexCoord2f(0.0f, 1);
		glVertex2f(-5, 5);
	glEnd();
	glPopMatrix();
}

void Ball::update()
{
	_pos = proposeMove();
}

void Ball::update(vec3 vel)
{
	_vel = _vel/2 + vel/2;
	_pos = proposeMove();
}

vec3 Ball::proposeMove()
{
	return _pos + _vel;
}

Ball::~Ball(void)
{
}
