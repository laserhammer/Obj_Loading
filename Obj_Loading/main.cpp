#include "GLEW/GL/glew.h"
#include "GLFW/glfw3.h"
#include <ctime>
#include <random>
#include <crtdbg.h>

#include "Init_Shader.h"
#include "CameraManager.h"
#include "LightingManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "RenderObject.h"

GLFWwindow* window;

RenderObject* sphere;
RenderObject* sphere2;

Light* light;

void InitLights()
{
	light = LightingManager::GetLight(0);
	light->position.y = 3.0f;
	light->color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	light->ambient = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	light->power = 0.2f;
}

void init()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	if (!glfwInit()) exit(EXIT_FAILURE);

	//Create window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(800, 600, "Obj_Loading", NULL, NULL); // Windowed

	//Activate window
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	glewInit();

	ResourceManager::Init();
	RenderManager::Init(1);
	LightingManager::Init();

	glfwSetTime(0.0);

	time_t timer;
	time(&timer);
	srand((unsigned int)timer);

	InitLights();

	sphere = RenderManager::InitRenderObject(&ResourceManager::sphere, &ResourceManager::phongShader, GL_TRIANGLES, 1);
	sphere2 = RenderManager::InitRenderObject(&ResourceManager::sphere, &ResourceManager::phongShader, GL_TRIANGLES, 1);
	sphere2->transform().position.y = 3.0f;

	//generateTeapot();

	InputManager::Init(window);
	CameraManager::Init(800.0f / 600.0f, 80.0f, 0.1f, 100.0f);

	glEnable(GL_CULL_FACE);
}

void step()
{
	// Clear to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	InputManager::Update();

	// Get delta time since the last frame
	float dt = (float)glfwGetTime();
	glfwSetTime(0.0);

	//float dTheta = 45.0f * InputManager::rightKey();
	//dTheta -= 45.0f * InputManager::leftKey();

	//teapot->transform().angularVelocity = glm::angleAxis(dTheta, glm::vec3(0.0f, 1.0f, 0.0f));

	CameraManager::Update(dt);

	RenderManager::Update(dt);

	LightingManager::Update(dt);

	//teapot->Update(dt);

	RenderManager::Draw(1);

	// Swap buffers
	glfwSwapBuffers(window);
}

void cleanUp()
{
	//glDeleteProgram(phongShader.shaderPointer);
	//glDeleteProgram(selfIllumShader.shaderPointer);
	//glDeleteBuffers(1, &cubeVBO);
	//glDeleteBuffers(1, &cubeEBO);
	//glDeleteVertexArrays(1, &cubeVAO);

	RenderManager::DumpData();

	//delete teapot;
	ResourceManager::DumpData();
	CameraManager::DumpData();
	LightingManager::DumpData();

	glfwTerminate();
}

int main()
{
	init();

	while (!glfwWindowShouldClose(window))
	{
		step();
		glfwPollEvents();
	}

	cleanUp();

	return 0;
}