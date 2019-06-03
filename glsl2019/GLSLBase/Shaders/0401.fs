#version 440

layout(location=0) out vec4 FragColor;

in vec4 v_Color;
in vec2 v_oriXY;
in float v_Radius;


void main()
{
	vec4 newColor;
	float dis = sqrt(v_oriXY.x * v_oriXY.x + v_oriXY.y * v_oriXY.y);
	if(dis < v_Radius)
	{
		newColor = v_Color;
		newColor.a = 1 - dis / v_Radius;
	}
	else
	{
		newColor = vec4(0, 0, 0, 0);
	}
	FragColor = newColor;
}
