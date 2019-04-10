#version 450

in vec3 a_Position;
in vec2 a_Temp;
in vec2 a_EmitLife;
in vec3 a_Vel;
in float a_RandV;

uniform float u_Time;

const float PI = 3.141592;
const mat3 c_RP = mat3(0,-1,0, 1,0,0, 0,0,0);
const vec3 c_Gravity = vec3(0, -0.1, 0);

void main()
{
	vec3 newPos = a_Position.xyz;
	vec3 newVel = a_Vel.xyz;
	vec3 tempVel = vec3(0.1,0.1,0);

	float emitTime = a_EmitLife.x;
	float lifeTime = a_EmitLife.y;
	float ratio = a_Temp.x;
	float amp = a_Temp.y;
	
	float newTime = u_Time - emitTime;

	// 포지션이랑 속도도 받아왔으니 시간에 따라 너의 위치를 입력된 속도를 기준으로해서 옮기고 출력
	if(newTime > 0)
	{	
		newPos.x += sin(a_RandV*PI*2);
		newPos.y += cos(a_RandV*PI*2);

		newTime = mod(newTime, lifeTime);

		newVel = newVel + c_Gravity * newTime;

		newPos = newPos + tempVel * newTime + 0.5*c_Gravity*newTime*newTime;
		vec3 vSin = tempVel * c_RP;
		newPos = newPos + vSin * sin(newTime * PI * 2 * ratio) * amp;
	} 
	else{
		newPos = vec3(100000,100000,100000);
	}

	gl_Position = vec4(newPos, 1);
}
