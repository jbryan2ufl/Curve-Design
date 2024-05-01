#version 330 core

out vec4 FragColor;

uniform vec4 color;

void main()
{
	float distance = length(vec2(0.5, 0.5) - gl_PointCoord.xy);

	if (distance <= 0.5)
	{
		FragColor = vec4(color); // Inside the circle, set color to red
	}
	else
	{
		discard; // Outside the circle, discard the fragment
	}
}