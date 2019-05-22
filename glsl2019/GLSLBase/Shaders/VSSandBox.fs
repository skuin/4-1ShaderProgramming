#version 450

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Texture;

in float v_Grey;
in vec2 v_Tex;


void main()
{
	FragColor = texture(u_Texture, v_Tex);
}
