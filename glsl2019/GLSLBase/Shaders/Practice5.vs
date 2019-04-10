#version 450

// attributes
in vec3 a_Position;
in vec3 a_Vel;
in vec2 a_LifeTime;
in vec2 a_RatioAmp;
in float a_Value;
in vec4 a_Color;

// Varying --> fragment shader input
out vec4 v_Color;
out vec2 v_OriXY;
out float v_Radius;

uniform float u_Time;

const float PI = 3.141592;
const mat3 c_RP = mat3(0, -1, 0, 1, 0, 0, 0, 0, 0);
const vec3 c_Gravity = vec3(0, -1, 0);

void main()
{
	vec3 newPos = a_Position.xyz;
	v_OriXY = a_Position.xy;
	v_Radius = abs(a_Position.x); // 절대값
	vec3 newVel = a_Vel.xyz;

	float ratio = a_RatioAmp.x;
	float amp = a_RatioAmp.y;
	float lifeTime = a_LifeTime.y;
	float newTime = u_Time - a_LifeTime.x;

	float newAlpha = 1.f;
	
	float life = lifeTime - newTime;
	//newPos.x = cos(a_Value * 2 * PI) * 0.5f;
	//newPos.y = sin(a_Value * 2 * PI) * 0.5f;

	if(newTime > 0)
	{
		newPos.x += sin(a_Value * 2 * PI) * 0.8f;
		newPos.y += cos(a_Value * 2 * PI) * 0.8f;
		newTime = mod(newTime, lifeTime);
		newVel = newVel + c_Gravity * newTime;
		// 초기속도랑은 다른것 newVel은
		newPos = newPos + a_Vel * newTime + 0.5 * c_Gravity * newTime * newTime;	

		vec3 vSin = a_Vel*c_RP;

		newAlpha = (1 - newTime/lifeTime) * (1 - newTime/lifeTime);
		//newPos = newPos + vSin * sin(newTime * PI * 2 * ratio) * amp;
	}
	
	else{
		newPos = vec3(100000, 100000, 100000);
	}

	gl_Position = vec4(newPos.xyz, 1);

	v_Color.x = a_Color.x;
	v_Color.y = a_Color.y;
	v_Color.z = a_Color.z;
	//v_Color.a = a_Color.a;
	v_Color.a = newAlpha;
}
