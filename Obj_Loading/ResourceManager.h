#pragma once
#include "GLEW\GL\glew.h"
#include <GLM\gtc\matrix_transform.hpp>

struct Vertex
{
	float posX;
	float posY;
	float posZ;
	float texCoordU;
	float texCoordV;
	float normX;
	float normY;
	float normZ;
};

struct Shader
{
	GLuint program;
	GLuint uCameraBlockIndex;
	GLuint uPerModelBlockIndex;
};

struct CameraBlock
{
	float viewProjMat[4][4];
	float camPos[4];
};

struct Mesh
{
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint count;
};

class ResourceManager
{
public:
	static void Init();
	static void DumpData();

	static Shader phongShader;
	static GLuint phongFragShader;
	static GLuint phongVertShader;
	static Mesh sphere;

private:
	static char* ReadTextFile(const char* filepath);
	static GLuint CompileShader(char* shader, GLenum type);
	static GLuint LinkShaderProgram(GLuint* shaders, int numShaders, GLuint fragDataBindColorNumber, char* fragDataBindName);
	static void ParseOBJ(char* obj, GLfloat** vertPos, unsigned int& numVertPos, GLfloat** vertNorm, unsigned int& numVertNorms, GLint*** elements, unsigned int& numElements);
	static Mesh GenMesh(Vertex* verts, GLuint* elements, GLuint numElements);
	static void ReleaseMesh(Mesh& mesh);
};