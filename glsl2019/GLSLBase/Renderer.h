#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <chrono>

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
	void myTest();
	void myTest1();
	void myTest4();
	void lecture4();
	void lecture5();
	void practice5();

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects();
	void CreateVertexBufferObjectsWith4Element();
	void createRectParticles();

	void GenQuadsVBO(int quadCount, bool bRand, GLuint& id, GLuint& vertexCount);

	void Renderer::CreateProxyGeometry();
	unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight);

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_VBORectColor = 0;
	GLuint m_VBOTriangle = 0;
	GLuint m_VBOParticles = 0;
	GLuint m_VBO_GridMesh = 0;
	GLuint m_VBO_GridMesh_Count = 0;
	GLuint m_VBOQuads1 = 0;

	GLuint particleVertexCount = 0;
	GLuint m_VBOQuads_VertexCount1 = 0;

	// Shaders
	GLuint m_SolidRectShader = 0;
	GLuint m_SimpleVal = 0;
	GLuint m_Lecture3 = 0;
	GLuint m_Practice5 = 0;
};

