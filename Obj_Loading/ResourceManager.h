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

struct Shader
{
	GLint shaderPointer;

	GLint uPerModelBlockIndex;
	GLint uCameraBlockIndex;
	GLint uLightsBlockIndex;
};

struct Mesh
{
	GLuint vao;
	GLuint vbo;
	GLfloat* vertexBuffer;
	GLint vertexBufferSize;
	GLuint ebo;
	GLint* elementBuffer;
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
	static Mesh cube;
	static Mesh plane;

private:
	static char* ReadTextFile(const char* filepath);
	static GLuint CompileShader(char* shader, GLenum type);
	static GLuint LinkShaderProgram(GLuint* shaders, int numShaders, GLuint fragDataBindColorNumber, char* fragDataBindName);
	static void LoadOBJ(char* obj, Mesh& mesh, Shader* shader);
	static void ParseOBJ(char* obj, std::vector<GLfloat>* vertPos, std::vector<GLfloat>* vertNorm, std::vector<GLint>* elements);
	static void GenMesh(GLfloat* verts, GLint vertsLength, GLint* elements, GLint count, Mesh& mesh, Shader* shader);
	static void GenVertices(std::vector<GLfloat>* verts, std::vector<GLint>* vertElements, std::vector<GLfloat>* vertPos, std::vector<GLfloat>* vertNorms, std::vector<GLint>* elements);
	static void ReleaseMesh(Mesh& mesh);
};