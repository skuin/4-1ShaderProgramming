#version 440

layout(location=0) out vec4 FragColor;
in vec2 v_Temp;

const float PI = 3.141592;

uniform float u_Time;
uniform vec2 u_Points[5];


void Rader()
{
    vec2 newUV = v_Temp.xy - vec2(0.5, 0.5); // 0, 0 ~ 1, 1

	float pointGrey = 0;


	float distance = length(newUV);
	float newTime = fract(u_Time);
	float ringWidth = 0.1;

	if(distance < newTime + ringWidth && distance > newTime)
	{
		float temp = (distance - newTime) / ringWidth;
		pointGrey = temp;
		for(int i = 0 ;i < 5; ++i)
		{
			vec2 newPoint = u_Points[i];
			vec2 newVec = newPoint - newUV;
			float distance = sqrt(newVec.x * newVec.x + newVec.y * newVec.y);
			if(distance < 0.1)
			{
				pointGrey += 0.5f * pow((1 - (distance / 0.1)), 5);
			}
		}

	}
	FragColor = vec4(pointGrey);
}

void Waves()
{
    vec4 newColor = vec4(0);
    vec2 newUV = v_Temp.xy - vec2(0.5, 0.5); // 0, 0 ~ 1, 1

    for(int i = 0 ;i < 5; ++i)
    {
		vec2 newPoint = u_Points[i];
		vec2 newVec = newPoint - newUV;
		float distance = length(newVec) * 8 * 3.141592 - u_Time;
        newColor += vec4(sin(distance));
	}
    FragColor = newColor;
}

void main()
{
    //Rader()
    Waves();
	
}
