#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect tex_grid;
uniform sampler2DRect tex_channel3;
uniform sampler2D tex_transfer;

void main (void) 
{
	vec2 tc = texture2DRect(tex_grid, gl_FragCoord.st).rg;
	float result = texture2DRect(tex_channel3, tc).r;
	
	gl_FragColor = vec4(tc.r, tc.g, 0.0, 1.0);
	
	//gl_FragColor = vec4(result.r, result.g, 0.0, 1.0);
	//gl_FragColor = vec4(result.r, 0.0, 0.0, 1.0);
	//vec3 tf = texture2D(tex_transfer, gl_FragCoord.st).rgb;
	//gl_FragColor = vec4(tf.r, tf.g, tf.b, 1.0);
	
	//gl_FragColor = texture2D(tex_transfer, gl_FragCoord);
	//gl_FragColor = vec4(tex_transfer, gl_FragCoord.st).rgb;
	//gl_FragColor = vec4(1, 1, 0.0, 1.0);
	//gl_FragColor = vec4(1, 1, 0.0, 1.0);
}