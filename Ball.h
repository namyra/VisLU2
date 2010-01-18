#include "vec3.h"

class Ball
{
public:
	Ball(float x = 0, float y = 0, vec3 vel = vec3());
	~Ball(void);
	float x();
	float y();
	void setPos(float x, float y);
	void draw();
	void update();
	void update(vec3 vel);

private:
	float _x;
	float _y;
	vec3 _vel;
};

