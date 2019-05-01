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
	
	
	/* 4/29 위아래 합치기 */
	
	/*
	// 위아래 데칼코마니
	vec2 newTex = abs(v_Tex - vec2(0, 0.5)) + vec2(0, 0.5);
	
	vec4 newColor = vec4(newTex, 0, 1);

	newColor = texture(uTexSampler, newTex);
	
	FragColor = newColor;
	*/


	/* 5/1 블러 및 좌표 가지고 놀기 실습 */

	
	// simple mean blurr
	// 블러 만드는 거지만, 이정도 사이즈 가지고는 눈에띄는 효과를 보여주긴 힘들다.
	// 이 기술들이 라이트 쉬프트와 같은 방식을 쓴다.
	/*
	vec2 newTex[5];
	vec4 newColor[5];
	float widht = 1.0/256.0;
	float height = 1.0/256.0;
	newTex[0] = vec2(v_Tex.x, v_Tex.y);
	newTex[1] = vec2(v_Tex.x-widht, v_Tex.y);
	newTex[2] = vec2(v_Tex.x+widht, v_Tex.y);
	newTex[3] = vec2(v_Tex.x, v_Tex.y-height);
	newTex[4] = vec2(v_Tex.x, v_Tex.y+height);

	newColor[0] = texture(uTexSampler, newTex[0]);
	newColor[1] = texture(uTexSampler, newTex[1]);
	newColor[2] = texture(uTexSampler, newTex[2]);
	newColor[3] = texture(uTexSampler, newTex[3]);
	newColor[4] = texture(uTexSampler, newTex[4]);

	vec4 finalColor = newColor[0] + newColor[1] + newColor[2] + newColor[3] + newColor[4];
	finalColor /= 5;

	FragColor = finalColor;
	*/


	// uv 좌표 바꾸는 것 이거저거 연습

	/*
	vec2 newTex;
	// fract는 정수를 제외한 소수를 구하는것, floor는 소수를 제외한 정수부분을 구하는것.
	newTex.x = fract(v_Tex.x*3);
	newTex.y = v_Tex.y/3.0;
	newTex.y += floor(v_Tex.x*3)/3; // offset 0,1,2
	
	vec4 newColor;
	newColor = texture(uTexSampler, newTex);
	
	FragColor = newColor;
	*/

	/*
	vec2 newTex;
	newTex.x = fract(v_Tex.x*3);
	newTex.y = v_Tex.y/3.0;
	newTex.y += (2-floor(v_Tex.x*3))/3; // 0,1,2 순서에서 2-를하면 2,1,0 이된다.
	
	vec4 newColor;
	newColor = texture(uTexSampler, newTex);
	
	FragColor = newColor;
	*/

	/*
	vec2 newTex;
	newTex.x = v_Tex.x;
	newTex.y = (2-floor(v_Tex.y*3))/3;
	newTex.y += fract(v_Tex.y*3.0)/3;
	
	vec4 newColor;
	newColor = texture(uTexSampler, newTex);
	
	FragColor = newColor;
	*/

    /*
    //벽돌 과제1
	vec2 newTex;
    vec2 newnewTex;

    newTex.x = v_Tex.x*2.0;
    newTex.y = v_Tex.y*2.0;

    newnewTex.x = newTex.x + floor(newTex.y)/2;
    newnewTex.y = newTex.y;

	vec4 newColor;
	newColor = texture(uTexSampler, newnewTex);

	FragColor = newColor;
    */

    /*
    //벽돌 과제2
	vec2 newTex;
    vec2 newnewTex;

    newTex.x = v_Tex.x*2.0;
    newTex.y = v_Tex.y*2.0;

    newnewTex.x = newTex.x;
    newnewTex.y = newTex.y + floor(newTex.x)/2;

	vec4 newColor;
	newColor = texture(uTexSampler, newnewTex);

	FragColor = newColor;
    */

    vec2 newTex;
    
    newTex.x = v_Tex.x;
    newTex.y = abs(1.0-v_Tex.y*2);

    vec4 newColor = texture(uTexSampler, newTex);

    FragColor = newColor;

}
