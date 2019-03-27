#version 450

in vec3 a_Position;
in vec3 a_Vel;
in vec2 a_EmitLife;

uniform float u_Time;
uniform bool u_Repeat = true; // true이면 무한반복, false면 한번..

const vec3 c_Gravity = vec3(0, -0.05, 0);

void main()
{
	vec3 newPos = a_Position.xyz;
	float emitTime = a_EmitLife.x;
	float lifeTime = a_EmitLife.y;

	float newTime = u_Time - emitTime;

	// 포지션이랑 속도도 받아왔으니 시간에 따라 너의 위치를 입력된 속도를 기준으로해서 옮기고 출력
	if(newTime > 0)
	{
		float life = newTime;
		
		float remainingLife = lifeTime - life;
	
		if(u_Repeat == true)
		{
			remainingLife = 1.f;
			newTime = mod(newTime, lifeTime);
		}

		if( remainingLife < 0 ){
			newPos = vec3(100000,100000,100000);
		}
		else{
			newPos += 0.05 * a_Vel * newTime + 0.5 * c_Gravity * newTime*newTime;
		}
	} 
	else{
		newPos = vec3(100000,100000,100000);
	}

	gl_Position = vec4(newPos, 1);
}
