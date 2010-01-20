#extension GL_ARB_texture_rectangle : enable

varying mat2 rot;
uniform sampler2DRect velocity;
uniform float maxSize;
 
void main()
{
	vec3 vel = texture2DRect(velocity, gl_Vertex.xy).rgb;
	float cos = vel.y/sqrt(vel.x * vel.x + vel.y * vel.y);
	float sin = vel.x/sqrt(vel.x * vel.x + vel.y * vel.y);
	rot = mat2(cos, -sin, sin, cos);
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_PointSize = 	max(maxSize/4, min(maxSize, vel.z * maxSize/10));
}