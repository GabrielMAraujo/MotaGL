#version 330 core

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float ambientStrength;

void main()
{
	vec3 ambient = ambientStrength * lightColor;

	FragColor = vec4(ambient * objectColor, 1.0f);
}