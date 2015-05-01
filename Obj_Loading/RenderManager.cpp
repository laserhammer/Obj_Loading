#include "RenderManager.h"
#include "ResourceManager.h"
#include "RenderObject.h"
#include "CameraManager.h"

std::vector<RenderObject*> RenderManager::_displayList;
unsigned int RenderManager::_displayListLength;
unsigned int RenderManager::_numInitializedRenderObjects;

void RenderManager::Init(unsigned int numRenderObjects)
{
	_displayListLength = numRenderObjects;
	_displayList = std::vector<RenderObject*>();
	_displayList.reserve(numRenderObjects);
	_numInitializedRenderObjects = 0;
}

RenderObject* RenderManager::InitRenderObject(Mesh* mesh, Shader* shader, GLenum mode, GLuint layer)
{
	RenderObject* retObj = new RenderObject(mesh, shader, mode, layer);
	_displayList.push_back(retObj);
	return retObj;
}

void RenderManager::Update(float dt)
{
	unsigned int size = _displayList.size();
	for (unsigned int i = 0; i < size; ++i)
	{
		_displayList[i]->Update(dt);
	}
}

void RenderManager::Draw(GLuint mask)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	unsigned int size = _displayList.size();
	for (unsigned int i = 0; i < size; ++i)
	{
		if (mask & _displayList[i]->layer())
			_displayList[i]->Draw();
	}
}
void RenderManager::DumpData()
{
	unsigned int size = _displayList.size();
	for (int i = 0; i < size; ++i)
	{
		delete _displayList[i];
	}
}

	