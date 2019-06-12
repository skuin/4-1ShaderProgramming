#version 450

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Texture;

in vec2 v_Tex;

uniform float u_Number; 
uniform float u_ResolX;
uniform float u_ResolY;

void main()
{
	vec2 newTex;

	float x_index = floor(fract(u_Number / u_ResolX) * u_ResolX);
	float y_index = floor(floor(u_Number / u_ResolX));

	//x_tex coord
	newTex.x = v_Tex.x/u_ResolX + x_index/u_ResolX;
	newTex.y = v_Tex.y/u_ResolY + y_index/u_ResolY;
	
	vec4 newColor = texture(u_Texture, newTex);
	FragColor = newColor;
}
