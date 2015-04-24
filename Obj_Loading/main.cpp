#include "GLEW/GL/glew.h"
#include "GLFW/glfw3.h"
#include <ctime>
#include <random>

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

void init()
{
	ResourceManager::Init();

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

	//initShaders();

	glfwSetTime(0.0);

	time_t timer;
	time(&timer);
	srand((unsigned int)timer);

	//SetupLights();

	//generateTeapot();

	//InputManager::Init(window);
	//CameraManager::Init(800.0f / 600.0f, 60.0f, 0.1f, 100.0f);
}

void step()
{
	// Clear to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	InputManager::Update();

	// Get delta time since the last frame
	float dt = glfwGetTime();
	glfwSetTime(0.0);

	//float dTheta = 45.0f * InputManager::rightKey();
	//dTheta -= 45.0f * InputManager::leftKey();

	//teapot->transform().angularVelocity = glm::angleAxis(dTheta, glm::vec3(0.0f, 1.0f, 0.0f));

	CameraManager::Update(dt);

	RenderManager::Update(dt);

	//LightingManager::Update(dt);

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