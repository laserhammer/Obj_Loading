#pragma once
#include "GLEW\GL\glew.h"
#include <GLM\gtc\matrix_transform.hpp>
#include <vector>

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
/*
struct Shader
{
	GLint program;
	GLint uCameraBlockIndex;
	GLint uPerModelBlockIndex;
	GLint uLightsBlockIndex;
};*/

struct Shader
{
	GLint shaderPointer;
	GLint uModelMat;
	GLint uViewMat;
	GLint uProjMat;
	GLint uColor;
	GLint uCamPos;
};

struct Mesh
{
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLint count;
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
	static void ParseOBJ(char* obj, std::vector<GLfloat>* vertPos, std::vector<GLfloat>* vertNorm, std::vector<GLint>* elements);
	static void GenMesh(std::vector<GLfloat>* verts, std::vector<GLint>* elements, Mesh& mesh);
	static void GenVertices(std::vector<GLfloat>* verts, std::vector<GLint>* vertElements, std::vector<GLfloat>* vertPos, std::vector<GLfloat>* vertNorms, std::vector<GLint>* elements);
	static void ReleaseMesh(Mesh& mesh);
};