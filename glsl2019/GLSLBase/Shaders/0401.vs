#version 440
 // in�� �� attribute���̾���.
in vec3 a_Position;
in vec3 a_Vel;
in vec4 a_StartLifeRatioAmp;
in float a_RandV;
in vec4 a_Color;

//Varying -> fragment shader input
out vec4 v_Color;
out vec2 v_oriXY;
out float v_Radius;

uniform float u_Time;
uniform bool u_Repeat = true;

const float PI = 3.141592;
const mat3 c_RParticle = mat3(0, -1, 0, 1 , 0, 0, 0, 0, 0);//���������� ���� ���� ��.
const vec3 c_Gravity = vec3(0, -50, 0);//�̰��� uniform���� �ټ� �ִ�.

void main()
{
	vec3 newPos = a_Position.xyz;
	v_oriXY = a_Position.xy;//���� ��ġ
	v_Radius = abs(a_Position.x);

	float startTime = a_StartLifeRatioAmp.x;
	float lifeTime = a_StartLifeRatioAmp.y;
	float ratio = a_StartLifeRatioAmp.z;
	float amp = a_StartLifeRatioAmp.w;
	vec3 newVel = a_Vel.xyz;
	vec4 newColor = a_Color;

	float newTime = u_Time - startTime;	//����� ��쿡�� �׸���.

	if(newTime > 0)
	{
		newPos.x += sin(a_RandV * 2 * PI);
		newPos.y += cos(a_RandV * 2 * PI);
		
		newTime = mod(newTime, lifeTime);

		newColor.w = 1.f - (newTime / lifeTime);//������ �������

		newVel = newVel + c_Gravity * newTime;

		newPos = newPos + a_Vel * newTime + 0.5 * c_Gravity * newTime * newTime; //1/2 at^2

		vec3 vSin = a_Vel * c_RParticle;

		newPos += vSin * sin(newTime * 2 * PI * ratio) * 0.1;
	}
	else
	{
		newPos = vec3(10000, 10000, 10000);
	}
	gl_Position = vec4(newPos, 1);

	
	v_Color = newColor;
}
