#pragma once
#include <GLEW\GL\glew.h>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\quaternion.hpp>

struct Mesh;
struct Shader;
struct CameraBlock;

struct PerModelBlock
{
	float modelMat[4][4];
	float invTransModelMat[4][4];
	float color[4];
};

struct Transform
{
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	glm::vec3 linearVelocity;
	glm::quat angularVelocity;
	glm::vec3 rotationOrigin;
	glm::vec3 scaleOrigin;
	glm::mat4 translate;
	glm::mat4 model;
	Transform* parent;
};

class RenderObject
{
public:
	RenderObject(Mesh* mesh = nullptr, Shader* shader = nullptr, GLenum mode = GL_TRIANGLES, GLuint layer = 1);
	~RenderObject();

	void Update(float dt);
	void Draw();

	Transform& transform();
	void setColor(glm::vec4 color);
	bool active();
	void active(bool isActive);
	bool useDepthTest();
	void useDepthTest(bool usingDepth);
	GLenum mode();
	void mode(GLenum newMode);
	GLuint uniformBufferBindPoint();
	void uniformBufferBindPoint(GLuint newBindPoint);
	GLuint layer();
	void layer(GLuint newLayer);
private:
	Mesh* _mesh;
	Shader* _shader;
	PerModelBlock _perModelBlock;
	GLuint _perModelBuffer;
	Transform _transform;
	GLenum _mode;
	GLuint _uBufferBindPoint;
	GLuint _layer;
};