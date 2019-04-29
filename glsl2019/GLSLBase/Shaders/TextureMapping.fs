#version 450

layout(location=0) out vec4 FragColor;

in vec2 v_Tex;

uniform sampler2D uTexSampler;
uniform float u_Time;

void main()
{
	/*vec4 newColor = vec4(0);

	// 문제!
	float x = v_Tex.x * 3.141592 * 2 + u_Time;
	float value = ((sin(x)+1)*0.5);
	if(value >= v_Tex.y && value - 0.01 <= v_Tex.y)
	{
		newColor = vec4(1);
	}
	else
	{
		newColor = vec4(0);
	}
	FragColor = newColor;
	*/

	// 오른쪽으로 흐르는 그림
	//vec2 newTex = fract(v_Tex*2 + vec2(u_Time, 0));
	
	// 위아래 데칼코마니
	vec2 newTex = abs(v_Tex - vec2(0, 0.5)) + vec2(0, 0.5);
	
	vec4 newColor = vec4(newTex, 0, 1);

	newColor = texture(uTexSampler, newTex);
	
	FragColor = newColor;
}
