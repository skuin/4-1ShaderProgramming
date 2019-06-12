#version 450

layout(location=0) out vec4 FragColor;

in vec4 v_Color;

uniform vec2 u_Points[5];

const vec2 c_Points[2] = {vec2(0,0), vec2(0.5, 0.5)};

void main()
{
	//float distance = sqrt(v_Color.x*v_Color.x + v_Color.y*v_Color.y);
	//float grey = sin(distance*3.141592*4*10);

	float pointGrey = 0;
	for (int i = 0 ; i<2; i++){
		vec2 newPoint = c_Points[i];
		vec2 newVec = newPoint - v_Color.xy;
		float distance = sqrt(newVec.x*newVec.x + newVec.y*newVec.y);
		if(distance < 0.1){
			pointGrey += 0.5 * (1-(distance/0.1));
		}
	}

	/*vec2 point1 = v_Color.xy + c_Points[0];
	vec2 point2 = v_Color.xy + c_Points[1];
	   
	float distance = sqrt(point1.x*point1.x + point1.y*point1.y);
	float distance2 = sqrt(point2.x*point2.x + point2.y*point2.y);

	if(distance < 0.2 && distance > 0.18 || distance2 < 0.2 && distance2 > 0.18 ){
		FragColor = vec4(1);
	}else{
		FragColor = vec4(0);
	}*/

	//여러개의 동심원 그리기
	//FragColor = vec4(grey);
	
	FragColor = vec4(pointGrey);

	
}
