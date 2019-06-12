#version 450

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Texture;

in vec2 v_Tex;

uniform float u_Time;

uniform float u_BlurSize = 20.0;  // 픽셀단위의 20개
uniform vec2 u_TexelSize = vec2(1.0/1024.0, 1.0/1024.0); // 텍셀 하나의 크기 지금 1024의 해상도니 한개의 사이즈는 1/1024임

void main()
{
    float size = u_BlurSize/2.0; // 10
    vec2 xDir = vec2(1.0, 0.0);
    vec2 yDir = vec2(0.0, 1.0);
    vec3 newColor = vec3(0.0, 0.0, 0.0);
    float count = 0.0;
    float maxDis = length(size*vec2(1.0, 0.0)*u_TexelSize);

    for(float x = -size; x<size; x+=1.0)
    {
        for(float y = -size ; y<size; y+=1.0)
        {
            vec2 newTex = v_Tex + x*xDir*u_TexelSize + y*yDir*u_TexelSize;
            // 블러를 원형으로
            float dis = length(newTex - v_Tex);
            // 가중치를 두어 바깥으로 갈수록 희미하게
            float add = clamp(1.0 - dis/maxDis, 0.0, 1.0);
            add = pow(add, 5);
            vec4 sampleTemp = texture(u_Texture, newTex);
            // 1을 빼줬으니 rgba 1이하는 0이된다.
            sampleTemp = clamp(sampleTemp - vec4(1.0), 0.0, 100.0);
            newColor += sampleTemp.rgb * add;
            count += 0.2;
        }
    }

    count /= 5;
    // 원래 텍스쳐를 더해주면 가운데에 텍스쳐가 생긴다.
    FragColor = vec4(newColor/count + texture(u_Texture, v_Tex).rgb, 1.0);
}
