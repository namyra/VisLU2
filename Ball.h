#include "vec3.h"

class Ball
{
public:
	Ball(vec3 pos = vec3(), vec3 vel = vec3());
	Ball(float x, float y, float velX = 0, float velY = 0);
	~Ball(void);
	vec3 pos();
	float x();
	float y();
	void setPos(vec3 pos);
	void setPos(float x, float y);
	void draw();
	void update();
	void update(vec3 vel);
	vec3 proposeMove();

private:
	vec3 _pos;
	vec3 _vel;
};

