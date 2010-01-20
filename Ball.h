/*! \file Ball.h
	\brief Ball header file.

	Contains the declarations for the Ball class, a representation of the ball for the Pong game.
*/

#pragma once

#include "vec3.h"
#include "Player.h"

//! Representation of the player of the Pong game.
/*!
	This class stores the position and velocty of the ball, updates and draws it.
*/
class Ball
{
public:

	//! Constructor.
	/*!
		Constructor that sets the ball's position and velocity to the given values.
		\param pos The ball's initial position. Defaults to (0, 0).
		\param vel The ball's initial velocity. Defaults to (0, 0).
		\sa ~Ball(float, float, float, float), ~Ball()
	*/
	Ball(vec3 pos = vec3(), vec3 vel = vec3());

	//! Constructor.
	/*!
		Constructor that sets the ball's position and velocity to the given values.
		\param x The ball's initial x-coordinate.
		\param y The ball's initial y-coordinate.
		\param velX The x-coordinate of the ball's initial velocity. Defaults to 0.
		\param velY The y-coordinate of the ball's initial velocity. Defaults to 0.
		\sa ~Ball(vec3, vec3), ~Ball()
	*/
	Ball(float x, float y, float velX = 0, float velY = 0);

	//! Default destructor.
	/*!
		\sa Ball(vec3, vec3), Ball(float, float, float, float)
	*/
	~Ball(void);

	//! Returns the ball's current position.
	/*
		\return The ball's current position.
		\sa setPos(vec3), setPos(float, float), x(), y()
	!*/
	vec3 pos();

	//! Returns the ball's current x-coordinate.
	/*!
		\return The ball's current x-coordinate.
		\sa y(), pos(), setPos(vec3), setPos(float, float)
	*/
	float x();

	//! Returns the ball's current y-coordinate.
	/*!
		\return The ball's current y-coordinate.
		\sa x(), pos(), setPos(vec3), setPos(float, float)
	*/
	float y();

	//! Sets the ball's current position.
	/*!
		\param pos The ball's new position.
		\sa setPos(float, float), pos(), x(), y()
	*/
	void setPos(vec3 pos);

	//! Sets the ball's current position.
	/*!
		\param x The ball's new x-coordinate.
		\param y The ball's new y-coordinate.
		\sa setPos(float, float), pos(), x(), y()
	*/
	void setPos(float x, float y);

	//! Sets the ball's current velocity.
	/*!
		\param vel The ball's new velocity. Defaults to (0, 0).
	*/
	void setVel(vec3 vel = vec3());

	//! Draws the graphical representation of the ball in OpenGL.
	void draw();

	//! Updates the ball's current position using only its own velocity.
	/*!
		Calls proposeMove() to simply move one step ahead according to the ball's current velocity.
		\sa proposeMove(), update(vec3, vec3, PongMode)
	*/
	void update();

	//! Updates the ball's current position and velocity using external forces.
	/*!
		First updates the ball's current velocity by taking an average of it's own velocity and the velocity of the flow data at its position.
		Then, if the player is currently in attraction or repulsion mode, the ball's velocity is further adjusted using an appropriate attraction or
		repulsion force, inversely strong to the distance between ball and player.
		Finally calls proposeMove() to move one step ahead according to the ball's updated velocity.
		\sa proposeMove(), update()
	*/
	void update(vec3 vel, vec3 playerPos, PongMode mode);

private:

	//! The ball's position.
	vec3 _pos;

	//! The ball's velocity.
	vec3 _vel;

	//! Proposes the next step of movement for the ball.
	/*!
		Simply adds the ball's current velocity vector to its current position.
		\return The proposed new position.
		\sa update(), update(vec3, vec3, PongMode)
	*/
	vec3 proposeMove();
};
