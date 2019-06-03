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
	void GenQuadsVBO(int count);
	void GenRandVelQuadsVBO(int count, bool bRandPos, GLuint *VBOquads, GLuint *vertexCounts);
	void GenFragVBO(int count, bool bRandPos, GLuint *VBOquads, GLuint *vertexCounts);
	void GenSineParticle();
	void GenQuads();
	void Lecture4();
	void Lecture6();
	void Lecture7();//fragment shader
	void RandVel();
	void TextureAnim(int number);
	void drawNumber(int * number);
	void DrawTextureRect(GLuint tex);
	void VSSandBox();

	void FillAll(float alpha);

	void CreateTextures();
	void SimpleCube();
	void Cube();
	void InitMatrices();

private:
	void CreateGridMesh();

	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects(); 
	unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight);


	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_VBOTri = 0;
	GLuint m_VBOQuads = 0;
	GLuint M_VBOTextureRect = 0;
	GLuint gTextureID = 0;
	GLuint m_VBO_Cube = 0;

	GLuint m_SolidRectShader = 0;
	GLuint m_SimplevelShader = 0;
	GLuint m_Lecture6Shader = 0;
	GLuint m_FragShader = 0;
	GLuint m_FillAllShader = 0;
	GLuint m_TextureRectShader = 0;
	GLuint m_DrawNumberShader = 0;
	GLuint m_AnimShader = 0;
	GLuint m_VSSandBoxShader = 0;
	GLuint m_OthoShader = 0;

	GLuint m_VBORectColor = 0;

	GLuint m_ParticleTexture = 0;
	GLuint m_ParticleTexture1 = 0;

	GLuint m_ParticleTexture2 = 0;

	GLuint m_checkerboardTexture = 0;
	GLuint m_RGBTexture = 0;
	GLuint m_HeightMapTexture = 0;

	GLuint m_0Texture = 0;
	GLuint m_1Texture = 0;
	GLuint m_2Texture = 0;
	GLuint m_3Texture = 0;
	GLuint m_WalkTexture = 0;

	GLuint m_GrassTexture = 0;
	GLuint m_SnowTexture = 0;

	GLuint m_NumberTexture = 0;
	GLuint m_ThanosTexture = 0;


	float g_Time = 0.f;
	bool u_Repeat = 0.f;

	glm::mat4 m_PersProjMat4;
	glm::mat4 m_ViewProjMat4;
	glm::mat4 m_ViewMat4;
	glm::mat4 m_OrthoProjMat4;
	glm::vec3 m_CameraPosVec3;
	glm::vec3 m_CameraLookatVec3;
	glm::vec3 m_CameraUpVec3;


	unsigned int m_VBOQuads_VertexCount;

	GLuint m_VBOGridMesh = 0;
	int m_VBOGridMesh_Count = 0;


};

