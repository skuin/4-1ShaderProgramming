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
	void CreateSimpleCube();
	void GenQuadsVBO(int count, bool bRandPos, GLuint * id, GLuint * vCount);
	void CreateGridMesh();
	void CreateTexture();
	void InitMatrices();

	unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight);

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_VBORectColor = 0;

	GLuint m_VBO_Cube = 0;

	GLuint m_VBOLecture = 0;
	GLuint m_VBOQuads = 0;
	GLuint m_VBOQuads_VertexCount = 0;
	GLuint m_VBOQuads1 = 0;
	GLuint m_VBOQuads_VertexCount1 = 0;

	GLuint m_VBOTextureRect = 0;


	GLuint m_VBOGridMesh = 0;
	int m_VBOGridMesh_Count = 0;

	//Shaders
	GLuint m_SolidRectShader = 0;
	GLuint m_SimpleVelShader = 0;
	GLuint m_SinTrailShader = 0;
	GLuint m_FSSandboxShader = 0;
	GLuint m_FillAllShader = 0;
	GLuint m_VSSandBoxShader = 0;
	GLuint m_TextureRectShader = 0;
	GLuint m_CubeShader = 0;

	//Textures
	GLuint m_ParticleTexture = 0;
	GLuint m_Particle1Texture = 0;
	GLuint m_Particle2Texture = 0;
	
	GLuint m_CatTexture = 0;
	GLuint m_RGBTexture = 0;

	GLuint m_CheckboardTexture = 0;

	//Matrices
	glm::mat4 m_OrthoProjMat4;
	glm::mat4 m_ViewMat4;
	glm::mat4 m_ViewProjMat4;
	glm::vec3 m_CameraPosVec3;
	glm::vec3 m_CameraUpVec3;
	glm::vec3 m_CameraLookatVec3;
	
public:
	void Lecture7();
	void Lecture6();
	void DrawTextureRect(GLuint tex);
	void VSSandBox();
	void Cube();
};

