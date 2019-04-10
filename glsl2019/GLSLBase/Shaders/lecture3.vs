#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Vel;
layout(location = 2) in vec2 a_lifeTime;

uniform float u_Time;

const vec3 c_Gravity = vec3(0, -5, 0);

void main()
{
	vec3 newPos = a_Position.xyz;
	vec3 newVel = a_Vel;

	float begin = a_lifeTime.x;
	float end = a_lifeTime.y;

	float newTime = u_Time;
	int times = 0;
	if(newTime > begin){
		int times = int(newTime) / int(end);
	}

	newTime = newTime - (end) * times;

	if(newTime < begin)
	{
		newPos = vec3(999999.f, 999999.f, 999999.f);
	}
	


	//newPos += a_Vel * newTime + 0.5 * c_Gravity * newTime * newTime;

	newVel += c_Gravity * newTime;
	newPos += newVel * newTime;

	gl_Position = vec4(newPos, 1.0);
}
