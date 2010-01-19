#pragma once

#include "vec3.h"

enum PongMode {PONG_NEUTRAL, PONG_ATTRACTION, PONG_REPULSION};

class Player
{
public:
	Player(void);
	~Player(void);
	PongMode mode();
	vec3 pos();
	void setMode(PongMode mode);
	void setPos(float x, float y);
	void draw();

private:
	vec3 _pos;
	PongMode _mode;
};

