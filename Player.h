/*! \file Player.h
	\brief Player header file.

	Contains the declarations for the Player class, a representation of the player of the Pong game.
*/

#pragma once

#include "vec3.h"

//! The different modes for the player: neutral, attracting the ball, or repulsing the ball.
enum PongMode{PONG_NEUTRAL, PONG_ATTRACTION, PONG_REPULSION};

//! Representation of the player of the Pong game.
/*!
	This simple class stores the position and mode of the player, i.e. the cursor and draws it.
*/
class Player
{
public:
	//! Default constructor.
	/*!
		\sa ~Player()
	*/
	Player(void);

	//! Default destructor.
	/*!
		\sa Player()
	*/
	~Player(void);

	//! Returns the player's mode.
	/*!
		\return The player's attraction/repulsion mode.
		\sa setMode()
	*/
	PongMode mode();

	//! Returns the player's position.
	/*!
		\return The player's position.
		\sa setPos()
	*/
	vec3 pos();

	//! Sets the player's mode.
	/*!
		\param mode The player's new mode.
		\sa mode()
	*/
	void setMode(PongMode mode);

	//! Sets the player's position.
	/*!
		\param x The player's new x-coordinate.
		\param y The player's new y-coordinate.
		\sa pos()
	*/
	void setPos(float x, float y);

	//! Draws the graphical representation of the player in OpenGL.
	void draw();

private:
	//! The player's position. z-coordinate is unused.
	vec3 _pos;

	//! The player's attraction/repulsion mode.
	PongMode _mode;
};

