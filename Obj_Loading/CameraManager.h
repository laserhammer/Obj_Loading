#pragma once
#include <GLEW\GL\glew.h>
#include <GLM\gtc\type_ptr.hpp>
#include <GLM\gtc\matrix_transform.hpp>

struct CameraBlock
{
	glm::mat4 viewProjMat;
	glm::vec4 camPos;
};

class CameraManager
{
public:
	static void Init(float aspectRatio, float fov, float near, float far);
	static void Update(float dt);
	static void DumpData(); 
	static glm::mat4 ViewMat();
	static glm::mat4 ProjMat();
	static glm::vec4 CamPos();
private:
	static glm::mat4 _proj;
	static glm::mat4 _view;
	static glm::vec4 _camPos;

	static glm::vec2 _position;

	static CameraBlock _cameraData;
	static GLuint _cameraBufferLocation;
};