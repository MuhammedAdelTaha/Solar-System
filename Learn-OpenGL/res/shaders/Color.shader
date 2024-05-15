#shader vertex
#version 330 core

attribute vec4 aPosition;

uniform mat4 u_ModelMat;
uniform mat4 u_ViewMat;
uniform mat4 u_ProjectionMat;

void main()
{
	gl_Position = u_ProjectionMat * u_ViewMat * u_ModelMat * aPosition;
}

#shader fragment
#version 330 core

uniform vec4 u_Color;

void main() 
{
	gl_FragColor = u_Color;
}