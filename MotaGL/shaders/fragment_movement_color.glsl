#version 330 core

in vec4 vertexPos;

out vec4 FragColor;

void main()
{
	FragColor = vertexPos;
}