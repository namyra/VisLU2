#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect tex_grid, tex_channel3;
uniform sampler2D tex_transfer;
uniform sampler2DRect tex_inverseX, tex_inverseY;
uniform bool flipped;
uniform float width, height;

void main (void) 
{
	vec2 tc = vec2(gl_TexCoord[0].x, gl_TexCoord[0].y);
	vec2 pos = vec2(texture2DRect(tex_inverseX, vec2(tc.x, 0.0)).x, texture2DRect(tex_inverseY, vec2(tc.y, 0.0)).x);
	float tf = texture2DRect(tex_channel3, vec2(pos.x*width, pos.y*height)).r;
	gl_FragColor = texture2D(tex_transfer, vec2(tf, 0.0));
}