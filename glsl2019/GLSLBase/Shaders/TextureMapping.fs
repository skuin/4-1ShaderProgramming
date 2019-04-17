#version 450

layout(location=0) out vec4 FragColor;

in vec2 v_Tex;

uniform float u_Time;

void main()
{
	vec4 newColor = vec4(0);

	// ¹®Á¦!
	float x = v_Tex.x * 3.141592 * 2 + u_Time;
	float value = ((sin(x)+1)*0.5);
	if(value >= v_Tex.y && value - 0.01 <= v_Tex.y)
	{
		newColor = vec4(1);
	}
	else
	{
		newColor = vec4(0);
	}
	FragColor = newColor;

}
