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

RenderObject* sphere0;
RenderObject* sphere1;
RenderObject* plane;
RenderObject* cube;

Light* light0;
Light* light1;

void InitScene()
{
	light0 = LightingManager::GetLight(0);
	light0->position.y = 5.0f;
	light0->color = glm::vec4(0.6f, 1.0f, 0.0f, 1.0f);
	light0->ambient = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	light0->power = 5.0f;

	light1 = LightingManager::GetLight(1);
	light1->position = glm::vec3(10.0f, 10.0f, 25.0f);
	light1->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light1->ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	light1->power = 15.0f;

	sphere0 = RenderManager::InitRenderObject(&ResourceManager::sphere, &ResourceManager::phongShader, GL_TRIANGLES, 1);
	sphere0->transform().position = glm::vec3(3.0f, 0.5f, 1.5f);
	sphere0->transform().scale = glm::vec3(3.0f, 1.0f, 1.0f);
	
	sphere1 = RenderManager::InitRenderObject(&ResourceManager::sphere, &ResourceManager::phongShader, GL_TRIANGLES, 1);
	sphere1->transform().position = glm::vec3(-2.0f, 0.5f, -3.5f);
	
	/*plane = RenderManager::InitRenderObject(&ResourceManager::plane, &ResourceManager::phongShader, GL_TRIANGLES, 1);
	plane->transform().scale = glm::vec3(100.0f, 1.0f, 100.0f);
	plane->transform().position = glm::vec3(0.0f, 0.0f, 0.0f);*/

	cube = RenderManager::InitRenderObject(&ResourceManager::cube, &ResourceManager::phongShader, GL_TRIANGLES, 1);
	cube->transform().position = glm::vec3(1.0f, 0.5f, -2.5f);
	cube->transform().scale = glm::vec3(1.0f, 3.0f, 1.0f);
	
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
	InputManager::Init(window);
	CameraManager::Init(800.0f / 600.0f, 45.0f, 0.1f, 100.0f);

	glfwSetTime(0.0);

	time_t timer;
	time(&timer);
	srand((unsigned int)timer);

	InitScene();
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

	CameraManager::Update(dt);

	RenderManager::Update(dt);

	LightingManager::Update(dt);

	RenderManager::Draw(1);

	// Swap buffers
	glfwSwapBuffers(window);
}

void cleanUp()
{
	RenderManager::DumpData();
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