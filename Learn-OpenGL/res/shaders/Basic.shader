#shader vertex
#version 330 core

attribute vec4 position;
attribute vec2 texCoord;

uniform mat4 u_MVP;

varying vec2 v_TexCoord;

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

uniform int u_UseTexture;
uniform float u_Opacity;
uniform sampler2D u_Texture;

varying vec2 v_TexCoord;

void main() 
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	if (u_UseTexture == 1)
		gl_FragColor = vec4(texColor.rgb, u_Opacity);
	else
		gl_FragColor = vec4(1.0f, 1.0f, 1.0f, u_Opacity);
}