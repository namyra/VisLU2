varying mat2 rot;
uniform sampler2D velocity;

void main()
{
	//float vel_x = texture2D(velocity, gl_Vertex.xy).r;
	//float vel_y = texture2D(velocity, gl_Vertex.xy).g;
	float vel_x = gl_Vertex.x;
	float vel_y = gl_Vertex.y;
	float cos = vel_x/sqrt(vel_x * vel_x + vel_y * vel_y);
	float sin = vel_y/sqrt(vel_x * vel_x + vel_y * vel_y);
	rot = mat2(cos, -sin, sin, cos);
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}