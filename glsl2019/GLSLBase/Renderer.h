#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"
#include "Dependencies\wglew.h"
#include "Dependencies\glm/glm.hpp"
#include "Dependencies\glm/gtc/matrix_transform.hpp"
#include "Dependencies\glm/gtx/euler_angles.hpp"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	GLuint CreatePngTexture(char * filePath);
	GLuint CreateBmpTexture(char * filePath);
	   
	void Test();
	void Lecture2();
	void Lecture3();
	void Lecture4();
	void Lecture5();
	void FillAll(float);

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects(); 
	void GenQuadsVBO(int count, bool bRandPos, GLuint * id, GLuint * vCount);
	void CreateGridMesh();

	unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight);

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_VBORectColor = 0;

	GLuint m_VBOLecture = 0;
	GLuint m_VBOQuads = 0;
	GLuint m_VBOQuads_VertexCount = 0;
	GLuint m_VBOQuads1 = 0;
	GLuint m_VBOQuads_VertexCount1 = 0;

	GLuint m_VBOGridMesh = 0;
	int m_VBOGridMesh_Count = 0;

	//Shaders
	GLuint m_SolidRectShader = 0;
	GLuint m_SimpleVelShader = 0;
	GLuint m_SinTrailShader = 0;
	GLuint m_FSSandboxShader = 0;
	GLuint m_FillAllShader = 0;

	//Textures
	GLuint m_ParticleTexture = 0;
	GLuint m_Particle1Texture = 0;
	GLuint m_Particle2Texture = 0;

public:
	void Lecture7();
	void Lecture6();
};

