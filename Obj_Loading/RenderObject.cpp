#include "RenderObject.h"
#include "ResourceManager.h"
#include <GLM\gtc\type_ptr.hpp>

RenderObject::RenderObject(Mesh* mesh, Shader* shader, GLenum mode, GLuint layer)
{
	_mesh = mesh;
	_shader = shader;
	_transform = Transform();
	_transform.position = glm::vec3();
	_transform.rotation = glm::quat();
	_transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
	_transform.linearVelocity = glm::vec3();
	_transform.angularVelocity = glm::quat();
	_transform.rotationOrigin = glm::vec3();
	_transform.scaleOrigin = glm::vec3();
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
	glBindBufferBase(GL_UNIFORM_BUFFER, ResourceManager::PERMODEL_BIND_POINT, _perModelBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(_perModelBlock), &_perModelBlock, GL_DYNAMIC_DRAW);

	_mode = mode;
	_layer = layer;
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

	memcpy(_perModelBlock.modelMat[0], glm::value_ptr(_transform.model[0]), sizeof(GLfloat) * 4);
	memcpy(_perModelBlock.modelMat[1], glm::value_ptr(_transform.model[1]), sizeof(GLfloat) * 4);
	memcpy(_perModelBlock.modelMat[2], glm::value_ptr(_transform.model[2]), sizeof(GLfloat) * 4);
	memcpy(_perModelBlock.modelMat[3], glm::value_ptr(_transform.model[3]), sizeof(GLfloat) * 4);

	glm::mat4 invTransModel = glm::inverse(glm::transpose(_transform.model));
	memcpy(_perModelBlock.invTransModelMat[0], glm::value_ptr(invTransModel[0]), sizeof(GLfloat) * 4);
	memcpy(_perModelBlock.invTransModelMat[1], glm::value_ptr(invTransModel[1]), sizeof(GLfloat) * 4);
	memcpy(_perModelBlock.invTransModelMat[2], glm::value_ptr(invTransModel[2]), sizeof(GLfloat) * 4);
	memcpy(_perModelBlock.invTransModelMat[3], glm::value_ptr(invTransModel[3]), sizeof(GLfloat) * 4);
}

void RenderObject::Draw()
{
	glBindVertexArray(_mesh->vao);

	glBindBuffer(GL_UNIFORM_BUFFER, _perModelBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(_perModelBlock), &_perModelBlock, GL_DYNAMIC_DRAW);

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
GLuint RenderObject::uniformBufferBindPoint() { return _uBufferBindPoint; }
void RenderObject::uniformBufferBindPoint(GLuint newBindPoint) { _uBufferBindPoint = newBindPoint; }
GLuint RenderObject::layer() { return _layer; }
void RenderObject::layer(GLuint newLayer) { _layer = newLayer; }

