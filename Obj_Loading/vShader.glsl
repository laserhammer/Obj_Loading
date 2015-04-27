#version 440

in vec3 position;
in vec2 texCoord;
in vec3 normal;

layout (std140) uniform camera
{
	mat4 viewProjMat;
	vec4 camPos;
}Camera;

layout (std140) uniform perModel
{
	mat4 modelMat;
	mat4 invTransModelMat;
	vec4 color;
}PerModel;

out vertexData
{
	vec4 Color;
	vec4 Normal;
	vec4 WorldPos;
	vec4 CamPos;
} outData;

void main()
{
	outData.Color = PerModel.color;
	outData.Normal =  PerModel.invTransModelMat * vec4(normal.xyz, 0.0);
	outData.WorldPos = PerModel.modelMat * vec4(position.xyz, 1.0);
	outData.CamPos = Camera.camPos;
	gl_Position = Camera.viewProjMat * PerModel.modelMat * vec4(position.xyz, 1.0);
}