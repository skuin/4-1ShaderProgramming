#version 440

layout(location=0) out vec4 FragColor;
in vec2 v_Tex;

uniform float u_Time;
uniform int u_Number[3];
uniform sampler2D u_Texture;

void main()
{
	vec2 newTex = v_Tex;

	newTex.x = fract(newTex.x * 3.0);

	int newIndex = int(floor(v_Tex.x * 3.0));

	float newy = 9.0 - float(u_Number[newIndex]);
	newTex.y = newTex.y / 10.0 + newy / 10.0;

	vec4 newColor = texture(u_Texture, newTex);
	FragColor = newColor;
}
