#version 450

layout(location = 0) in vec3 a_Position;

const float PI = 3.141592;

uniform float u_Time;
uniform vec2 u_Points[5];
uniform mat4 u_ProjView;

out float v_Grey;
out vec2 v_Tex;

void Flag()
{
	/* 깃발 펄럭이는거 */
	vec3 newPos = a_Position;

	// 0~1 사이
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

void Wave()
{
	float grey = 0;
	vec3 newPos = a_Position.xyz;

	for(int i = 0; i<5; i++){
		// 입력된 버텍스의 x,y좌표
		vec2 target;
		vec2 source;

		target = a_Position.xy;
		source = u_Points[i];
		float dis = length( target - source ) * 4 * 3.141592 * 2; // 0~0.5 --> 0~2PI로 변환
		grey += sin(dis - u_Time);
	}

	newPos.z += grey;

	gl_Position = vec4(newPos.xyz, 1);

	v_Grey = (grey + 1.0)/2.0;
	v_Tex = vec2(0.5, 0.5) + a_Position.xy;
}

void SphereMapping()
{
	float r = 0.5;
	float beta = (a_Position.x + 0.5) * 2 * PI;
	float theta = (a_Position.y + 0.5) * 2 * PI;

	vec3 spherePos = a_Position.xyz;
	
	spherePos = vec3(
		r * sin(theta) * cos(beta),
		r * sin(theta) * sin(beta),
		r * cos(theta)
	);

	vec3 originPos = a_Position.xyz;

	float interpol = fract(u_Time / 10);

	vec3 newPos = mix(originPos, spherePos, interpol);

	gl_Position = vec4(newPos.xyz, 1) * u_ProjView;

	v_Grey = 1;
}

void Proj()
{
	gl_Position = vec4(a_Position, 1.0);// * u_ProjView;
	v_Grey = 1.0;
}

void main()
{
	//Flag();
	//Wave();
	//SphereMapping();
	gl_Position = vec4(a_Position.xyz, 1.0);
	//	Proj();
}
