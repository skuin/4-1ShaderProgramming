#version 450

in vec4 a_Position;
in vec2 a_UV;

out vec4 v_Color;

uniform float u_Time;

void main()
{
	gl_Position = vec4(a_Position.xyz, 1.0);

	// 두개는 같은 값을 가진다.
	//v_Color = vec4(a_UV.xy, 0, 1.0);
	//v_Color = vec4(a_Position.xy+vec2(0.5,0.5), 0, 1.0);

	v_Color = vec4(a_Position.xy, 0, 1);
}
