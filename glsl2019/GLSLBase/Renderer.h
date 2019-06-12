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

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects();
	void CreateTextures();
	void GenQuadsVBO(int count, bool bRandPos, GLuint * id, GLuint * vCount);
	void CreateGridMesh();
	void BindNumberTextures();

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
	GLuint m_VBOTextureRect = 0;

	//Shaders
	GLuint m_SolidRectShader = 0;
	GLuint m_SimpleVelShader = 0;
	GLuint m_SinTrailShader = 0;
	GLuint m_FSSandboxShader = 0;
	GLuint m_FillAllShader = 0;
	GLuint m_TextureRectShader = 0;
	GLuint m_DrawNumberShader = 0;
	GLuint m_SpriteAnimShader = 0;
	GLuint m_VSSandboxShader = 0;
	GLuint m_HDRTextureRectShader = 0;

	//Textures
	GLuint m_ParticleTexture = 0;
	GLuint m_Particle1Texture = 0;
	GLuint m_Particle2Texture = 0;
	GLuint m_CheckerboardTexture = 0;
	GLuint m_PororoTexture = 0;
	GLuint m_RGBTexture = 0;
	GLuint m_NumberTexture[10] = { 0, };
	GLuint m_NumbersTexture = 0;
	GLuint m_SpriteAnimWalkTexture = 0;
	GLuint m_TwiceTexture = 0;
	GLuint m_HeightTexture = 0;
	GLuint m_SnowTexture = 0;
	GLuint m_GrassTexture = 0;
	GLuint m_FBOTexture0 = 0;
	GLuint m_FBOTexture1 = 0;
	GLuint m_FBOTexture2 = 0;
	GLuint m_FBOTexture3 = 0;
	GLuint m_DepthRenderBuffer = 0;

	//Matrices
	glm::mat4 m_ViewMat4;
	glm::mat4 m_OrthoProjMat4;
	glm::mat4 m_PersProjMat4;
	glm::mat4 m_ViewProjMat4;
	glm::vec3 m_CameraPosVec3;
	glm::vec3 m_CameraUpVec3;
	glm::vec3 m_CameraLookatVec3;

	//FrameBufferObjects
	GLuint m_FBO0 = 0;
	GLuint m_FBO1 = 0;
	GLuint m_FBO2 = 0;
	GLuint m_FBO3 = 0;
		
	float g_Time = 0.f;

public:
	void Lecture7();
	void Lecture6();
	void FillAll(float alpha);
	void DrawTextureRect(GLuint tex, float x, float y, float sx, float sy);
	void DrawHDRTextureRect(GLuint tex, float x, float y, float sx, float sy);
	void DrawNumber(int * number);
	void DrawSpriteSequence(GLuint number);
	void VSSandbox();
	void InitMatrices();
	GLuint CreateFBO(int sx, int sy, GLuint *tex, bool isHDR);
	void TestFBO();
	void TestFBO2();
};

