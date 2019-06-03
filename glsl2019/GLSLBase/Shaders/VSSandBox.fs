#version 440

layout(location=0) out vec4 FragColor;

in float v_Grey;
in vec2 v_Tex;
 
uniform sampler2D u_Texture;
uniform sampler2D u_TextureSnow;
uniform sampler2D u_TextureGrass;

void main()
{
	//vec4 newTex = texture(u_Texture, v_Tex);
	//FragColor = texture(u_Texture, newTex);
	//FragColor = vec4(1,1,1,1);

    vec4 snowColor = texture(u_TextureSnow, v_Tex * 5) * (1.0 - v_Grey);
    vec4 grassColor = texture(u_TextureGrass, v_Tex * 5) * v_Grey;
    vec4 finalColor = snowColor + grassColor;

    FragColor = finalColor;
 }
