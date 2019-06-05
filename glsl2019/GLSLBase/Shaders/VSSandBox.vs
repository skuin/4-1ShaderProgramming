#version 440

in vec3 a_Position;

const float PI = 3.141592;

uniform float u_Time;

out float v_Grey;
out vec2 v_Tex;
out vec3 v_Norm;
out vec3 v_Pos;

uniform mat4 u_ViewProjMat;

uniform vec2 u_Points[5];

uniform sampler2D u_Texture;
uniform sampler2D u_heightMapTexture;

void flag()
{
	vec3 newPos = a_Position;

	float additionalValueX = newPos.x + 0.5;
	float additionalValueY = newPos.y + 0.5;

	float periodX = 1.0 + (1.0  - additionalValueY) * 0.5;
	float periodY = 1.0 + additionalValueX * 0.5;

	float valueX = (additionalValueY * 2 * PI * periodX) - u_Time *12.0;
	float valueY = (additionalValueX * 2 * PI * periodY) - u_Time * 5.0;

	float sinValueX = sin(valueX) * 0.08;
	float sinValueY = sin(valueY) * 0.2;

	//y scale
	newPos.y = newPos.y * ((1.0 - additionalValueX) * 0.3 + 0.7);

	newPos.x = newPos.x - sinValueX * additionalValueX;
	newPos.y = newPos.y + sinValueY * additionalValueX;

	newPos.y *= 1.0 - (additionalValueX * 0.5);

	//gl_Position = vec4(newPos, 1.0);
	gl_Position = vec4(newPos.xyz, 1.0) * u_ViewProjMat;

	v_Grey = sinValueX + 0.5;

	v_Tex = vec2(0.5, 0.5) + a_Position.xy;

}

void Wave()
{
	vec3 newPos = a_Position.xyz;
	float grey = 0.0;
	float period = 3;

	for(int i = 0; i < 5; i++)
	{
		vec2 target;
		vec2 source = u_Points[i];
		target = a_Position.xy;
		//source = vec2(0.0, 0.0);

		float distance = length(target - source) * 4.0 * PI * period;

		grey += sin(distance - u_Time);
	}
	v_Grey = (grey + 1.0) / 2.0;
	v_Tex = vec2(0.5, 0.5) + a_Position.xy;

	newPos.z += grey * 0.03;

	//gl_Position = vec4(newPos, 1);
	gl_Position = vec4(newPos, 1.0) * u_ViewProjMat;
}

void Sphere()
{
	float newTime = sin(u_Time) * 0.5 + 0.5;

	float r = 0.8;
	float beta = (a_Position.x  + 0.5) * 2 * PI;//0 - PI
	float theta = (a_Position.y + 0.5) * PI;//0 - PI
	vec3 SpherePos;

	SpherePos = vec3
	(
		r * sin(theta) * cos(beta),
		r * sin(theta) * sin(beta),
		r * cos(theta)
	);

	vec3 final = mix(a_Position.xyz, SpherePos, newTime);
	v_Grey = 1.0;
	gl_Position = vec4(final.xyz, 1);
}

void Proj()
{
	gl_Position = vec4(a_Position, 1.0) * u_ViewProjMat;
	v_Grey = 1.0;
}

void HeightMap()
{
	float gap = 2.0/100.0;

	vec2 newUV = a_Position.xy + vec2(0.5, 0.5);
	vec2 newUVRight = a_Position.xy + vec2(0.5, 0.5) + vec2(gap, 0.0);
	vec2 newUVUp = a_Position.xy + vec2(0.5, 0.5) + vec2(0.0, gap);

	// 버텍스 쉐이더에서 텍스쳐 샘플링을해서 하이트맵을 넣는다.
	float height = texture(u_heightMapTexture, newUV).r;
	float heightRight = texture(u_heightMapTexture, newUVRight).r;
	float heightUP = texture(u_heightMapTexture, newUVUp).r;

	vec3 newPos = vec3(a_Position.xy, a_Position.z + height * 0.5);
	vec3 newPosRight = vec3(a_Position.xy + vec2(gap, 0.0), a_Position.z + heightRight * 0.5);
	vec3 newPosUp = vec3(a_Position.xy + vec2(0.0, gap), a_Position.z + heightUP * 0.5);

	vec3 diff1 = newPosRight - newPos;
	vec3 diff2 = newPosUp - newPos;

	vec3 norm = cross(diff1, diff2);

	gl_Position = u_ViewProjMat * vec4(newPos,1.0);
	v_Grey = height;
	v_Tex = newUV;
	v_Norm = normalize(norm);
	v_Pos = newPos;
}
void main()
{
	//Wave();
	//Sphere();
	//Proj();
	//flag();

	HeightMap();
}
