#include "vec3.h"

class Ball
{
public:
	Ball(float x = 0, float y = 0, float velX = 0, float velY = 0);
	~Ball(void);
	void draw();
	void update();

private:
	float _x;
	float _y;
	float _velX;
	float _velY;
};

