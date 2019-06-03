#version 440

in vec3 a_Position;
in vec2 a_UV;

out vec2 v_Temp;


void main()
{
	gl_Position = vec4(a_Position, 1);
	v_Temp = a_UV;
}
