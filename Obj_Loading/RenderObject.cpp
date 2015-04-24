#include "RenderObject.h"
#include "ResourceManager.h"
#include <GLM\gtc\type_ptr.hpp>

RenderObject::RenderObject(Mesh* mesh, Shader* shader, GLenum mode, GLuint layer, GLuint uBufferBindPoint)
{
	_mesh = mesh;
	_shader = shader;
	_transform = Transform();
	_transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
	_transform.rotation = glm::quat();
	_transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
	_transform.linearVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	_transform.angularVelocity = glm::quat();
	_transform.rotationOrigin = glm::vec3(0.0f, 0.0f, 0.0f);
	_transform.scaleOrigin = glm::vec3(0.0f, 0.0f, 0.0f);
	_transform.translate = glm::mat4();
	_transform.model = glm::mat4();
	_transform.parent = nullptr;

	_perModelBlock = PerModelBlock();
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			_perModelBlock.modelMat[i][j] = i == j ? 1.0f : 0.0f;
			_perModelBlock.invTransModelMat[i][j] = i == j ? 1.0f : 0.0f;
		}
	}
	_perModelBlock.color[0] = 1.0f;
	_perModelBlock.color[1] = 1.0f; 
	_perModelBlock.color[2] = 1.0f;
	_perModelBlock.color[3] = 1.0f;

	glGenBuffers(1, &_perModelBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, _perModelBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(_perModelBlock), &_perModelBlock, GL_DYNAMIC_DRAW);

	_mode = mode;
	_layer = layer;
	_uBufferBindPoint = uBufferBindPoint;
}

RenderObject::~RenderObject()
{
	glDeleteBuffers(1, &_perModelBuffer);
}

void RenderObject::Update(float dt)
{
	// Apply velocities
	_transform.position += _transform.linearVelocity;
	_transform.rotation = glm::slerp(_transform.rotation, _transform.rotation * _transform.angularVelocity, dt);

	// Update model matrix
	glm::mat4 parentModel = _transform.parent ? _transform.parent->model : glm::mat4();
	glm::mat4 parentTrans = _transform.parent ? _transform.parent->translate : glm::mat4();

	_transform.translate = glm::translate(glm::mat4(), _transform.position);

	glm::mat4 rotOrigin = glm::translate(glm::mat4(), _transform.rotationOrigin);
	glm::mat4 globalTranslateRotOrigin = parentTrans * _transform.translate * rotOrigin;
	glm::mat4 rotate = glm::inverse(globalTranslateRotOrigin) * glm::mat4_cast(_transform.rotation) * globalTranslateRotOrigin;

	glm::mat4 scaleOrigin = glm::translate(glm::mat4(), _transform.scaleOrigin);
	glm::mat4 globalTraslateScaleOrigin = parentTrans * _transform.translate * scaleOrigin;
	glm::mat4 scale = glm::inverse(globalTraslateScaleOrigin) * glm::scale(glm::mat4(), _transform.scale) * globalTraslateScaleOrigin;

	_transform.model = parentModel * (_transform.translate * rotate * scale);
	memcpy(_perModelBlock.modelMat, glm::value_ptr(_transform.model), sizeof(GLfloat) * 16);
	memcpy(_perModelBlock.invTransModelMat, glm::value_ptr(glm::inverse(glm::transpose(_transform.model))), sizeof(GLfloat) * 16);
}

void RenderObject::Draw()
{
	glBindVertexArray(_mesh->vao);

	glBindBuffer(GL_UNIFORM_BUFFER, _perModelBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(_perModelBlock), &_perModelBlock, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, _uBufferBindPoint, _perModelBuffer);

	glUseProgram(_shader->program);

	glDrawElements(_mode, _mesh->count, GL_UNSIGNED_INT, 0);
}

Transform& RenderObject::transform() { return _transform; }
void RenderObject::setColor(glm::vec4 color)
{ 
	memcpy(_perModelBlock.color, glm::value_ptr(color), sizeof(GLfloat) * 4);
}

GLenum RenderObject::mode() { return _mode; }
void RenderObject::mode(GLenum newMode) { _mode = newMode; }
GLuint RenderObject::uniformBufferBindPoint() { return _uniformBufferBindPoint; }
void RenderObject::uniformBufferBindPoint(GLuint newBindPoint) { _uniformBufferBindPoint = newBindPoint; }

