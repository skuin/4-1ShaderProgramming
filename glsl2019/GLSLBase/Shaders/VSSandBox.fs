#version 450

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Texture;

in float v_Grey;
in vec2 v_Tex;


void main()
{
    // ±ê¹ß ÆÞ·°ÀÌ´Â ½¦ÀÌ´õ
	//FragColor = texture(u_Texture, vec2(v_Tex.x, v_Tex.y)) * v_Grey;

    FragColor = vec4(1) * v_Grey ;
}
