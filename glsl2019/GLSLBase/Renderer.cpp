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
	m_SimpleVelShader = CompileShaders("./Shaders/SimpleVel.vs", "./Shaders/SimpleVel.fs");

	//Create VBOs
	//CreateVertexBufferObjects();
	createRectParticles();
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-0.5, -0.5, 0.f, -0.5, 0.5, 0.f, 0.5, 0.5, 0.f, //Triangle1
		-0.5, -0.5, 0.f,  0.5, 0.5, 0.f, 0.5, -0.5, 0.f, //Triangle2
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

//void Renderer::GenQuadsVBO(int count)
//{
//	float size = 0.01f;
//	int verticePerQuad = 6;
//	int flaotsPerVertex = 3 + 3 + 2;
//	int countQuad = count;
//	int arraySize = countQuad * verticePerQuad * flaotsPerVertex;
//	float *vertices = new float[arraySize];
//
//	for (int i = 0; i < countQuad; i++)
//	{
//		float randX, randY;
//		float randVelX, randVelY, randVelZ;
//		float startTime, lifeTime;
//		float startTimeMax = 6.f;
//		float lifeTimeMax = 3.f;
//
//		int index = i * verticePerQuad * flaotsPerVertex;
//
//		randX = 2.f*(((float)rand() / (float)RAND_MAX) - 0.5f);
//		randY = 2.f*(((float)rand() / (float)RAND_MAX) - 0.5f);
//
//		randVelX = 2.f*(((float)rand() / (float)RAND_MAX) - 0.5f);
//		randVelY = 2.f*(((float)rand() / (float)RAND_MAX) - 0.5f);
//		randVelZ = 0.f;
//
//		startTime = 2.f*(((float)rand() / (float)RAND_MAX) - startTimeMax);
//		startTime = 2.f*(((float)rand() / (float)RAND_MAX) - lifeTimeMax);
//
//	}
//
//}

void Renderer::createRectParticles()
{
	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_real_distribution<> urd(-1.f, 1.f);
	std::uniform_real_distribution<> vurd(-1.f, 1.f);
	std::uniform_real_distribution<> eurd(0.f, 5.f);

	const int particleNum = 1000;
	const double particleSize = 0.005;

	float particles[particleNum][48];

	m_VBOVertexCount = particleNum * 48;

	// 파티클 100개
	for (int i = 0; i < particleNum; ++i){
		double xPos = urd(dre);
		double yPos = urd(dre);

		double xVel = vurd(dre);
		double yVel = vurd(dre);
		double zVel = 0;

		double emitTime = eurd(dre);
		double lifeTime = eurd(dre) + 1;

		// 버텍스 6개
		float particle[] = {
			-particleSize + xPos, -particleSize + yPos, 0.f,
			-particleSize + xVel,-particleSize + yVel, zVel,
			emitTime, lifeTime,
			-particleSize + xPos, particleSize + yPos, 0.f,
			-particleSize + xVel, particleSize + yVel, zVel,
			emitTime, lifeTime,
			particleSize + xPos, particleSize + yPos, 0.f,	//Triangle1
			particleSize + xVel, particleSize + yVel, zVel,
			emitTime, lifeTime,
			-particleSize + xPos, -particleSize + yPos, 0.f,
			-particleSize + xVel, -particleSize + yVel, zVel,
			emitTime, lifeTime,
			particleSize + xPos, particleSize + yPos, 0.f,
			particleSize + xVel, particleSize + yVel, zVel,
			emitTime, lifeTime,
			particleSize + xPos, -particleSize + yPos, 0.f,	//Triangle2
			particleSize + xVel, -particleSize + yVel, zVel,
			emitTime, lifeTime
		};
		memcpy(&particles[i], particle, sizeof(float) * 48);
	}

	glGenBuffers(1, &m_VBOParticles);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticles);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_STATIC_DRAW);
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

void Renderer::Test()
{
	glUseProgram(m_SolidRectShader);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	// 이 데이터를 어떻게 읽을지 정해준다.
	// float형태로 3개씩 끊어서 하나의 vertex를 구성해라
	// sizeof(float) * 3 이거는 다음 데이터를 읽기 위해 얼마만큼의 메모리를 건너뛰어야 하는지 표시한다. stride라고 말한다?
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
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticles);

	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	glDrawArrays(GL_TRIANGLES, 0, 60000);

	glDisableVertexAttribArray(attribPosition);
}

float g_Time = 0;

//void Renderer::Lecture4()
//{	
//	g_Time += 0.0005;
//
//	glUseProgram(m_SimpleVelShader);
//
//	// 유니폼을 넣어줘야 한다.
//	GLuint uTime = glGetUniformLocation(m_SimpleVelShader, "u_Time");
//	glUniform1f(uTime, g_Time);
//
//	glUseProgram(m_SimpleVelShader);
//
//	GLuint aPos = glGetAttribLocation(m_SimpleVelShader, "a_Position");
//	GLuint aVel = glGetAttribLocation(m_SimpleVelShader, "a_Vel");
//
//	// glEnableVertexAttribArray(aPos); 안에 전달인자로 aPos를 넣는거 시험문제!
//	glEnableVertexAttribArray(aPos);
//	glEnableVertexAttribArray(aVel);
//	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticles);
//	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
//	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float)*3));
//
//	glDrawArrays(GL_TRIANGLES, 0, 5000);
//
//	glDisableVertexAttribArray(aPos);
//	glDisableVertexAttribArray(aVel);
//}

void Renderer::Lecture5()
{
	g_Time += 0.0005;

	glUseProgram(m_SimpleVelShader);

	// 유니폼을 넣어줘야 한다.
	GLuint uTime = glGetUniformLocation(m_SimpleVelShader, "u_Time");
	GLuint uRepeat = glGetUniformLocation(m_SimpleVelShader, "u_Repeat");
	glUniform1f(uTime, g_Time);


	glUseProgram(m_SimpleVelShader);

	GLuint aPos = glGetAttribLocation(m_SimpleVelShader, "a_Position");
	GLuint aVel = glGetAttribLocation(m_SimpleVelShader, "a_Vel");
	GLuint aEmitLife = glGetAttribLocation(m_SimpleVelShader, "a_EmitLife");


	// glEnableVertexAttribArray(aPos); 안에 전달인자로 aPos를 넣는거 시험문제!
	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aEmitLife);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticles);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aEmitLife, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 6));


	glDrawArrays(GL_TRIANGLES, 0, m_VBOVertexCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aEmitLife);
}