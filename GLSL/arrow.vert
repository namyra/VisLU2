#extension GL_ARB_texture_rectangle : enable

varying mat2 rot;
uniform sampler2DRect velocity;
 
void main()
{
	vec3 vel = texture2DRect(velocity, gl_Vertex.xy).rgb;
	float cos = vel.y/sqrt(vel.x * vel.x + vel.y * vel.y);
	float sin = vel.x/sqrt(vel.x * vel.x + vel.y * vel.y);
	rot = mat2(cos, -sin, sin, cos);
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_PointSize = min(32.0, vel.z * 6.0 + 6.0);
}