#version 440

layout(location=0) out vec4 FragColor;
in vec2 v_Tex;

uniform float u_Time;
uniform float u_Number;
uniform sampler2D u_Texture;
uniform float u_ResolX;
uniform float u_ResolY;

void main()
{
	vec2 newTex;

	float x_index = int(floor(fract(u_Number / u_ResolX) * u_ResolX));
	float y_index = int(floor(u_Number / u_ResolX));

	newTex.x = v_Tex.x / u_ResolX + x_index / float(u_ResolX);
	newTex.y = v_Tex.y / u_ResolY + y_index / float(u_ResolY);

	vec4 newColor = texture(u_Texture, newTex);
	FragColor = newColor;
}
