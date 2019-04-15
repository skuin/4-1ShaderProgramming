#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include <Windows.h>
#include <cstdlib>
#include <cassert>

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_SimpleVal = CompileShaders("./Shaders/SimpleVal.vs", "./Shaders/SimpleVal.fs");
	m_Lecture3 = CompileShaders("./Shaders/lecture3.vs", "./Shaders/lecture3.fs");
	m_Practice5 = CompileShaders("./Shaders/Practice5.vs", "./Shaders/Practice5.fs");
	m_FSSandboxshader = CompileShaders("./Shaders/lecture7.vs", "./Shaders/lecture7.fs");
	//Create VBOs
	CreateVertexBufferObjects();

	//createRectParticles();
	//CreateProxyGeometry();
	//CreateVertexBufferObjectsWith4Element();
	//GenQuadsVBO(1000, false, m_VBOQuads1, m_VBOQuads_VertexCount1);
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-0.5, -0.5, 0.f,0.f,0.f,
		-0.5, 0.5, 0.f,0.f,1.f,
		0.5, 0.5, 0.f, 1.f, 1.f, //Triangle1
		-0.5, -0.5, 0.f, 0.f, 0.f,
		0.5, 0.5, 0.f, 1.f, 1.f,
		0.5, -0.5, 0.f, 1.f, 0.f  //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	// GL_ARRAY_BUFFER형식으로 읽을것이다 라고 말해주는거.
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	// 이 데이터를 gpu memory에 전달해주는거 ?
	// 동적할당 할때는 size를 저렇게 주면 안됨. 포인터의 크기가 전달되기 때문.
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	// 이 단계들이 모두 끝나면 gpu memory상에 올라간것.
	// rect는 이미 gpu memory에 올라갔으므로 건들일이 없고 m_VBORect만 통해서 다루면 됨.

	float triangleVertex[] =
	{
		-1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f
	};

	glGenBuffers(1, &m_VBOTriangle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTriangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertex), triangleVertex, GL_STATIC_DRAW);
}

void Renderer::CreateVertexBufferObjectsWith4Element()
{
	float size = 0.02f;
	float rect[]
		=
	{
		-size, -size, 0.f, 0.5f, 
		-size, size, 0.f, 0.5f, 
		size, size, 0.f, 0.5f, //Triangle1
		-size, -size, 0.f, 0.5f,
		size, size, 0.f, 0.5f,
		size, -size, 0.f, 0.5f //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	// GL_ARRAY_BUFFER형식으로 읽을것이다 라고 말해주는거.
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	// 이 데이터를 gpu memory에 전달해주는거 ?
	// 동적할당 할때는 size를 저렇게 주면 안됨. 포인터의 크기가 전달되기 때문.
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	// 이 단계들이 모두 끝나면 gpu memory상에 올라간것.
	// rect는 이미 gpu memory에 올라갔으므로 건들일이 없고 m_VBORect만 통해서 다루면 됨.

	float color[] =
	{
		1.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f,
		0.f, 0.f, 1.f, 1.f,
		0.5f, 0.f, 0.f, 1.f,
		0.f, 0.5f, 0.f, 1.f,
		0.f, 0.f, 0.5f, 1.f
	};

	glGenBuffers(1, &m_VBORectColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
}

void Renderer::createRectParticles()
{
	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_real_distribution<> urd(-1.f, 1.f);
	std::uniform_real_distribution<> urdTime(0.f, 1.f);

	float particles[1000][48];

	// 파티클 100개
	for (int i = 0; i < 1000; ++i){
		float xPos = urd(dre);
		float yPos = urd(dre);
		float xVel = urd(dre);
		float yVel = urd(dre);
		float zVel = 0.f;

		

		float begin = urdTime(dre);
		float end = urdTime(dre);
		if(begin > end) {
			std::swap(begin, end);
		}
		// 버텍스 6개
		float particle[] = {
			-0.005 + xPos, -0.005 + yPos, 0.f, xVel, yVel, zVel, begin, end,
			-0.005 + xPos, 0.005 + yPos, 0.f, xVel, yVel, zVel, begin, end,
			0.005 + xPos, 0.005 + yPos, 0.f, xVel, yVel, zVel, begin, end,	//Triangle1
			-0.005 + xPos, -0.005 + yPos, 0.f, xVel, yVel, zVel, begin, end,
			0.005 + xPos, 0.005 + yPos, 0.f, xVel, yVel, zVel, begin, end,
			0.005 + xPos, -0.005 + yPos, 0.f, xVel, yVel, zVel, begin, end	//Triangle2
		};
		memcpy(&particles[i], particle, sizeof(float) * 48);
	}
	particleVertexCount = sizeof(particles) / 6;

	glGenBuffers(1, &m_VBOParticles);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticles);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_STATIC_DRAW);
}

void Renderer::GenQuadsVBO(int quadCount, bool bRand, GLuint& id, GLuint& vertexCount)
{
	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_real_distribution<> urd(-1.f, 1.f);
	std::uniform_real_distribution<> urdVel(-0.05f, 0.05f);
	std::uniform_real_distribution<> urdColor(0.f, 1.f);
	
	int verticesPerQuad = 6;
	int elementsInVertex = 15;
	int arraySize = quadCount * verticesPerQuad * elementsInVertex;
	float* vertices = new float[arraySize];

	float posX = 0.f, posY = 0.f, posZ = 0.f;
	float velX = urdVel(dre), velY = urdVel(dre), velZ = urdVel(dre);
	float begin = 0.f, end = 0.f;
	float randX, randY;
	float quadXsize = 0.05f;
	float quadYsize = 0.05f;

	float rColor = urdColor(dre);
	float gColor = urdColor(dre);
	float bColor = urdColor(dre);
	float alpha = 1.0f;

	float startTime, lifeTime;
	float startTimeMax = 6.f;
	float lifeTimeMax = 3.f;

	float ratio, amp;
	float ratioMin = 2.f;
	float ratioThres = 4.f;
	float ampMin = 0.f;
	float ampThres = 0.1f;

	float value = 0.f;

	std::uniform_real_distribution<> urdStartTime(0.f, startTimeMax);
	std::uniform_real_distribution<> urdLifeTime(0.f, lifeTimeMax);
	std::uniform_real_distribution<> urdRatio(ratioMin, ratioThres);
	std::uniform_real_distribution<> urdAmp(ampMin, ampThres);
	std::uniform_real_distribution<> urdValue(0.f, 1.f);

	for (int i = 0; i < quadCount; ++i) {
		if(bRand) {
			randX = urd(dre);
			randY = urd(dre);
		}
		else {
			randX = 0.f;
			randY = 0.f;
		}
		velX = urdVel(dre), velY = urdVel(dre), velZ = urdVel(dre);
		startTime = urdStartTime(dre);
		lifeTime = urdLifeTime(dre);
		ratio = urdRatio(dre);
		amp = urdAmp(dre);
		value = urdValue(dre);
		rColor = urdColor(dre);
		gColor = urdColor(dre);
		bColor = urdColor(dre);

		float particle[] = {
			-quadXsize + posX, -quadYsize + posY, posZ, velX, velY, velZ, startTime, lifeTime, ratio, amp, value, rColor, gColor, bColor, alpha,
			-quadXsize + posX, quadYsize + posY, posZ, velX, velY, velZ, startTime, lifeTime, ratio, amp, value, rColor, gColor, bColor, alpha,
			quadXsize + posX, quadYsize + posY, posZ, velX, velY, velZ, startTime, lifeTime, ratio, amp, value,	rColor, gColor, bColor, alpha,//Triangle1
			-quadXsize + posX, -quadYsize + posY, posZ, velX, velY, velZ, startTime, lifeTime, ratio, amp, value, rColor, gColor, bColor, alpha,
			quadXsize + posX, quadYsize + posY, posZ, velX, velY, velZ, startTime, lifeTime, ratio, amp, value, rColor, gColor, bColor, alpha,
			quadXsize + posX, -quadYsize + posY, posZ, velX, velY, velZ, startTime, lifeTime, ratio, amp, value, rColor, gColor, bColor, alpha	//Triangle2
		};
		memcpy(&vertices[i * verticesPerQuad * elementsInVertex], particle, sizeof(float) * verticesPerQuad * elementsInVertex);
	}

	GLuint vboId = 0;

	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arraySize, vertices, GL_STATIC_DRAW);
	vertexCount = quadCount * verticesPerQuad;
	id = vboId;

	delete[] vertices;
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = (GLint)strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}
unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight)
{
	std::cout << "Loading bmp file " << imagepath << " ... " << std::endl;
	outWidth = -1;
	outHeight = -1;
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = NULL;
	fopen_s(&file, imagepath, "rb");
	if (!file)
	{
		std::cout << "Image could not be opened, " << imagepath << " is missing. " << std::endl;
		return NULL;
	}

	if (fread(header, 1, 54, file) != 54)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1E]) != 0)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1C]) != 24)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	outWidth = *(int*)&(header[0x12]);
	outHeight = *(int*)&(header[0x16]);

	if (imageSize == 0)
		imageSize = outWidth * outHeight * 3;

	if (dataPos == 0)
		dataPos = 54;

	data = new unsigned char[imageSize];

	fread(data, 1, imageSize, file);

	fclose(file);

	std::cout << imagepath << " is succesfully loaded. " << std::endl;

	return data;
}

GLuint Renderer::CreatePngTexture(char * filePath)
{
	//Load Pngs: Load file and decode image.
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0)
	{
		lodepng_error_text(error);
		assert(error == 0);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	return temp;
}

GLuint Renderer::CreateBmpTexture(char * filePath)
{
	//Load Bmp: Load file and decode image.
	unsigned int width, height;
	unsigned char * bmp
		= loadBMPRaw(filePath, width, height);

	if (bmp == NULL)
	{
		std::cout << "Error while loading bmp file : " << filePath << std::endl;
		assert(bmp != NULL);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp);

	return temp;
}

void Renderer::CreateProxyGeometry()
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = 32;
	int pointCountY = 32;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX*pointCountY * 2];
	float* vertices = new float[(pointCountX - 1)*(pointCountY - 1) * 2 * 3 * 3];
	m_VBO_GridMesh_Count = (pointCountX - 1)*(pointCountY - 1) * 2 * 3;

	//Prepare points
	for (int x = 0; x < pointCountX; x++)
	{
		for (int y = 0; y < pointCountY; y++)
		{
			point[(y*pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y*pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}

	//Make triangles
	int vertIndex = 0;
	for (int x = 0; x < pointCountX - 1; x++)
	{
		for (int y = 0; y < pointCountY - 1; y++)
		{
			//Triangle part 1
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			//Triangle part 2
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
		}
	}

	glGenBuffers(1, &m_VBO_GridMesh);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_GridMesh);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(pointCountX - 1)*(pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);
}

void Renderer::Test()
{
	glUseProgram(m_SolidRectShader);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	// 이 데이터를 어떻게 읽을지 정해준다.
	// float형태로 3개씩 끊어서 하나의 vertex를 구성해라
	// sizeofF(float) * 3 이거는 다음 데이터를 읽기 위해 얼마만큼의 메모리를 건너뛰어야 하는지 표시한다. stride라고 말한다?
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	// array의 0번지부터 시작해서 6개의 vertex를 읽어라?
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}

//void Renderer::myTest()
//{
//	glUseProgram(m_SolidRectShader);
//
//	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
//	glEnableVertexAttribArray(attribPosition);
//	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTriangle);
//
//	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
//
//	glDrawArrays(GL_TRIANGLES, 0, 3);
//
//	glDisableVertexAttribArray(attribPosition);
//}

void Renderer::myTest()
{
	glUseProgram(m_SolidRectShader);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_GridMesh);

	glVertexAttribPointer(attribPosition, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	glDrawArrays(GL_LINE_STRIP, 0, 4);

	glDisableVertexAttribArray(attribPosition);
}



//void Renderer::myTest1()
//{
//	static float time = 0.01f;
//	//time += 0.01f;
//	glUseProgram(m_SolidRectShader);
//
//	GLuint uTime = glGetUniformLocation(m_SolidRectShader, "u_Time");
//	glUniform1f(uTime, time);
//
//	glEnableVertexAttribArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticles);
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
//
//	//glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);
//	//glEnableVertexAttribArray(1);
//	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
//
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//
//	glDisableVertexAttribArray(0);
//	//glDisableVertexAttribArray(1);
//
//}

void Renderer::myTest4()
{
	glUseProgram(m_SimpleVal);

	int attribPosition = glGetAttribLocation(m_SimpleVal, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticles);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);

}

void Renderer::lecture4()
{
	static float time = 0.f;
	time += 0.01f;

	glUseProgram(m_Lecture3);

	GLuint uTime = glGetUniformLocation(m_Lecture3, "u_Time");
	glUniform1f(uTime, time);

	GLuint aPos = glGetAttribLocation(m_Lecture3, "a_Position");
	GLuint aVel = glGetAttribLocation(m_Lecture3, "a_Vel");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aVel);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticles);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float)*3));

	glDrawArrays(GL_TRIANGLES, 0, particleVertexCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
}

void Renderer::lecture5()
{
	static float time = 0.f;
	time += 100.f;

	glUseProgram(m_Lecture3);

	GLuint uTime = glGetUniformLocation(m_Lecture3, "u_Time");
	glUniform1f(uTime, time);

	GLuint aPos = glGetAttribLocation(m_Lecture3, "a_Position");
	GLuint aVel = glGetAttribLocation(m_Lecture3, "a_Vel");
	GLuint aLifeTime = glGetAttribLocation(m_Lecture3, "a_lifeTime");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aLifeTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticles);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aLifeTime, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, particleVertexCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aLifeTime);
}

void Renderer::practice5()
{
	static float time = 0.f;
	time += 0.01f;

	glUseProgram(m_Practice5);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint uTime = glGetUniformLocation(m_Practice5, "u_Time");

	glUniform1f(uTime, time);

	GLuint aPos = glGetAttribLocation(m_Practice5, "a_Position");
	GLuint aVel = glGetAttribLocation(m_Practice5, "a_Vel");
	GLuint aLifeTime = glGetAttribLocation(m_Practice5, "a_LifeTime");
	GLuint aRatioAmp = glGetAttribLocation(m_Practice5, "a_RatioAmp");
	GLuint aValue = glGetAttribLocation(m_Practice5, "a_Value");
	GLuint aColor = glGetAttribLocation(m_Practice5, "a_Color");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aLifeTime);
	glEnableVertexAttribArray(aRatioAmp);
	glEnableVertexAttribArray(aValue);
	glEnableVertexAttribArray(aColor);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads1);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aLifeTime, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 6));
	glVertexAttribPointer(aRatioAmp, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 8));
	glVertexAttribPointer(aValue, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 10));
	glVertexAttribPointer(aColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 11));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount1);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aLifeTime);
	glDisableVertexAttribArray(aRatioAmp);
	glDisableVertexAttribArray(aValue);
	glDisableVertexAttribArray(aColor);
}

void Renderer::Lecture7()
{
	GLuint shader = m_FSSandboxshader;
	glUseProgram(shader);

	GLuint attribPosition = glGetAttribLocation(shader, "a_Position");
	GLuint aUV = glGetAttribLocation(shader, "a_UV");

	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(aUV);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(aUV);
}