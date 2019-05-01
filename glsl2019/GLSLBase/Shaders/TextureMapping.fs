#version 450

layout(location=0) out vec4 FragColor;

in vec2 v_Tex;

uniform sampler2D uTexSampler;
uniform float u_Time;

void main()
{
	/*vec4 newColor = vec4(0);

	// ����!
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

	// ���������� �帣�� �׸�
	//vec2 newTex = fract(v_Tex*2 + vec2(u_Time, 0));
	
	
	/* 4/29 ���Ʒ� ��ġ�� */
	
	/*
	// ���Ʒ� ��Į�ڸ���
	vec2 newTex = abs(v_Tex - vec2(0, 0.5)) + vec2(0, 0.5);
	
	vec4 newColor = vec4(newTex, 0, 1);

	newColor = texture(uTexSampler, newTex);
	
	FragColor = newColor;
	*/


	/* 5/1 �� �� ��ǥ ������ ��� �ǽ� */

	
	// simple mean blurr
	// �� ����� ������, ������ ������ ������� ������� ȿ���� �����ֱ� �����.
	// �� ������� ����Ʈ ����Ʈ�� ���� ����� ����.
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


	// uv ��ǥ �ٲٴ� �� �̰����� ����

	/*
	vec2 newTex;
	// fract�� ������ ������ �Ҽ��� ���ϴ°�, floor�� �Ҽ��� ������ �����κ��� ���ϴ°�.
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
	newTex.y += (2-floor(v_Tex.x*3))/3; // 0,1,2 �������� 2-���ϸ� 2,1,0 �̵ȴ�.
	
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
    //���� ����1
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
    //���� ����2
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
