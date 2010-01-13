uniform sampler2D arrow;

void main()
{
	vec2 h = vec2(0.5, 0.5);
	float angle = 0.z;
	mat2 rot = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
	gl_FragColor = texture2D(arrow, (rot * (gl_TexCoord[0].st - h)) + h);
}