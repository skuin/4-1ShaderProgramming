#version 450

in vec3 a_Position;
in vec2 a_Tex;

out vec2 v_Tex;

void main()
{
	v_Tex = a_Tex;
	gl_Position = vec4(a_Position.xyz, 1);
}
