varying mat2 rot;
uniform sampler2D arrow;

void main()
{
	vec2 h = vec2(0.5, 0.5);
	gl_FragColor = texture2D(arrow, (rot * (gl_TexCoord[0].st - h)) + h);
}