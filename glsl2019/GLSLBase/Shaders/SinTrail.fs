#version 450

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Texture;

in vec4 v_Color;
in vec2 v_OriXY;
in float v_Radius;
in vec2 v_TexCoord;

void main()
{
	/*vec4 newColor;
	float dis = sqrt(v_OriXY.x*v_OriXY.x + v_OriXY.y*v_OriXY.y);
	if(dis < v_Radius)
	{
		newColor = v_Color;
		newColor.a = pow(1 - dis / v_Radius, 7);
	}
	else
	{
		newColor = vec4(0, 0, 0, 0);
	}
	FragColor = newColor;*/

	//FragColor = vec4(v_TexCoord, 0, 1);
	//FragColor = texture(u_Texture, v_TexCoord);
    //FragColor.rgb *= 100.0;

    FragColor = vec4(10.0); // 이값이 평균이되서 출력이된다.
	FragColor.a = 1.0;
}
