#version 440

layout(location=0) out vec4 FragColor;
in vec2 v_Tex;

uniform float u_Time;
uniform sampler2D u_Texture;

void main()
{
	float newTime = floor(u_Time);


	vec2 newTex = v_Tex;

	

	vec4 newColor = texture(u_Texture, newTex);
		
	FragColor = newColor;
}
