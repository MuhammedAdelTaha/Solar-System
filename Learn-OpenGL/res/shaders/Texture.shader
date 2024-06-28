#shader vertex
#version 330 core

attribute vec4 aPosition;
attribute vec2 aTexCoord;
attribute vec3 aNormal;

uniform mat4 u_ModelMat;
uniform mat4 u_ViewMat;
uniform mat4 u_ProjectionMat;

varying vec2 v_TexCoord;
varying vec3 v_Normal;
varying vec3 v_Position;

void main()
{
	gl_Position = u_ProjectionMat * u_ViewMat * u_ModelMat * aPosition;
	v_TexCoord = aTexCoord;
	v_Normal = mat3(transpose(inverse(u_ModelMat))) * aNormal;
	v_Position = vec3(u_ModelMat * aPosition);
}

#shader fragment
#version 330 core

uniform float u_TexOpacity;
uniform sampler2D u_Texture;

varying vec2 v_TexCoord;
varying vec3 v_Normal;
varying vec3 v_Position;

void main() 
{
	vec3 lightPos = vec3(0.0f);
	vec3 lightColor = vec3(1.0f, 0.8f, 0.8f);
	
	// Ambient
	float ambientStrength = 0.8f;
	vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse
    vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(lightPos - v_Position);
    float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
    
    // Specular
	float specularStrength = 1.2f;
	vec3 viewDir = normalize(-v_Position);
    vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;


	vec4 texColor = texture(u_Texture, v_TexCoord);
	vec4 result = vec4((ambient + diffuse + specular) * texColor.rgb, u_TexOpacity);
	gl_FragColor = result;
}