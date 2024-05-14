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

uniform vec4 u_Color;
uniform bool u_UseTexture;
uniform float u_TexOpacity;
uniform sampler2D u_Texture;

varying vec2 v_TexCoord;

void main() 
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	if (u_UseTexture)
		gl_FragColor = vec4(texColor.rgb, u_TexOpacity);
	else
		gl_FragColor = u_Color;
}