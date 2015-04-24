#include "RenderManager.h"
#include "ResourceManager.h"
#include "RenderObject.h"
#include "CameraManager.h"

RenderObject* RenderManager::_displayList;
unsigned int RenderManager::_displayListLength;
unsigned int RenderManager::_numInitializedRenderObjects;
const unsigned int RenderManager::UBUFFER_BIND_POINT = 0;

void RenderManager::Init(unsigned int numRenderObjects = 0)
{
	_displayListLength = numRenderObjects;
	_displayList = new RenderObject[_displayListLength];
	_numInitializedRenderObjects = 0;
}

RenderObject* RenderManager::InitRenderObject(Mesh* mesh, Shader* shader, GLenum mode = GL_TRIANGLES, GLuint layer)
{
	if (_numInitializedRenderObjects + 1 > _displayListLength)
	{
		RenderObject* temp = _displayList;
		_displayList = new RenderObject[_displayListLength + 1];
		memcpy(_displayList, temp, sizeof(RenderObject) * _displayListLength);
		delete[] temp;
		++_displayListLength;
	}

	RenderObject* retObj = &(_displayList[_numInitializedRenderObjects] = RenderObject(mesh, shader, mode, layer, UBUFFER_BIND_POINT));
	return retObj;
}

void RenderManager::Update(float dt)
{
	for (int i = 0; i < _numInitializedRenderObjects; ++i)
	{
		_displayList[i].Update(dt);
	}
}

void RenderManager::Draw(GLuint mask)
{
	for (int i = 0; i < _numInitializedRenderObjects; ++i)
	{
		if (mask & _displayList[i].layer())
			_displayList[i].Draw();
	}
}
void RenderManager::DumpData()
{
	delete[] _displayList;
}

	