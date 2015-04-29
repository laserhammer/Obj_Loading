#version 440

in vec3 position;
in vec2 texCoord;
in vec3 normal;

layout (std140) uniform camera
{
	mat4 viewProjMat;
	vec4 camPos;
};

layout (std140) uniform perModel
{
	mat4 modelMat;
	mat4 invTransModelMat;
	vec4 color;
};

out vertexData
{
	vec4 Color;
	vec4 Normal;
	vec4 WorldPos;
	vec4 CamPos;
} ;

void main()
{
	Color = color;
	Normal =  invTransModelMat * vec4(normal.xyz, 0.0);
	WorldPos = modelMat * vec4(position.xyz, 1.0);
	CamPos = camPos;
	gl_Position =vec4(position.xyz, 1.0);//viewProjMat * modelMat * vec4(position.xyz, 1.0);
}