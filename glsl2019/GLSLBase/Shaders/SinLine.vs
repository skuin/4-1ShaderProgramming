#version 450

in vec3 a_Position;
in vec2 a_Temp;
in vec2 a_EmitLife;

uniform float u_Time;

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

		if( remainingLife < 0 ){
			newPos = vec3(100000,100000,100000);
		}

		newPos.x += sin(u_Time * a_Temp.x * 0.1) * a_Temp.x;
		newPos.y += sin(u_Time * a_Temp.x ) * a_Temp.y;
	} 
	else{
		newPos = vec3(100000,100000,100000);
	}

	gl_Position = vec4(newPos, 1);
}
