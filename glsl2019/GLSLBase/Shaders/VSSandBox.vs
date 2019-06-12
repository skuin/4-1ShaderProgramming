#version 450

layout(location = 0) in vec3 a_Position;
const float PI = 3.14592;

uniform float u_Time;
uniform vec2 u_Points[5];
uniform mat4 u_ViewProjMat;
uniform sampler2D u_heightMapTexture;

out float v_Grey;
out vec2 v_Tex;
out vec3 v_Norm;
out vec3 v_Pos;

void Flag()
{
	vec3 newPos = a_Position;
	
	//0~1
	float additionalValueX = newPos.x + 0.5; //0~1
	float additionalValueY = newPos.y + 0.5; //0~1
	
	float periodX = 1.0 + (1.0 - additionalValueY) * 0.5;
	float periodY = 1.0 + additionalValueX * 0.5;
	
	//x :: -0.5~0.5 --> +0.5 -> 0~1 -> * 2 * PI -> 0~2PI
	float valueX = (additionalValueY * 2 * PI * periodX) - u_Time*12.0;
	float valueY = (additionalValueX * 2 * PI  * periodY) - u_Time*5.0;

	float sinValueX = sin(valueX) * 0.08;
	float sinValueY = sin(valueY) * 0.2;

	//y scale
	newPos.y = newPos.y * ((1.0 - additionalValueX)*0.5 + 0.5);

	//x 
	newPos.x = newPos.x - sinValueX * additionalValueX;
	//y 
	newPos.y = newPos.y + sinValueY * additionalValueX;

	//gl_Position = vec4(newPos.xyz, 1);
	gl_Position = u_ViewProjMat * vec4(newPos.xyz, 1.0);

	v_Grey = sinValueY + 0.5;
	v_Tex = vec2(0.5, 0.5) + a_Position.xy; //0~1, 0~1 tex coordinate
}

void Wave()
{
	float grey = 0.0;
	vec3 newPos = a_Position.xyz;

	for(int i=0; i<5; i++)
	{
		vec2 target;
		vec2 source;
		target = a_Position.xy;
		source = u_Points[i];
		float dis = length(target - source) * 4 * PI * float(i); //0~0.5 --> 0~2PI
		grey += sin(dis - u_Time);
	}

	newPos.z += grey*0.01;

	//gl_Position = vec4(newPos.xyz, 1);
	gl_Position = u_ViewProjMat * vec4(newPos.xyz, 1.0);

	v_Grey = (grey + 1.0)/2.0;
	v_Tex = vec2(0.5, 0.5) + a_Position.xy;
}

void SphereMapping()
{
	float grey = 0.0;
	vec3 newPos = a_Position.xyz;

	for(int i=0; i<2; i++)
	{
		vec2 target;
		vec2 source;
		target = a_Position.xy;
		source = u_Points[i];
		float dis = length(target - source) * 4 * PI * float(2); //0~0.5 --> 0~2PI
		grey += sin(dis - u_Time);
	}

	float r = (grey)*0.1;
	float beta = (a_Position.x + 0.5) * 2 * PI;
	float theta = (a_Position.y + 0.5) * PI;
	vec3 spherePos = a_Position.xyz;
	
	spherePos = vec3(
		r * sin(theta) * cos(beta),
		r * sin(theta) * sin(beta),
		r * cos(theta)
	);
	
	//gl_Position = vec4(spherePos.xyz, 1);
	gl_Position = u_ViewProjMat * vec4(spherePos.xyz, 1.0);

	v_Grey = abs(r);
}

void Proj()
{
	gl_Position = u_ViewProjMat * vec4(a_Position, 1.0);
	v_Grey = 1;
}

void HeightMap()
{
	float gap = 2.0/100.0;

	vec3 newPos = a_Position.xyz;// + vec3(u_Time/20.0, 0.0, 0.0);

	vec2 newUV = newPos.xy + vec2(0.5, 0.5); //0~1
	vec2 newUVRight = newPos.xy + vec2(0.5, 0.5) + vec2(gap, 0.0); //0~1
	vec2 newUVUp = newPos.xy + vec2(0.5, 0.5) + vec2(0.0, gap); //0~1

	float height = texture(u_heightMapTexture, newUV).r;
	float heightRight = texture(u_heightMapTexture, newUVRight).r;
	float heightUp = texture(u_heightMapTexture, newUVUp).r;

	vec3 newPosCenter = vec3(a_Position.xy, a_Position.z + height*0.2);
	vec3 newPosRight = vec3(a_Position.xy + vec2(gap, 0.0), a_Position.z + heightRight*0.2);
	vec3 newPosUp = vec3(a_Position.xy + vec2(0.0, gap), a_Position.z + heightUp*0.2);

	vec3 diff1 = newPosRight - newPosCenter;
	vec3 diff2 = newPosUp - newPosCenter;

	vec3 norm = cross(diff1, diff2);

	gl_Position = u_ViewProjMat * vec4(newPosCenter, 1.0);
	v_Grey = height; 
	v_Tex = newUV;
	v_Norm = normalize(norm);
	v_Pos = newPosCenter;
}

void main()
{
	//Flag();
	//Wave();
	//SphereMapping();
	//Proj();
	HeightMap();
}
