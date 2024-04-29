#version 330 core

out vec4 FragColor;

in vec3 color;

void main()
{
	FragColor = vec4(color, 1.0);
	// FragColor = vec4(1.0, 0.5, 0.5, 1.0);
}