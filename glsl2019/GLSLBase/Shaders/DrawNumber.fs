#version 450

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Texture;

in vec2 v_Tex;

uniform int u_Number[3]; //3 digits

void main()
{
	vec2 newTex = v_Tex;

	//x_tex coord
	newTex.x = fract(v_Tex.x*3.0);

	//cal index
	int newIndex = int(floor(v_Tex.x*3.0));

	float newy = 9.0 - float(u_Number[newIndex]);
	newTex.y = newTex.y/10.0 + newy/10.0;

	vec4 newColor = texture(u_Texture, newTex);
	FragColor = newColor;
}
