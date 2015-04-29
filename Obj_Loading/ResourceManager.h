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
	GLuint uLightsBlockIndex;
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
	static const GLuint PERMODEL_BIND_POINT;
	static const GLuint LIGHTS_BIND_POINT;
	static const GLuint CAMERA_BIND_POINT;
	static GLubyte* perModelBuffer;
	static GLubyte* cameraBuffer;
	static GLubyte* lightsBuffer;
	static GLint perModelBufferSize;
	static GLint cameraBufferSize;
	static GLint lightsBufferSize;

	static Mesh sphere;

private:
	static char* ReadTextFile(const char* filepath);
	static GLuint CompileShader(char* shader, GLenum type);
	static GLuint LinkShaderProgram(GLuint* shaders, int numShaders, GLuint fragDataBindColorNumber, char* fragDataBindName);
	static void ParseOBJ(char* obj, GLfloat** vertPos, unsigned int& numVertPos, GLfloat** vertNorm, unsigned int& numVertNorms, GLint*** elements, unsigned int& numElements);
	static void GenMesh(GLfloat* verts, GLuint numVerts, GLuint* elements, GLuint numElements, Mesh& mesh);
	static void GenVertices(GLfloat** verts, unsigned int& numVerts, GLuint** vertElements, GLfloat* vertPos, GLfloat* vertNorms, unsigned int numVertNorms, GLint** elements, unsigned int numElements);
	static void ReleaseMesh(Mesh& mesh);
};