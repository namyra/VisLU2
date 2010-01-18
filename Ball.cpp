#include "Ball.h"
#include "qgl.h"

Ball::Ball(float x, float y, vec3 vel)
{
	_x = x;
	_y = y;
	_vel = vel;
}

float Ball::x()
{
	return _x;
}

float Ball::y()
{
	return _y;
}

void Ball::setPos(float x, float y)
{
	_x = x;
	_y = y;
}

void Ball::draw()
{
	glColor3f(0.6, 0.0, 0.8);
	glBegin(GL_QUADS);
		//glTexCoord2f(0.0f, 0.0f);
		glVertex2f(_x - 5, _y - 5);
		//glTexCoord2f(1, 0.0f);
		glVertex2f(_x + 5, _y - 5);
		//glTexCoord2f(1, 1);
		glVertex2f(_x + 5, _y + 5);
		//glTexCoord2f(0.0f, 1);
		glVertex2f(_x - 5, _y + 5);
	glEnd();
}

void Ball::update()
{
	_x += _vel.v[0];
	_y += _vel.v[1];
}

void Ball::update(vec3 vel)
{
	_vel = _vel/2 + vel/2;
	_x += _vel.v[0];
	_y += _vel.v[1];
}

Ball::~Ball(void)
{
}
