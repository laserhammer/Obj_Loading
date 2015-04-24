#pragma once
#include <GLEW\GL\glew.h>

class RenderObject;
struct Mesh;
struct Shader;

class RenderManager
{
public:
	static void Init(unsigned int numRenderObjects = 0);
	static RenderObject* InitRenderObject(Mesh* mesh, Shader* shader, GLenum mode = GL_TRIANGLES, GLuint layer = 1);
	static void Update(float dt);

	static void Draw(GLuint mask);
	static void DumpData();
private:
	static RenderObject* _displayList;
	static unsigned int _displayListLength;
	static unsigned int _numInitializedRenderObjects;
	static const unsigned int UBUFFER_BIND_POINT;
};