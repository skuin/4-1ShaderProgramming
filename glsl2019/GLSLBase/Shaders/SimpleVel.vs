#version 440

in vec3 a_Position;
in vec3 a_Vel;
in vec2 a_StartLife;

uniform float u_Time;
uniform bool u_Repeat = true;

const vec3 c_Gravity = vec3(0, -100, 0);


void main()
{
	vec3 newPos = a_Position.xyz;
	float startTime = a_StartLife.x;
	float lifeTime = a_StartLife.y;


	float newTime = u_Time - startTime;	//양수일 경우에만 그린다.




	if(newTime > 0.f)
	{
		float life = newTime;
		float remainginLife = lifeTime - life;

		if(u_Repeat == true)
		{
			remainginLife = 1.f;
			newTime = mod(newTime, lifeTime);
		}


		if(remainginLife < 0)
			newPos = vec3(10000, 10000, 10000);
		else
			newPos += a_Vel * newTime + 0.5 * c_Gravity * newTime * newTime ;
			
	}
	else
	{
		newPos = vec3(10000, 10000, 10000);
	}

	gl_Position = vec4(newPos, 1);
}
