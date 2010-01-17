#include "Ball.h"
#include "qgl.h"

Ball::Ball(float x, float y, float velX, float velY)
{
	_x = x;
	_y = y;
	_velX = velX;
	_velY = velY;
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
	_x += _velX;
	_y += _velY;
}

Ball::~Ball(void)
{
}
