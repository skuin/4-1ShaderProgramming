#version 450

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Texture;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform sampler2D u_Texture3;

in vec2 v_Tex;

uniform float u_Time;

void main()
{
	//default
	vec2 newTex = v_Tex;
	vec4 newColor = texture(u_Texture, newTex);
FragColor = newColor;

	//simple mean blurr
	/*vec2 newTex[5];
	vec4 newColor[5];
	float width = 1.0/256.0;
	float height = 1.0/256.0;
	newTex[0] = vec2(v_Tex.x, v_Tex.y);
	newTex[1] = vec2(v_Tex.x-width, v_Tex.y);
	newTex[2] = vec2(v_Tex.x+width, v_Tex.y);
	newTex[3] = vec2(v_Tex.x, v_Tex.y-height);
	newTex[4] = vec2(v_Tex.x, v_Tex.y+height);
	newColor[0] = texture(u_Texture, newTex[0]);
	newColor[1] = texture(u_Texture, newTex[1]);
	newColor[2] = texture(u_Texture, newTex[2]);
	newColor[3] = texture(u_Texture, newTex[3]);
	newColor[4] = texture(u_Texture, newTex[4]);
	vec4 finalColor = newColor[0]+newColor[1]+newColor[2]+newColor[3]+newColor[4];
	finalColor /= 5.0;
	FragColor = finalColor;*/

	//twist tex coord
	/*vec2 newTex;
	newTex.x = fract(v_Tex.x * 3.0);
	newTex.y = v_Tex.y/3.0; //0~1/3
	newTex.y += (2-floor(v_Tex.x*3.0))/3.0; //0, 1, 2 offset + 0~1/3

	vec4 newColor;
	newColor = texture(u_Texture, newTex);
	FragColor = newColor;*/

	/*vec2 newTex;
	newTex.x = v_Tex.x;
	newTex.y = (2-floor(v_Tex.y*3.0))/3; //2/3, 1/3, 0 offset + 0~1/3
	newTex.y += fract(v_Tex.y*3.0)/3;

	vec4 newColor;
	newColor = texture(u_Texture, newTex);
	FragColor = newColor;*/

	//Multiple Textures
	/*vec4 newColor;
	vec2 newTex = vec2(v_Tex.x, 1.0-v_Tex.y);

	if(newTex.x < 0.5 && newTex.y < 0.5)
	{
		newColor = texture(u_Texture2, vec2(newTex.x*2, newTex.y*2));
	}
	else if(newTex.x > 0.5 && newTex.y < 0.5)
	{
		newColor = texture(u_Texture3, vec2(fract(newTex.x*2), newTex.y*2));
	}
	else if(newTex.x < 0.5 && newTex.y > 0.5)
	{
		newColor = texture(u_Texture, vec2(newTex.x*2, fract(newTex.y*2)));
	}
	else if(newTex.x > 0.5 && newTex.y > 0.5)
	{
		newColor = texture(u_Texture1, vec2(fract(newTex.x*2), fract(newTex.y*2)));
	}
	FragColor = vec4(newColor);*/
}
