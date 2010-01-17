#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect tex_grid;
uniform sampler2DRect tex_channel3;
uniform sampler2D tex_transfer;

void main (void) 
{
	vec2 tc = texture2DRect(tex_channel3, gl_FragCoord.st).rg;
	vec2 result = texture2DRect(tex_channel3, tc).rg;
	
	gl_FragColor = vec4(tc.r, tc.g, 0.0, 1.0);
}