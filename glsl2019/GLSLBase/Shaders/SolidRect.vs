#version 450

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;

uniform float u_Time;

void main()
{
	vec2 temp;

	temp.x = a_Position.x + sin(u_Time * 3.141592 * 2.f);
	temp.y = a_Position.y + cos(u_Time * 3.141592 * 2.f);

	gl_Position = vec4(temp, 0.0, 1.0);
}
