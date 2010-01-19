#include "Player.h"
#include "qgl.h"

Player::Player(void)
{
	_pos = vec3();
	_mode = PONG_NEUTRAL;
}

PongMode Player::mode()
{
	return _mode;
}

vec3 Player::pos()
{
	return _pos;
}

void Player::setMode(PongMode mode)
{
	_mode = mode;
}

void Player::setPos(float x, float y)
{
	_pos[0] = x;
	_pos[1] = y;
}

void Player::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(_pos[0], _pos[1], 0);
	glColor3f(0.0, 0.5, 0.8);
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

Player::~Player(void)
{
}
