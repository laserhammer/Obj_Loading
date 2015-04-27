#include "LightingManager.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include <GLM\gtc\type_ptr.hpp>

Light LightingManager::_lights[MAX_LIGHTS];
GLuint LightingManager::_lightsBufferBlockIndex;
GLuint LightingManager::_uLights;
LightsBlock LightingManager::_lightsData;

void LightingManager::Init(GLuint lightsBufferBlockIndex)
{
	_lightsBufferBlockIndex = lightsBufferBlockIndex;
	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		_lights[i] = Light();
		_lights[i].position = glm::vec3();
		_lights[i].linearVelocity = glm::vec3();
		_lights[i].rotation = glm::quat();
		_lights[i].rotationOrigin = glm::vec3();
		_lights[i].angularVelocity = glm::quat();
		_lights[i].transformPos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		_lights[i].color = glm::vec4();
		_lights[i].power = 0.0f;
	}	

	glGenBuffers(1, &_uLights);
	glBindBufferBase(GL_UNIFORM_BUFFER, ResourceManager::LIGHTS_BIND_POINT, _uLights);
}
void LightingManager::Update(float dt)
{
	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		_lights[i].position += _lights[i].linearVelocity * dt;
		_lights[i].rotation = glm::slerp(_lights[i].rotation, _lights[i].rotation * _lights[i].angularVelocity, dt);

		glm::mat4 translateMat = glm::translate(glm::mat4(), _lights[i].position);

		glm::mat4 rotateOriginMat = glm::translate(glm::mat4(), _lights[i].rotationOrigin);
		glm::mat4 rotateMat = glm::inverse(rotateOriginMat) * glm::mat4_cast(_lights[i].rotation) * rotateOriginMat;

		_lights[i].transformPos = translateMat * rotateMat * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		memcpy(&_lightsData.lights[i].position, glm::value_ptr(_lights[i].position), sizeof(GLfloat) * 3);
		_lightsData.lights[i].position[3] = 1.0f;
		memcpy(&_lightsData.lights[i].color_power, glm::value_ptr(_lights[i].color), sizeof(GLfloat) * 3);
		_lightsData.lights[i].color_power[3] = _lights[i].power;
		memcpy(&_lightsData.lights[i].ambient, glm::value_ptr(_lights[i].ambient), sizeof(GLfloat) * 3);
		_lightsData.lights[i].ambient[3] = 1.0f;
	}
	glBindBuffer(GL_UNIFORM_BUFFER, _uLights);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(_lightsData), &_lightsData, GL_DYNAMIC_DRAW);
}
Light* LightingManager::GetLight(int index)
{
	return &_lights[index];
}

	