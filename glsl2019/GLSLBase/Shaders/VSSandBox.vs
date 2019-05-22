#version 450

layout(location = 0) in vec3 a_Position;
uniform float u_Time;

out float v_Grey;
out vec2 v_Tex;

void main()
{
	vec3 newPos = a_Position;

	// 0~1 ªÁ¿Ã
	float additionalValueX = newPos.x + 0.5;
	float additionalValueY = newPos.y + 0.5;

	float periodX = 1.0 + (1.0 - additionalValueY) * 0.5;
	float periodY = 1.0 + additionalValueX * 0.5;
	
	float valueX = (additionalValueY * 2 * 3.141592 * periodX) - u_Time*7.0;
	float valueY = (additionalValueX * 2 * 3.141592 * periodY) - u_Time*5.0;

	float sinValueX = sin(valueX) * 0.1;
	float sinValueY = sin(valueY) * 0.2;

	// y Scale
	newPos.y = newPos.y * (1.0-additionalValueX*0.4);
	
	// x
	newPos.x = newPos.x - sinValueX * additionalValueX;
	// y
	newPos.y = newPos.y + sinValueY * additionalValueX;

	gl_Position = vec4(newPos, 1);

	v_Grey = sinValueY + 0.5;
	v_Tex = vec2(0.5,0.5) + a_Position.xy; // 0~1 Tex Coordinate
}
