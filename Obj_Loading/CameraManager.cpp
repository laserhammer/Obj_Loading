#include "CameraManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include <GLEW\GL\glew.h>

CameraBlock CameraManager::_cameraData;
GLuint CameraManager::_cameraBufferLocation;

glm::mat4 CameraManager::_proj;
glm::mat4 CameraManager::_view;
glm::vec4 CameraManager::_camPos;
glm::vec2 CameraManager::_position;

void CameraManager::Init(float aspectRatio, float fov, float near, float far)
{
	_proj = glm::perspectiveFov(fov, aspectRatio, 1.0f / aspectRatio, near, far);
	_position = glm::vec2(0.0f, 0.0f);

	glGenBuffers(1, &_cameraBufferLocation);
	glBindBufferBase(GL_UNIFORM_BUFFER, ResourceManager::CAMERA_BIND_POINT, _cameraBufferLocation);
	_cameraData = CameraBlock(); 
}

void CameraManager::Update(float dt)
{
	float rotRate = 180.0f;
	if (InputManager::cursorLocked())
	{
		glm::vec2 mouseCoords = InputManager::GetMouseCoords();

		_position.x += mouseCoords.x * rotRate;
		_position.x -= _position.x > 360.0f ? 360.0f : 0.0f;

		_position.y += mouseCoords.y * rotRate;
	}

	_position.y = _position.y > 45.0f ? 45.0f : _position.y;
	_position.y = _position.y < -45.0f ? -45.0f : _position.y;

	glm::mat4 bearingMat = glm::mat4_cast(glm::angleAxis(_position.x, glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::mat4 eleMat = glm::mat4_cast(glm::angleAxis(_position.y, glm::vec3(1.0f, 0.0f, 0.0f)));
	glm::mat4 rotMat = eleMat * bearingMat;
	_camPos = glm::vec4(0.0f, 0.0f, -5.0f, 1.0f) * rotMat;

	_view = glm::lookAt(glm::vec3(_camPos), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 viewProjMat = _view * _proj;
	_cameraData.viewProjMat = viewProjMat;
	_cameraData.camPos = _camPos;

	GLsizei vec4size = sizeof(GLfloat) * 4;
	memcpy(ResourceManager::cameraBuffer, glm::value_ptr(_cameraData.viewProjMat[0]), vec4size);
	memcpy(ResourceManager::cameraBuffer + vec4size, glm::value_ptr(_cameraData.viewProjMat[1]), vec4size);
	memcpy(ResourceManager::cameraBuffer + vec4size * 2, glm::value_ptr(_cameraData.viewProjMat[2]), vec4size);
	memcpy(ResourceManager::cameraBuffer + vec4size * 3, glm::value_ptr(_cameraData.viewProjMat[3]), vec4size);

	memcpy(ResourceManager::cameraBuffer + vec4size * 4, glm::value_ptr(_cameraData.camPos), vec4size);

	glBindBuffer(GL_UNIFORM_BUFFER, _cameraBufferLocation);
	glBufferData(GL_UNIFORM_BUFFER, ResourceManager::cameraBufferSize, &ResourceManager::cameraBuffer, GL_DYNAMIC_DRAW);
}

void CameraManager::DumpData()
{
	glDeleteBuffers(1, &_cameraBufferLocation);
}

glm::mat4 CameraManager::ProjMat()
{
	return _proj;
}

glm::mat4 CameraManager::ViewMat()
{
	return _view;
}

glm::vec4 CameraManager::CamPos()
{
	return _camPos;
}