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
	m_SinTrailShader = CompileShaders("./Shaders/SinTrail.vs", "./Shaders/SinTrail.fs");
	m_FSSandboxShader = CompileShaders("./Shaders/FSSandbox.vs", "./Shaders/FSSandbox.fs");
	m_FillAllShader = CompileShaders("./Shaders/FillAll.vs", "./Shaders/FillAll.fs");
	m_TextureRectShader = CompileShaders("./Shaders/TextureRect.vs", "./Shaders/TextureRect.fs");
	m_DrawNumberShader = CompileShaders("./Shaders/DrawNumber.vs", "./Shaders/DrawNumber.fs");
	m_SpriteAnimShader = CompileShaders("./Shaders/SpriteAnim.vs", "./Shaders/SpriteAnim.fs");
	m_VSSandboxShader = CompileShaders("./Shaders/VSSandbox.vs", "./Shaders/VSSandbox.fs");
	m_HDRTextureRectShader = CompileShaders("./Shaders/HDRTextureRect.vs", "./Shaders/HDRTextureRect.fs");

	//Load Textures
	m_ParticleTexture = CreatePngTexture("./Textures/particle.png");
	m_Particle1Texture = CreatePngTexture("./Textures/particle1.png");
	m_Particle2Texture = CreatePngTexture("./Textures/particle2.png");
	m_PororoTexture = CreateBmpTexture("./Textures/pororo.bmp");
	m_RGBTexture = CreatePngTexture("./Textures/rgb.png");
	m_NumberTexture[0] = CreatePngTexture("./Textures/0.png");
	m_NumberTexture[1] = CreatePngTexture("./Textures/1.png");
	m_NumberTexture[2] = CreatePngTexture("./Textures/2.png");
	m_NumberTexture[3] = CreatePngTexture("./Textures/3.png");
	m_NumberTexture[4] = CreatePngTexture("./Textures/4.png");
	m_NumberTexture[5] = CreatePngTexture("./Textures/5.png");
	m_NumberTexture[6] = CreatePngTexture("./Textures/6.png");
	m_NumberTexture[7] = CreatePngTexture("./Textures/7.png");
	m_NumberTexture[8] = CreatePngTexture("./Textures/8.png");
	m_NumberTexture[9] = CreatePngTexture("./Textures/9.png");
	m_NumbersTexture = CreatePngTexture("./Textures/Numbers.png");
	m_SpriteAnimWalkTexture = CreatePngTexture("./Textures/sprite1.png");
	m_TwiceTexture = CreatePngTexture("./Textures/twice.png");
	m_HeightTexture = CreatePngTexture("./Textures/height.png");
	m_SnowTexture = CreatePngTexture("./Textures/snow.png");
	m_GrassTexture = CreatePngTexture("./Textures/grass.png");

	//Create VBOs
	CreateVertexBufferObjects();

	//Create Textures
	CreateTextures();

	//Init Matrices
	InitMatrices();

	//Create FBOs
	m_FBO0 = CreateFBO(1024, 1024, &m_FBOTexture0, true);
	m_FBO1 = CreateFBO(512, 512, &m_FBOTexture1, false);
	m_FBO2 = CreateFBO(512, 512, &m_FBOTexture2, false);
	m_FBO3 = CreateFBO(512, 512, &m_FBOTexture3, false);
}

void Renderer::InitMatrices()
{
	//Calc ortho projection matrix
	m_OrthoProjMat4 = glm::ortho(-1.f, 1.f, -1.f, 1.f, 0.f, 2.f);
	m_PersProjMat4 = glm::perspectiveRH(3.141592f * 0.5f, 1.f, 0.001f, 100.f);

	//Calc view matrix
	m_CameraPosVec3 = glm::vec3(0.f, -1.f, 0.2f);
	m_CameraUpVec3 = glm::vec3(0.f, 0.f, 1.f);
	m_CameraLookatVec3 = glm::vec3(0.f, 0.f, 0.f);
	m_ViewMat4 = glm::lookAt(m_CameraPosVec3, m_CameraLookatVec3, m_CameraUpVec3);

	//m_ViewProjMat4 = m_OrthoProjMat4 * m_ViewMat4;
	m_ViewProjMat4 = m_PersProjMat4 * m_ViewMat4;
}

void Renderer::CreateVertexBufferObjects()
{
	float size = 1.f;
	float rect[]
		=
	{
		-size, -size, 0.f, 0.5f, 0.f, 0.f,//x, y, z, value, u, v
		-size, size, 0.f, 0.5f, 0.f, 1.f,
		size, size, 0.f, 0.5f, 1.f, 1.f, //Triangle1
		-size, -size, 0.f, 0.5f, 0.f, 0.f,
		size, size, 0.f, 0.5f, 1.f, 1.f,
		size, -size, 0.f, 0.5f, 1.f, 0.f //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	size = 0.5f;
	float texRect[]
		=
	{
		-size, -size, 0.f, 0.f, 0.f, //x, y, z, u, v
		-size, size, 0.f, 0.f, 1.f,
		size, size, 0.f, 1.f, 1.f, //Triangle1
		-size, -size, 0.f, 0.f, 0.f,
		size, size, 0.f, 1.f, 1.f,
		size, -size, 0.f, 1.f, 0.f //Triangle2
	};

	glGenBuffers(1, &m_VBOTextureRect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTextureRect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texRect), texRect, GL_STATIC_DRAW);

	float color[]
		=
	{
		1, 0, 0, 1, //r, g, b, a
		1, 0, 0, 1,
		1, 0, 0, 1,
		1, 0, 0, 1,
		1, 0, 0, 1,
		1, 0, 0, 1,
	};

	glGenBuffers(1, &m_VBORectColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

	//lecture2
	float triangleVertex[]
		=
	{
		-1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, // 9 floats
	};

	glGenBuffers(1, &m_VBOLecture);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertex), triangleVertex, GL_STATIC_DRAW);

	GenQuadsVBO(1000, true, &m_VBOQuads, &m_VBOQuads_VertexCount);
	GenQuadsVBO(100, false, &m_VBOQuads1, &m_VBOQuads_VertexCount1);
	CreateGridMesh();
}

void Renderer::CreateTextures()
{
	GLulong checkerboard[] =
	{
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF
	};

	glGenTextures(1, &m_CheckerboardTexture);
	glBindTexture(GL_TEXTURE_2D, m_CheckerboardTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerboard);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Renderer::GenQuadsVBO(int count, bool bRandPos, GLuint * id, GLuint * vCount)
{
	float size = 0.01f;
	int countQuad = count;
	int verticesPerQuad = 6;
	int floatsPerVertex = 3 + 3 + 2 + 2 + 1 + 4; //x,y,z, vx,vy,vz, s,l, r,a, randValue, r, g, b, a
	int arraySize = countQuad * verticesPerQuad * floatsPerVertex;
	float *vertices = new float[arraySize];

	for (int i = 0; i < countQuad; i++)
	{
		float randX, randY;
		float randVelX, randVelY, randVelZ;
		float startTime, lifeTime;
		float startTimeMax = 6.f;
		float lifeTimeMax = 3.f;
		float ratio, amp;
		float ratioMin = 2.f;
		float ratioThres = 4.f;
		float ampMin = -0.1f;
		float ampThres = 0.2f;
		float randValue = 0.f, randThres = 1.f;
		float r, g, b, a;

		int index = i * verticesPerQuad * floatsPerVertex;

		if (bRandPos)
		{
			randX = 2.f*(((float)rand() / (float)RAND_MAX) - 0.5f);
			randY = 2.f*(((float)rand() / (float)RAND_MAX) - 0.5f);
		}
		else
		{
			randX = 0.f;
			randY = 0.f;
		}

		randVelX = 0.1f*(((float)rand() / (float)RAND_MAX) - 0.5f);
		randVelY = 0.1f*(((float)rand() / (float)RAND_MAX) - 0.5f);
		randVelZ = 0.f;

		startTime = ((float)rand() / (float)RAND_MAX) * startTimeMax;
		lifeTime = ((float)rand() / (float)RAND_MAX) * lifeTimeMax;

		ratio = ratioMin + ((float)rand() / (float)RAND_MAX) * ratioThres;
		amp = ampMin + ((float)rand() / (float)RAND_MAX) * ampThres;

		randValue = randValue + ((float)rand() / (float)RAND_MAX) * randThres;

		r = ((float)rand() / (float)RAND_MAX);
		g = ((float)rand() / (float)RAND_MAX);
		b = ((float)rand() / (float)RAND_MAX);
		a = 1.f;

		vertices[index] = randX - size; index++;
		vertices[index] = randY - size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = startTime; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = randValue; index++;
		vertices[index] = r; index++;
		vertices[index] = g; index++;
		vertices[index] = b; index++;
		vertices[index] = a; index++;

		vertices[index] = randX - size; index++;
		vertices[index] = randY + size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = startTime; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = randValue; index++;
		vertices[index] = r; index++;
		vertices[index] = g; index++;
		vertices[index] = b; index++;
		vertices[index] = a; index++;

		vertices[index] = randX + size; index++;
		vertices[index] = randY + size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = startTime; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = randValue; index++;
		vertices[index] = r; index++;
		vertices[index] = g; index++;
		vertices[index] = b; index++;
		vertices[index] = a; index++;

		vertices[index] = randX - size; index++;
		vertices[index] = randY - size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = startTime; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = randValue; index++;
		vertices[index] = r; index++;
		vertices[index] = g; index++;
		vertices[index] = b; index++;
		vertices[index] = a; index++;

		vertices[index] = randX + size; index++;
		vertices[index] = randY + size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = startTime; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = randValue; index++;
		vertices[index] = r; index++;
		vertices[index] = g; index++;
		vertices[index] = b; index++;
		vertices[index] = a; index++;

		vertices[index] = randX + size; index++;
		vertices[index] = randY - size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = startTime; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = randValue; index++;
		vertices[index] = r; index++;
		vertices[index] = g; index++;
		vertices[index] = b; index++;
		vertices[index] = a; index++;
	}

	GLuint vboID = 0;

	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*arraySize, vertices, GL_STATIC_DRAW);
	*vCount = countQuad * verticesPerQuad;
	*id = vboID;
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
void Renderer::CreateGridMesh()
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = 100;
	int pointCountY = 100;

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

GLuint Renderer::CreateFBO(int sx, int sy, GLuint *tex, bool isHDR)
{
	//Gen render target
	GLuint tempTex = 0;
	glGenTextures(1, &tempTex);
	glBindTexture(GL_TEXTURE_2D, tempTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	if (isHDR)
	{
		// GL_RGBA32F - 각 채널당 32float point를 가진다 : 성능이 많이 떨어진다. -> 성능이 떨어지거나 안되면 16F로 바꿀것
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, sx, sy, 0, GL_RGBA, GL_FLOAT, 0);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, sx, sy, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	}
		*tex = tempTex;
	//Gen depth buffer
	glGenRenderbuffers(1, &m_DepthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, sx, sy);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	GLuint tempFBO;
	glGenFramebuffers(1, &tempFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, tempFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tempTex, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRenderBuffer);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Error while attach fbo. \n";
		return 0;
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return tempFBO;
}

void Renderer::Test()
{
	glUseProgram(m_SolidRectShader);

	g_Time += 0.01;
	if (g_Time > 1.0f)
		g_Time = 0.f;

	GLuint uTime = glGetUniformLocation(m_SolidRectShader, "u_Time");
	glUniform1f(uTime, g_Time);

	GLuint aPos = glGetAttribLocation(m_SolidRectShader, "a_Position");
	GLuint aCol = glGetAttribLocation(m_SolidRectShader, "a_Color");

	glEnableVertexAttribArray(aPos);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(aPos, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	glEnableVertexAttribArray(aCol);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);
	glVertexAttribPointer(aCol, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Renderer::Lecture2()
{
	glUseProgram(m_SolidRectShader);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount);

	glDisableVertexAttribArray(0);
}

void Renderer::Lecture3()
{
	glUseProgram(m_SolidRectShader);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOGridMesh);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_LINE_STRIP, 0, m_VBOGridMesh_Count);

	glDisableVertexAttribArray(0);
}

void Renderer::Lecture4()
{
	glUseProgram(m_SimpleVelShader);

	GLuint uTime = glGetUniformLocation(m_SimpleVelShader, "u_Time");
	glUniform1f(uTime, g_Time);
	g_Time += 0.01;

	GLuint aPos = glGetAttribLocation(m_SimpleVelShader, "a_Position");
	GLuint aVel = glGetAttribLocation(m_SimpleVelShader, "a_Vel");
	
	//(0x, 1y, 2z, 3vx, 4vy, 5vz, 6s, 7l, 8x, 9y, 10z, 11vx, 12vy, 13vz, 14s, 15l, )

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aVel);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 
		(GLvoid*)(sizeof(float)*3));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount);
	//glDrawArrays(GL_LINE_STRIP, 0, m_VBOQuads_VertexCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
}

void Renderer::Lecture5()
{
	glUseProgram(m_SimpleVelShader);

	GLuint uTime = glGetUniformLocation(m_SimpleVelShader, "u_Time");
	GLuint uRepeat = glGetUniformLocation(m_SimpleVelShader, "u_Repeat");

	glUniform1f(uTime, g_Time);
	g_Time += 0.01;

	GLuint aPos = glGetAttribLocation(m_SimpleVelShader, "a_Position");
	GLuint aVel = glGetAttribLocation(m_SimpleVelShader, "a_Vel");
	GLuint aStartLife = glGetAttribLocation(m_SimpleVelShader, "a_StartLife");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aStartLife);

	//(0x, 1y, 2z, 3vx, 4vy, 5vz, 6s, 7l,  8x, 9y, 10z, 11vx, 12vy, 13vz, 14s, 15l, )

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8,
		(GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aStartLife, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8,
		(GLvoid*)(sizeof(float) * 6));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount);
	//glDrawArrays(GL_LINE_STRIP, 0, m_VBOQuads_VertexCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aStartLife);
}

void Renderer::Lecture6()
{
	// TODO: 여기에 구현 코드 추가.
	GLuint shader = m_SinTrailShader;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(shader);

	GLuint uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, g_Time);
	g_Time += 0.01;

	GLuint uTex = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(uTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Particle1Texture);

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

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads1);

	//(0x, 1y, 2z, 3vx, 4vy, 5vz, 6s, 7l, 8r, 9a, 10v, 11x, 12y, 13z, 14vx
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15,
		(GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aSLRA, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15,
		(GLvoid*)(sizeof(float) * 6));
	glVertexAttribPointer(aRandV, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15,
		(GLvoid*)(sizeof(float) * 10));
	glVertexAttribPointer(aColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15,
		(GLvoid*)(sizeof(float) * 11));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount1);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aSLRA);
	glDisableVertexAttribArray(aRandV);
	glDisableVertexAttribArray(aColor);

	glDisable(GL_BLEND);
}

void Renderer::Lecture7()
{
	//Prepare points
	GLfloat points[] = { 0, 0, 0.5, 0.5, 0.3, 0.3, -0.2, -0.2, -0.3, -0.3 };

	// TODO: 여기에 구현 코드 추가.
	GLuint shader = m_FSSandboxShader;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint uPoints = glGetUniformLocation(shader, "u_Points");
	glUniform2fv(uPoints, 5, points);

	GLuint uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, g_Time);
	g_Time += 0.005;

	glUseProgram(shader);

	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aUV = glGetAttribLocation(shader, "a_UV");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aUV);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6,
		(GLvoid*)(sizeof(float) * 4));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aUV);
}


void Renderer::FillAll(float alpha)
{
	// TODO: 여기에 구현 코드 추가.
	GLuint shader = m_FillAllShader;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(shader);

	GLuint aPos = glGetAttribLocation(shader, "a_Position");

	glEnableVertexAttribArray(aPos);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(aPos);

	glDisable(GL_BLEND);
}

void Renderer::BindNumberTextures()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_NumberTexture[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_NumberTexture[1]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_NumberTexture[2]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_NumberTexture[3]);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_NumberTexture[4]);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_NumberTexture[5]);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_NumberTexture[6]);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, m_NumberTexture[7]);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, m_NumberTexture[8]);
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, m_NumberTexture[9]);
}

void Renderer::DrawTextureRect(GLuint tex, float x, float y, float sx, float sy)
{
	GLuint shader = m_TextureRectShader;

	glUseProgram(shader);

	GLuint uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, g_Time);
	g_Time += 0.01;
	GLuint uTexture = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(uTexture, 0);
	GLuint uPos = glGetUniformLocation(shader, "u_Pos");
	GLuint uSize = glGetUniformLocation(shader, "u_Size");
	glUniform2f(uPos, x, y);
	glUniform2f(uSize, sx, sy);

	//BindNumberTextures();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aTex = glGetAttribLocation(shader, "a_Tex");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aTex);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTextureRect);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(aTex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aTex);
}

void Renderer::DrawHDRTextureRect(GLuint tex, float x, float y, float sx, float sy)
{
	GLuint shader = m_HDRTextureRectShader;

	static float kernelSize = 1.0;
	kernelSize += 0.05;
	if (kernelSize > 60.f)
		kernelSize = 0.f;

	glUseProgram(shader);

	GLuint uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, g_Time);
	g_Time += 0.01;
	GLuint uTexture = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(uTexture, 0);
	GLuint uPos = glGetUniformLocation(shader, "u_Pos");
	GLuint uSize = glGetUniformLocation(shader, "u_Size");
	glUniform2f(uPos, x, y);
	glUniform2f(uSize, sx, sy);
	
	// 블러사이즈와 텍셀사이즈
	// 평균 블러를 쓰면 블러 사이즈가 커질수록 흐릿해지기 때문에 잘안쓴다... 하지만 일단 해본것
	GLuint uBlurSize = glGetUniformLocation(shader, "u_BlurSize");
	glUniform1f(uBlurSize, kernelSize);
	GLuint uTexelSize = glGetUniformLocation(shader, "u_TexelSize");
	glUniform2f(uBlurSize, 1.f / 1024.f, 1.f / 1024.f);

	//BindNumberTextures();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aTex = glGetAttribLocation(shader, "a_Tex");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aTex);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTextureRect);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(aTex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aTex);
}

void Renderer::DrawNumber(int * number)
{
	GLuint shader = m_DrawNumberShader;

	glUseProgram(shader);
	
	//Uniform inputs
	GLuint uNumber = glGetUniformLocation(shader, "u_Number");
	glUniform1iv(uNumber, 3, number);

	//Vertex settings
	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aTex = glGetAttribLocation(shader, "a_Tex");
	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aTex);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTextureRect);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float)*5, 0);
	glVertexAttribPointer(aTex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	//Texture settings
	GLuint uTex = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(uTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_NumbersTexture);

	//Draw here
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Restore to default
	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aTex);
}

void Renderer::DrawSpriteSequence(GLuint number)
{
	GLuint shader = m_SpriteAnimShader;

	glUseProgram(shader);

	//Uniform inputs
	GLuint uNumber = glGetUniformLocation(shader, "u_Number");
	glUniform1f(uNumber, (float)number);
	GLuint uX = glGetUniformLocation(shader, "u_ResolX");
	glUniform1f(uX, (float)6);
	GLuint uY = glGetUniformLocation(shader, "u_ResolY");
	glUniform1f(uY, (float)1);

	//Vertex settings
	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aTex = glGetAttribLocation(shader, "a_Tex");
	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aTex);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTextureRect);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(aTex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	//Texture settings
	GLuint uTex = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(uTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_SpriteAnimWalkTexture);

	//Draw here
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Restore to default
	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aTex);
}

void Renderer::VSSandbox()
{
	//Prepare points
	GLfloat points[] = { 0, 0, 0.5, 0.5, 0.3, 0.3, -0.2, -0.2, -0.3, -0.3 };
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	GLuint shader = m_VSSandboxShader;
	glUseProgram(shader);

	static float gTime = 0; gTime += 0.05f;

	GLuint uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, gTime);
	GLuint uTex = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(uTex, 0);
	GLuint uPoints = glGetUniformLocation(shader, "u_Points");
	glUniform2fv(uPoints, 5, points);
	GLuint uHeight = glGetUniformLocation(shader, "u_heightMapTexture");
	glUniform1i(uHeight, 1);
	GLuint uSnow = glGetUniformLocation(shader, "u_TextureSnow");
	glUniform1i(uSnow, 2);
	GLuint uGrass = glGetUniformLocation(shader, "u_TextureGrass");
	glUniform1i(uGrass, 3);

	GLuint uViewProjMat = glGetUniformLocation(shader, "u_ViewProjMat");
	glUniformMatrix4fv(uViewProjMat, 1, GL_FALSE, &m_ViewProjMat4[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TwiceTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_HeightTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_SnowTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_GrassTexture);
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOGridMesh);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, m_VBOGridMesh_Count);

	glDisableVertexAttribArray(0);
}

void Renderer::TestFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0f);
	glViewport(0, 0, 512, 512);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	VSSandbox();

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO1);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0f);
	glViewport(0, 0, 512, 512);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Lecture6(); //ok

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO2);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0f);
	glViewport(0, 0, 512, 512);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Lecture7(); //ok

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO3);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0f);
	glViewport(0, 0, 512, 512);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	static int gAnimNum = 0;
	gAnimNum++;
	if (gAnimNum > 6)
		gAnimNum = 0;
	DrawSpriteSequence(gAnimNum); //ok

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glViewport(0, 0, 1024, 1024);
	DrawTextureRect(m_FBOTexture0, -0.5, -0.5, 1, 1);
	DrawTextureRect(m_FBOTexture1,  0.5, -0.5, 1, 1);
	DrawTextureRect(m_FBOTexture2, -0.5,  0.5, 1, 1);
	DrawTextureRect(m_FBOTexture3,  0.5,  0.5, 1, 1);
}

// Blur 적용 - 1024x1024 꽉찬 sinTrail 생성
void Renderer::TestFBO2()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0f);
	glViewport(0, 0, 1024, 1024);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	VSSandbox();
	Lecture6(); //ok

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glViewport(0, 0, 1024, 1024);
	DrawHDRTextureRect(m_FBOTexture0, 0, 0, 2, 2);
}


