#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect tex_grid;
uniform sampler2DRect tex_channel3;
uniform sampler2D tex_transfer;

vec2 cellAt(vec2 position)
{	
	vec2 coord = vec2((position.y - min.y) / step.y, (position.x - min.x) / step.x);
	
	return vec2(texture2DRect(inverseGridTextureY, coord).x, texture2DRect(inverseGridTextureX, coord).x);
}

void main (void) 
{
	//vec2 tc = texture2DRect(tex_channel3, gl_FragCoord.st).rg;
	//vec2 result = texture2DRect(tex_channel3, tc).rg;
	
	//gl_FragColor = vec4(tc.r, tc.g, 0.0, 1.0);

	//float sample = (gl_TexCoord[0].x - 8.49314) / (20.4381 - 8.49314);
	//vec4 color = vec4(
	//gl_FragColor = vec4(sample, sample, sample, sample);
	
	//vec2 tc = texture2DRect(tex_channel3, vec2(110.0, 311.0)).rg;
	//gl_FragColor = vec4(tc.r, tc.g, 0.0, 1.0);
	
	//vec2 tc = texture2DRect(tex_grid, gl_FragCoord.st).rg;
	//tc.x = tc.x * 500.0;
	//tc.y = tc.y * 500.0;
	//tc.x = 1 - tc.x;
	//tc.x = tc.x *314.0;
	//tc.y = 1 - tc.y;
	//tc.y = tc.y *538.0;
	//tc.x = tc.x /0.003611;
	//tc.y = tc.y /0.003611;
	//gl_FragColor = texture2DRect(tex_channel3, tc);
	//gl_FragColor = vec4(tc.r, tc.g, 0.0, 1.0);
	
	//vec2 tc = vec2(gl_TexCoord[0].x, gl_TexCoord[0].y);
	//vec2 coord = vec2((tc.y) / min.x, (tc.x) / min.y);
	
	vec2 tc = texture2DRect(tex_grid, gl_FragCoord.st).rg;
	vec2 c = cellAt(tc);
	vec2(texture2DRect(inverseGridTextureY, coord).x, texture2DRect(inverseGridTextureX, coord).x);
	
	
}