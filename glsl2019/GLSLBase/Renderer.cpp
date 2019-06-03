#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include <Windows.h>
#include <cstdlib>
#include <cassert>
#include <stdlib.h>
#include <math.h>
#define PI 3.141592
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
	//m_Lecture6Shader = CompileShaders("./Shaders/0401.vs", "./Shaders/0401.fs");
	//m_FragShader = CompileShaders("./Shaders/fragshad.vs", "./Shaders/fragshad.fs");
	//m_FillAllShader = CompileShaders("./Shaders/fillall.vs", "./Shaders/fillall.fs");
	m_TextureRectShader = CompileShaders("./Shaders/TexShader.vs", "./Shaders/TexShader.fs");
	m_DrawNumberShader = CompileShaders("./Shaders/DrawNumberShader.vs", "./Shaders/DrawNumberShader.fs");
	m_AnimShader = CompileShaders("./Shaders/AnimShader.vs", "./Shaders/AnimShader.fs");
	m_VSSandBoxShader = CompileShaders("./Shaders/VSSandBox.vs", "./Shaders/VSSandBox.fs");
	m_OthoShader = CompileShaders("./Shaders/Otho.vs", "./Shaders/Otho.fs");

	m_ParticleTexture = CreatePngTexture("./Textures/particle.png");

	m_RGBTexture = CreatePngTexture("./Textures/rgb.png");
	m_0Texture = CreatePngTexture("./Textures/0.png");
	m_1Texture = CreatePngTexture("./Textures/1.png");
	m_2Texture = CreatePngTexture("./Textures/2.png");
	m_3Texture = CreatePngTexture("./Textures/3.png");
	m_NumberTexture = CreatePngTexture("./Textures/numbers.png");
	m_WalkTexture = CreatePngTexture("./Textures/walk.png");
	m_ThanosTexture = CreatePngTexture("./Textures/thanos.png");
	m_HeightMapTexture = CreatePngTexture("./Textures/heightmap.png");
	m_GrassTexture = CreatePngTexture("./Textures/grass.png");
	m_SnowTexture = CreatePngTexture("./Textures/snowtexture.png");

	
	//Create VBOs
	CreateVertexBufferObjects();

	CreateTextures();
}

void Renderer::CreateVertexBufferObjects()
{
	float size = 0.5f;

	float rect[]
		=
	{
		-0.5, -0.5, 0.f, -0.5, 0.5, 0.f, 0.5, 0.5, 0.f, //Triangle1
		-0.5, -0.5, 0.f,  0.5, 0.5, 0.f, 0.5, -0.5, 0.f, //Triangle2
	};

	float recta[]
		=
	{
		-size, -size, 0.f, 0.5f, -size, size, 0.f, 0.5f,  size, size, 0.f, 0.5f, //Triangle1
		-size, -size, 0.f, 0.5f,  size, size, 0.f, 0.5f,  size, -size, 0.f, 0.5f //Triangle2
	};

	

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(recta), recta, GL_STATIC_DRAW);

	float color[]
		=
	{
		-0.5, -0.5, 0.f, 1,
		-0.5, 0.5, 0.f, 1, 
		0.5, 0.5, 0.f, 1, //rgba
		-0.5, -0.5, 0.f, 1, 
		0.5, 0.5, 0.f, 1,
		0.5, -0.5, 0.f, 1 //Triangle2
	};



	glGenBuffers(1, &m_VBORectColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);


	//lec2
	float tri[] =
	{
		-1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f,//float 9개
	};
	glGenBuffers(1, &m_VBOTri);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTri);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);

	//GenQuadsVBO(300);
	
	//GenRandVelQuadsVBO(100, false, &m_VBOQuads, &m_VBOQuads_VertexCount);
	//GenFragVBO(1, false, &m_VBOQuads, &m_VBOQuads_VertexCount);//사각형 하나 그리기
	//CreateGridMesh();//VSSandBox shader
	InitMatrices();
	
}

void Renderer::CreateTextures()
{
	static const GLulong checkerboard[] =
	{
		0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000,
		0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF,
		0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
		0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000
	};

	glGenTextures(1, &m_checkerboardTexture);
	glBindTexture(GL_TEXTURE_2D, m_checkerboardTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerboard);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp);

	return temp;
}

void Renderer::Test()
{
	glUseProgram(m_FragShader);

	GLuint uTime = glGetUniformLocation(m_FragShader, "u_Time");
	glUniform1f(uTime, g_Time);


	g_Time += 0.01f;
	if (g_Time > 2 * PI)
		g_Time = 0.f;


	GLuint aPos = glGetAttribLocation(m_FragShader, "a_Position");
	GLuint aCol = glGetAttribLocation(m_FragShader, "a_Color");

	//int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(aPos);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(aPos, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	glEnableVertexAttribArray(aCol);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);
	glVertexAttribPointer(aCol, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aCol);
}

void Renderer::Lecture2()
{
	glUseProgram(m_SolidRectShader);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTri);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
}

void Renderer::Lecture3()
{
	glUseProgram(m_SolidRectShader);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOGridMesh);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_LINE_STRIP, 0, m_VBOGridMesh_Count);

	glDisableVertexAttribArray(0);
}

void Renderer::GenQuads()
{
	glUseProgram(m_SolidRectShader);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount);

	glDisableVertexAttribArray(0);
}

void Renderer::GenQuadsVBO(int count)
{
	float randX, randY;
	float size = 0.02f;
	float arraySize = count * 3 * 6;//4 * 6으로 바꾸기
	float *vertices = new float[arraySize];
	for (int i = 0; i < count; i++)
	{
		int index = i * 18;
		randX = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		randY = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);

		vertices[index] = randX - size; ++index;
		vertices[index] = randY - size; ++index;
		vertices[index] = 0.f; ++index;

		vertices[index] = randX + size; ++index;
		vertices[index] = randY + size; ++index;
		vertices[index] = 0.f; ++index;

		vertices[index] = randX - size; ++index;
		vertices[index] = randY + size; ++index;
		vertices[index] = 0.f; ++index;

		vertices[index] = randX - size; ++index;
		vertices[index] = randY - size; ++index;
		vertices[index] = 0.f; ++index;

		vertices[index] = randX + size; ++index;
		vertices[index] = randY + size; ++index;
		vertices[index] = 0.f; ++index;

		vertices[index] = randX + size; ++index;
		vertices[index] = randY - size; ++index;
		vertices[index] = 0.f;
	}
	glGenBuffers(1, &m_VBOQuads);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arraySize, vertices, GL_STATIC_DRAW);
	m_VBOQuads_VertexCount = count * 6;
}

void Renderer::GenRandVelQuadsVBO(int count, bool bRandPos, GLuint *VBOquads, GLuint *vertexCounts)
{
	float size = 0.1f;
	int countQuad = count;
	int verticesPerQuad = 6;
	int floatsPerVertex = 3 + 3 + 2 + 2 + 1 + 4;//pos, vel, startlife, life, ratio, amp, randvalue, color
	int arraySize = countQuad * verticesPerQuad * floatsPerVertex;//버텍스 float 8개 * 버텍스 6개(사각형)
	float *vertices = new float[arraySize];
	for (int i = 0; i < countQuad; i++)
	{
		float randX, randY;
		float randVelX, randVelY, randVelZ;
		float startTime, lifeTime;
		float startTimeMax = 2.f;
		float lifeTimeMax = 1.5f;
		float ratio;
		float amp;
		float value;
		float randValue = 0.f, randThres = 1.f;
		float r, g, b, a;

		int index = i * floatsPerVertex * verticesPerQuad;//18이 아니라 48개(float 8 * 점 6개)

		if (bRandPos)
		{
			randX = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
			randY = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		}
		else
		{
			randX = 0.f;
			randY = 0.f;
		}

		randVelX = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		randVelY = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		randVelZ = 0;

		startTime = ((float)rand() / (float)RAND_MAX) * startTimeMax;
		lifeTime = ((float)rand() / (float)RAND_MAX) * lifeTimeMax;

		ratio = 4.f * (((float)rand() / (float)RAND_MAX) - 1.f);
		amp = 1.f * (((float)rand() / (float)RAND_MAX) - 0.25f);

		randValue = randValue + ((float)rand() / (float)RAND_MAX) * randThres;

		r = 1.f * (((float)rand() / (float)RAND_MAX));
		g = 1.f * (((float)rand() / (float)RAND_MAX));
		b = 1.f * (((float)rand() / (float)RAND_MAX));
		a = 1.f;


		vertices[index] = randX - size; ++index;
		vertices[index] = randY - size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randVelX; ++index;
		vertices[index] = randVelY; ++index;
		vertices[index] = randVelZ; ++index;
		vertices[index] = startTime; ++index;
		vertices[index] = lifeTime; ++index;
		vertices[index] = ratio; ++index;
		vertices[index] = amp; ++index;
		vertices[index] = randValue; ++index;
		vertices[index] = r; ++index;
		vertices[index] = g; ++index;
		vertices[index] = b; ++index;
		vertices[index] = a; ++index;

		vertices[index] = randX + size; ++index;
		vertices[index] = randY + size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randVelX; ++index;
		vertices[index] = randVelY; ++index;
		vertices[index] = randVelZ; ++index;
		vertices[index] = startTime; ++index;
		vertices[index] = lifeTime; ++index;
		vertices[index] = ratio; ++index;
		vertices[index] = amp; ++index;
		vertices[index] = randValue; ++index;
		vertices[index] = r; ++index;
		vertices[index] = g; ++index;
		vertices[index] = b; ++index;
		vertices[index] = a; ++index;

		vertices[index] = randX - size; ++index;
		vertices[index] = randY + size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randVelX; ++index;
		vertices[index] = randVelY; ++index;
		vertices[index] = randVelZ; ++index;
		vertices[index] = startTime; ++index;
		vertices[index] = lifeTime; ++index;
		vertices[index] = ratio; ++index;
		vertices[index] = amp; ++index;
		vertices[index] = randValue; ++index;
		vertices[index] = r; ++index;
		vertices[index] = g; ++index;
		vertices[index] = b; ++index;
		vertices[index] = a; ++index;

		vertices[index] = randX - size; ++index;
		vertices[index] = randY - size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randVelX; ++index;
		vertices[index] = randVelY; ++index;
		vertices[index] = randVelZ; ++index;
		vertices[index] = startTime; ++index;
		vertices[index] = lifeTime; ++index;
		vertices[index] = ratio; ++index;
		vertices[index] = amp; ++index;
		vertices[index] = randValue; ++index;
		vertices[index] = r; ++index;
		vertices[index] = g; ++index;
		vertices[index] = b; ++index;
		vertices[index] = a; ++index;

		vertices[index] = randX + size; ++index;
		vertices[index] = randY - size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randVelX; ++index;
		vertices[index] = randVelY; ++index;
		vertices[index] = randVelZ; ++index;
		vertices[index] = startTime; ++index;
		vertices[index] = lifeTime; ++index;
		vertices[index] = ratio; ++index;
		vertices[index] = amp; ++index;
		vertices[index] = randValue; ++index;
		vertices[index] = r; ++index;
		vertices[index] = g; ++index;
		vertices[index] = b; ++index;
		vertices[index] = a; ++index;

		vertices[index] = randX + size; ++index;
		vertices[index] = randY + size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randVelX; ++index;
		vertices[index] = randVelY; ++index;
		vertices[index] = randVelZ; ++index;
		vertices[index] = startTime; ++index;
		vertices[index] = lifeTime; ++index;
		vertices[index] = ratio; ++index;
		vertices[index] = amp; ++index;
		vertices[index] = randValue; ++index;
		vertices[index] = r; ++index;
		vertices[index] = g; ++index;
		vertices[index] = b; ++index;
		vertices[index] = a; ++index;
	}
	/*
	glGenBuffers(1, &m_VBOQuads);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arraySize, vertices, GL_STATIC_DRAW);
	m_VBOQuads_VertexCount = countQuad * verticesPerQuad;//사각형은 버텍스가 6개.*/

	GLuint vboID = 0;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arraySize, vertices, GL_STATIC_DRAW);
	*vertexCounts = countQuad * verticesPerQuad;
	*VBOquads = vboID;
}

void Renderer::GenFragVBO(int count, bool bRandPos, GLuint *VBOquads, GLuint *vertexCounts)
{
	float size = 1.f;
	int countQuad = count;
	int verticesPerQuad = 6;
	int floatsPerVertex = 3 + 2;//pos, vel, startlife, life, ratio, amp, randvalue, color
	int arraySize = countQuad * verticesPerQuad * floatsPerVertex;//버텍스 float 8개 * 버텍스 6개(사각형)
	float *vertices = new float[arraySize];
	for (int i = 0; i < countQuad; i++)
	{
		float randX, randY;
		float randVelX, randVelY, randVelZ;

		int index = i * floatsPerVertex * verticesPerQuad;//18이 아니라 48개(float 8 * 점 6개)

		if (bRandPos)
		{
			randX = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
			randY = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		}
		else
		{
			randX = 0.f;
			randY = 0.f;
		}

		randVelX = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		randVelY = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		randVelZ = 0;



		vertices[index] = randX - size; ++index;
		vertices[index] = randY - size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = 0.f; ++index;

		vertices[index] = randX + size; ++index;
		vertices[index] = randY + size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = 1.f; ++index;
		vertices[index] = 1.f; ++index;

		vertices[index] = randX - size; ++index;
		vertices[index] = randY + size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = 1.f; ++index;

		vertices[index] = randX - size; ++index;
		vertices[index] = randY - size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = 0.f; ++index;

		vertices[index] = randX + size; ++index;
		vertices[index] = randY - size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = 1.f; ++index;
		vertices[index] = 0.f; ++index;

		vertices[index] = randX + size; ++index;
		vertices[index] = randY + size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = 1.f; ++index;
		vertices[index] = 1.f; ++index;
	}
	/*
	glGenBuffers(1, &m_VBOQuads);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arraySize, vertices, GL_STATIC_DRAW);
	m_VBOQuads_VertexCount = countQuad * verticesPerQuad;//사각형은 버텍스가 6개.*/

	GLuint vboID = 0;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arraySize, vertices, GL_STATIC_DRAW);
	*vertexCounts = countQuad * verticesPerQuad;
	*VBOquads = vboID;
}

void Renderer::CreateGridMesh()
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
	m_VBOGridMesh_Count = (pointCountX - 1)*(pointCountY - 1) * 2 * 3;

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
	glGenBuffers(1, &m_VBOGridMesh);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOGridMesh);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(pointCountX - 1)*(pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);
}


void Renderer::InitMatrices()
{
	m_OrthoProjMat4 = glm::ortho(-1.f, 1.f, -1.f, 1.f, 0.f, 2.f);
	m_PersProjMat4 = glm::perspective(3.141592f*0.5f, 1.f, 0.001f, 100.f);

	m_CameraPosVec3 = glm::vec3(0.f, -1.f, 0.2f);
	m_CameraUpVec3 = glm::vec3(0.f, 0.f, 1.f);
	m_CameraLookatVec3 = glm::vec3(0.f, 0.f, 0.f);
	m_ViewMat4 = glm::lookAt(m_CameraPosVec3, m_CameraLookatVec3, m_CameraUpVec3);

	m_ViewProjMat4 = m_PersProjMat4 * m_ViewMat4;
}

void Renderer::SimpleCube()
{
	float temp = 0.5f;

	float cube[] = {
	-temp,-temp,-temp, -1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, //x, y, z, nx, ny, nz, r, g, b, a
	-temp,-temp, temp, -1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f,
	-temp, temp, temp, -1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f,

	temp, temp,-temp, 0.f, 0.f, -1.f, 0.f, 0.f, 1.f, 1.f,
	-temp,-temp,-temp, 0.f, 0.f, -1.f, 0.f, 0.f, 1.f, 1.f,
	-temp, temp,-temp, 0.f, 0.f, -1.f, 0.f, 0.f, 1.f, 1.f,

	temp,-temp, temp, 0.f, -1.f, 0.f, 0.f, 1.f, 0.f, 1.f,
	-temp,-temp,-temp, 0.f, -1.f, 0.f, 0.f, 1.f, 0.f, 1.f,
	temp,-temp,-temp, 0.f, -1.f, 0.f, 0.f, 1.f, 0.f, 1.f,

	temp, temp,-temp, 0.f, 0.f, -1.f, 0.f, 0.f, 1.f, 1.f,
	temp,-temp,-temp, 0.f, 0.f, -1.f, 0.f, 0.f, 1.f, 1.f,
	-temp,-temp,-temp, 0.f, 0.f, -1.f, 0.f, 0.f, 1.f, 1.f,

	-temp,-temp,-temp, -1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f,
	-temp, temp, temp, -1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f,
	-temp, temp,-temp, -1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f,

	temp,-temp, temp, 0.f, -1.f, 0.f, 0.f, 1.f, 0.f, 1.f,
	-temp,-temp, temp, 0.f, -1.f, 0.f, 0.f, 1.f, 0.f, 1.f,
	-temp,-temp,-temp, 0.f, -1.f, 0.f, 0.f, 1.f, 0.f, 1.f,

	-temp, temp, temp, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f,
	-temp,-temp, temp, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f,
	temp,-temp, temp, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f,

	temp, temp, temp, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f,
	temp,-temp,-temp, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f,
	temp, temp,-temp, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f,

	temp,-temp,-temp, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f,
	temp, temp, temp, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f,
	temp,-temp, temp, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f,

	temp, temp, temp, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f,
	temp, temp,-temp, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f,
	-temp, temp,-temp, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f,

	temp, temp, temp, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f,
	-temp, temp,-temp, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f,
	-temp, temp, temp, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f,

	temp, temp, temp, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f,
	-temp, temp, temp, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f,
	temp,-temp, temp, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f,
	};

	glGenBuffers(1, &m_VBO_Cube);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
}

void Renderer::GenSineParticle()
{

}

void Renderer::Lecture4()
{
	glUseProgram(m_SimplevelShader);

	GLuint uTime = glGetUniformLocation(m_SimplevelShader, "u_Time");//프로그램 ID와 쉐이더프로그램의 uniform변수 를 넘긴다.
	glUniform1f(uTime, g_Time);


	g_Time += 0.8f;
	if (g_Time > 2 * PI)
		g_Time = 0.f;

	std::cout << g_Time << std::endl;

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount);

	glDisableVertexAttribArray(0);
}

void Renderer::RandVel()
{
	glUseProgram(m_SimplevelShader);

	GLuint uTime = glGetUniformLocation(m_SimplevelShader, "u_Time");//프로그램 ID와 쉐이더프로그램의 uniform변수 를 넘긴다.
	GLuint uRepeat = glGetUniformLocation(m_SimplevelShader, "u_Repeat");
	glUniform1f(uTime, g_Time);
	//glUniform1f(uRepeat, u_Repeat);


	g_Time += 0.0001f;
	if (g_Time > 2 * PI)
		g_Time = 0.f;

	GLuint aPos = glGetAttribLocation(m_SimplevelShader, "a_Position");
	GLuint aVel = glGetAttribLocation(m_SimplevelShader, "a_Vel");
	GLuint aStartLife = glGetAttribLocation(m_SimplevelShader, "a_StartLife");


	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aStartLife);



	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);//3개 읽고, 시작지점 0, 8개만큼 건너띰
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aStartLife, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 6));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aStartLife);

}

void Renderer::Lecture6()
{
	GLuint shader = m_Lecture6Shader;
	glUseProgram(shader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint uTime = glGetUniformLocation(shader, "u_Time");//프로그램 ID와 쉐이더프로그램의 uniform변수 를 넘긴다.
	GLuint uRepeat = glGetUniformLocation(shader, "u_Repeat");
	glUniform1f(uTime, g_Time);
	glUniform1f(uRepeat, u_Repeat);


	g_Time += 0.0003f;
	/*if (g_Time > 2 * PI)
		g_Time = 0.f;*/

	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aVel = glGetAttribLocation(shader, "a_Vel");
	GLuint aSLRA = glGetAttribLocation(shader, "a_StartLifeRatioAmp");
	GLuint aRandV = glGetAttribLocation(shader, "a_RandV");
	GLuint aColor = glGetAttribLocation(shader, "a_Color");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aSLRA);
	glEnableVertexAttribArray(aRandV);
	glEnableVertexAttribArray(aColor);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, 0);//3개 읽고, 시작지점 0, 8개만큼 건너띰
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aSLRA,4, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 6));
	glVertexAttribPointer(aRandV, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 10));
	glVertexAttribPointer(aColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 11));


	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aSLRA);
	glDisableVertexAttribArray(aRandV);
	glDisableVertexAttribArray(aColor);

	glDisable(GL_BLEND);
}

void Renderer::Lecture7()
{
	//prepare points
	GLfloat points[] = { 0, 0, 0.5, 0.5, 0.3, 0.3, -0.2, -0.2, -0.3, -0.3 };
	GLuint shader = m_FragShader;
	GLuint uPoints = glGetUniformLocation(shader, "u_Points");
	glUniform2fv(uPoints, 5, points);

	glUseProgram(shader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	GLuint uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, g_Time);
	g_Time += 0.00005f;

	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aUV = glGetAttribLocation(shader, "a_UV");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aUV);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);//3개 읽고, 시작지점 0, 8개만큼 건너띰
	glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aUV);

	glDisable(GL_BLEND);
}

void Renderer::FillAll(float alpha)
{
	//prepare points
	GLfloat points[] = { 0, 0, 0.5, 0.5, 0.3, 0.3, -0.2, -0.2, -0.3, -0.3 };
	GLuint shader = m_FillAllShader;


	GLuint uPoints = glGetUniformLocation(shader, "u_Points");
	glUniform2fv(uPoints, 5, points);

	glUseProgram(shader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, g_Time);
	g_Time += 0.00005f;

	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aUV = glGetAttribLocation(shader, "a_UV");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aUV);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);//3개 읽고, 시작지점 0, 8개만큼 건너띰
	glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aUV);

	glDisable(GL_BLEND);

}

void Renderer::DrawTextureRect(GLuint tex)
{
	//prepare points
	GLuint shader = m_TextureRectShader;
	glUseProgram(shader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aTex = glGetAttribLocation(shader, "a_Tex");

	GLuint uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, g_Time);
	g_Time += 0.0005f;

	GLuint uTex = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(uTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_NumberTexture);
	glActiveTexture(GL_TEXTURE1);


	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aTex);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);//3개 읽고, 시작지점 0, 5개만큼 건너띰
	glVertexAttribPointer(aTex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aTex);

	glDisable(GL_BLEND);

	

}

void Renderer::drawNumber(int * number)
{
	GLuint shader = m_DrawNumberShader;
	glUseProgram(shader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, g_Time);
	g_Time += 0.0005f;

	GLuint uNumber = glGetUniformLocation(shader, "u_Number");
	glUniform1iv(uNumber, 3,number);

	//printf("%f\n", g_Time);

	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aTex = glGetAttribLocation(shader, "a_Tex");
	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aTex);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);//3개 읽고, 시작지점 0, 5개만큼 건너띰
	glVertexAttribPointer(aTex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));
	
	GLuint uTex = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(uTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_WalkTexture);

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aTex);
	glDisable(GL_BLEND);

}

void Renderer::TextureAnim(int number)
{
	GLuint shader = m_AnimShader;
	glUseProgram(shader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, g_Time);
	g_Time += 0.0005f;

	GLuint uNumber = glGetUniformLocation(shader, "u_Number");
	glUniform1f(uNumber,number);
	GLuint uX = glGetUniformLocation(shader, "u_ResolX");
	glUniform1f(uX, 7);
	GLuint uY = glGetUniformLocation(shader, "u_ResolY");
	glUniform1f(uY, 4);

	//printf("%f\n", g_Time);

	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aTex = glGetAttribLocation(shader, "a_Tex");
	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aTex);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);//3개 읽고, 시작지점 0, 5개만큼 건너띰
	glVertexAttribPointer(aTex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	GLuint uTex = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(uTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_WalkTexture);

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aTex);
	glDisable(GL_BLEND);

}


void Renderer::VSSandBox()
{
	CreateGridMesh();
	InitMatrices();

	glUseProgram(m_VSSandBoxShader);

	glEnableVertexAttribArray(0);

	GLuint uTime = glGetUniformLocation(m_VSSandBoxShader, "u_Time");
	glUniform1f(uTime, g_Time);
	g_Time += 0.005f;

	GLuint uViewProjMat = glGetUniformLocation(m_VSSandBoxShader, "u_ViewProjMat");
	glUniformMatrix4fv(uViewProjMat, 1, GL_FALSE, &m_ViewProjMat4[0][0]);

	GLfloat points[] = { 0, 0, 0.1, 0.1, -0.1, 0.3, -0.2, -0.2, 0.3, 0.3 };
	GLuint uPoints = glGetUniformLocation(m_VSSandBoxShader, "u_Points");
	glUniform2fv(uPoints, 5, points);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOGridMesh);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint uTex = glGetUniformLocation(m_VSSandBoxShader, "u_Texture");
	glUniform1i(uTex, 0);

	GLuint uHeightTex = glGetUniformLocation(m_VSSandBoxShader, "u_heightMapTexture");
	glUniform1i(uHeightTex, 1);

	GLuint uSnowTex = glGetUniformLocation(m_VSSandBoxShader, "u_TextureSnow");
	glUniform1i(uSnowTex, 2);

	GLuint uGrassTex = glGetUniformLocation(m_VSSandBoxShader, "u_TextureGrass");
	glUniform1i(uGrassTex, 3);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_HeightMapTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_HeightMapTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_GrassTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_SnowTexture);

	glDrawArrays(GL_TRIANGLES, 0, m_VBOGridMesh_Count);

	glDisableVertexAttribArray(0);
}

void Renderer::Cube()
{
	InitMatrices();
	SimpleCube();

	GLuint shader = m_OthoShader;

	glUseProgram(shader);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	GLuint projView = glGetUniformLocation(shader, "u_ViewProjMat");

	glUniformMatrix4fv(projView, 1, GL_FALSE, &m_ViewProjMat4[0][0]);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	int attribNormal = glGetAttribLocation(shader, "a_Normal");
	int attribColor = glGetAttribLocation(shader, "a_Color");

	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribNormal);
	glEnableVertexAttribArray(attribColor);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Cube);

	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 10, 0);
	glVertexAttribPointer(attribNormal, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(attribColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (GLvoid*)(sizeof(float) * 6));

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribNormal);
	glDisableVertexAttribArray(attribColor);

}

