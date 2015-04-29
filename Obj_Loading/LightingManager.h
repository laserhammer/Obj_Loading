#pragma once
#include <GLEW\GL\glew.h>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\quaternion.hpp>

static const unsigned int MAX_LIGHTS = 8;
/*
struct Light
{
	glm::vec3 position;
	glm::vec3 linearVelocity;
	glm::quat rotation;
	glm::vec3 rotationOrigin;
	glm::quat angularVelocity;
	glm::vec4 ambient;
	glm::vec4 color;
	glm::vec4 transformPos;
	float power;
};*/

struct Light
{
	glm::vec3 position;
	glm::vec3 linearVelocity;
	glm::quat rotation;
	glm::vec3 rotationOrigin;
	glm::quat angularVelocity;
	glm::vec4 ambient;
	GLint uAmbient;
	glm::vec4 color;
	GLint uColor;
	glm::vec4 transformPos;
	GLint uPosition;
	float power;
	GLint uPower;
	bool active;
};

struct UniformLight
{
	//GLfloat position[4];
	//GLfloat color_power[4];
	//GLfloat ambient[4];
	glm::vec4 position;
	glm::vec4 color_power;
	glm::vec4 ambient;
};

struct LightsBlock
{
	UniformLight lights[MAX_LIGHTS];
};

class LightingManager
{
public:
	static void Init();
	static void Update(float dt);
	static void DumpData();
	static Light* GetLight(int index);
private:
	static Light _lights[MAX_LIGHTS];
	static GLfloat _lightBufferData[MAX_LIGHTS * 8];

	static GLuint _lightsBufferLocation;
	static LightsBlock _lightsData;
};