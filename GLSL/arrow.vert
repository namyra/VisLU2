#extension GL_ARB_texture_rectangle : enable

varying mat2 rot;
uniform sampler2DRect velocity;
uniform float maxSize;
uniform bool flipped;

void main()
{
	vec3 vel = texture2DRect(velocity, gl_Vertex.xy).rgb;
	float cos, sin;
	if (flipped  == true) {
		cos = vel.y/sqrt(vel.x * vel.x + vel.y * vel.y);
		sin = vel.x/sqrt(vel.x * vel.x + vel.y * vel.y);
	} else {
		cos = vel.x/sqrt(vel.y * vel.y + vel.x * vel.x);
		sin = vel.y/sqrt(vel.y * vel.y + vel.x * vel.x);
	}
	rot = mat2(cos, -sin, sin, cos);
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_PointSize = maxSize;
}