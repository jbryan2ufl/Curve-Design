#version 330 core

out vec4 FragColor;

in vec4 color;

void main()
{
	FragColor = color;
	// FragColor = vec4(1.0);
}