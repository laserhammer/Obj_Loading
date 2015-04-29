#include "LightingManager.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include <GLM\gtc\type_ptr.hpp>
#include <string>

Light LightingManager::_lights[MAX_LIGHTS];
GLuint LightingManager::_lightsBufferLocation;
LightsBlock LightingManager::_lightsData;

void LightingManager::Init()
{
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

		std::string currentLight = "lights[";
		char numStringBuffer[32];
		itoa(i, numStringBuffer, 10);
		currentLight += numStringBuffer;
		currentLight += "].";
		std::string pos = "position";
		std::string col = "color";
		std::string pow = "power";
		std::string amb = "ambient";
		_lights[i].uPosition = glGetUniformLocation(ResourceManager::phongShader.shaderPointer, (currentLight + pos).c_str());
		_lights[i].uColor = glGetUniformLocation(ResourceManager::phongShader.shaderPointer, (currentLight + col).c_str());
		_lights[i].uPower = glGetUniformLocation(ResourceManager::phongShader.shaderPointer, (currentLight + pow).c_str());
		_lights[i].uAmbient = glGetUniformLocation(ResourceManager::phongShader.shaderPointer, (currentLight + amb).c_str());
	}	

	glGenBuffers(1, &_lightsBufferLocation);
	glBindBufferBase(GL_UNIFORM_BUFFER, ResourceManager::LIGHTS_BIND_POINT, _lightsBufferLocation);
}
void LightingManager::Update(float dt)
{
	GLsizei lightSize = sizeof(GLfloat) * 12;
	GLsizei vec4Size = sizeof(GLfloat) * 4;
	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		_lights[i].position += _lights[i].linearVelocity * dt;
		_lights[i].rotation = glm::slerp(_lights[i].rotation, _lights[i].rotation * _lights[i].angularVelocity, dt);

		glm::mat4 translateMat = glm::translate(glm::mat4(), _lights[i].position);

		glm::mat4 rotateOriginMat = glm::translate(glm::mat4(), _lights[i].rotationOrigin);
		glm::mat4 rotateMat = glm::inverse(rotateOriginMat) * glm::mat4_cast(_lights[i].rotation) * rotateOriginMat;

		_lights[i].transformPos = translateMat * rotateMat * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		_lightsData.lights[i].position = glm::vec4(_lights[i].position, 1.0f);
		_lightsData.lights[i].color_power = glm::vec4(_lights[i].color.swizzle(glm::comp::X, glm::comp::Y, glm::comp::Z), _lights[i].power);
		_lightsData.lights[i].ambient = _lights[i].ambient;

		/*memcpy(ResourceManager::lightsBuffer + lightSize * i, glm::value_ptr(_lightsData.lights[i].position), vec4Size);
		memcpy(ResourceManager::lightsBuffer + lightSize * i + vec4Size, glm::value_ptr(_lightsData.lights[i].color_power), vec4Size);
		memcpy(ResourceManager::lightsBuffer + lightSize * i + vec4Size * 2, glm::value_ptr(_lightsData.lights[i].ambient), vec4Size);*/

		glUniform4fv(_lights[i].uPosition, 1, glm::value_ptr(_lights[i].transformPos));
		glUniform4fv(_lights[i].uColor, 1, glm::value_ptr(_lights[i].color));
		glUniform1f(_lights[i].uPower, _lights[i].power);
		glUniform4fv(_lights[i].uAmbient, 1, glm::value_ptr(_lights[i].ambient));

	}
	glBindBuffer(GL_UNIFORM_BUFFER, _lightsBufferLocation);
	glBufferData(GL_UNIFORM_BUFFER, ResourceManager::lightsBufferSize, &ResourceManager::lightsBuffer, GL_DYNAMIC_DRAW);
}

void LightingManager::DumpData()
{
	glDeleteBuffers(1, &_lightsBufferLocation);
}

Light* LightingManager::GetLight(int index)
{
	return &_lights[index];
}

	