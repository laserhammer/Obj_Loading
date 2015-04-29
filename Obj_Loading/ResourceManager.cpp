#include "ResourceManager.h"
#include "LightingManager.h"
#include <fstream>
#include <iostream>

Shader ResourceManager::phongShader;
Mesh ResourceManager::sphere;
GLuint ResourceManager::phongVertShader;
GLuint ResourceManager::phongFragShader;

const GLuint ResourceManager::PERMODEL_BIND_POINT = 0;
const GLuint ResourceManager::LIGHTS_BIND_POINT = 1;
const GLuint ResourceManager::CAMERA_BIND_POINT = 2;

GLubyte* ResourceManager::perModelBuffer;
GLubyte* ResourceManager::cameraBuffer;
GLubyte* ResourceManager::lightsBuffer;

GLint ResourceManager::perModelBufferSize;
GLint ResourceManager::cameraBufferSize;
GLint ResourceManager::lightsBufferSize;

GLfloat vertices[] = {
	-1.0f, +1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
	+1.0f, +1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
	+1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
	-1.0f, +1.0f, +1.0f, 1.0f, 1.0f, 1.0f,
	+1.0f, +1.0f, +1.0f, 1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, +1.0f, 1.0f, 1.0f, 1.0f,
	+1.0f, -1.0f, +1.0f, 1.0f, 1.0f, 1.0f
};

GLuint elements[] = {
	// Front
	0, 1, 2,
	1, 3, 2,

	// Back
	6, 5, 4,
	6, 7, 5,

	// Top
	4, 5, 0,
	1, 0, 5,

	// Bottom
	7, 2, 3,
	7, 6, 2,

	// Right
	1, 5, 7,
	7, 3, 1,

	// Left
	6, 4, 0,
	0, 2, 6,
};


void ResourceManager::Init()
{
	char* sphereObj = ReadTextFile("../Resources/meshes/Sphere.obj");
	std::vector<GLfloat> vertPos = std::vector<GLfloat>();
	std::vector<GLfloat> vertNorms = std::vector<GLfloat>();
	std::vector<GLint> elements = std::vector<GLint>();
	ParseOBJ(sphereObj, &vertPos, &vertNorms, &elements);

	std::vector<GLfloat> verts = std::vector<GLfloat>();
	std::vector<GLint> vertElements = std::vector<GLint>();
	GenVertices(&verts, &vertElements, &vertPos, &vertNorms, &elements);

	GenMesh(&verts, &elements, sphere);

	GLuint shaders[2];
	
	phongFragShader = CompileShader("fshader.glsl", GL_FRAGMENT_SHADER);
	phongVertShader = CompileShader("vshader.glsl", GL_VERTEX_SHADER);

	shaders[0] = phongFragShader;
	shaders[1] = phongVertShader;

	phongShader = Shader();
	phongShader.shaderPointer = LinkShaderProgram(shaders, 2, 0, "outColor");;
	phongShader.uModelMat = glGetUniformLocation(phongShader.shaderPointer, "modelMat");
	phongShader.uViewMat = glGetUniformLocation(phongShader.shaderPointer, "viewMat");
	phongShader.uProjMat = glGetUniformLocation(phongShader.shaderPointer, "projMat");
	phongShader.uColor = glGetUniformLocation(phongShader.shaderPointer, "color");
	phongShader.uCamPos = glGetUniformLocation(phongShader.shaderPointer, "camPos");

	GLint posAttrib = glGetAttribLocation(phongShader.shaderPointer, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

	GLint normAttrib = glGetAttribLocation(phongShader.shaderPointer, "normal");
	glEnableVertexAttribArray(normAttrib);
	glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
	/*
	phongShader.program = LinkShaderProgram(shaders, 2, 0, "outColor");


	phongShader.uPerModelBlockIndex = glGetUniformBlockIndex(phongShader.program, "perModel");
	glUniformBlockBinding(phongShader.program, phongShader.uPerModelBlockIndex, PERMODEL_BIND_POINT);
	glGetActiveUniformBlockiv(phongShader.program, phongShader.uPerModelBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &perModelBufferSize);
	perModelBuffer = new GLubyte[perModelBufferSize];

	phongShader.uCameraBlockIndex = glGetUniformBlockIndex(phongShader.program, "camera");
	glUniformBlockBinding(phongShader.program, phongShader.uCameraBlockIndex, CAMERA_BIND_POINT);
	glGetActiveUniformBlockiv(phongShader.program, phongShader.uCameraBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &cameraBufferSize);
	cameraBuffer = new GLubyte[cameraBufferSize];

	phongShader.uLightsBlockIndex = glGetUniformBlockIndex(phongShader.program, "lightsBlock");
	glUniformBlockBinding(phongShader.program, phongShader.uLightsBlockIndex, LIGHTS_BIND_POINT);
	glGetActiveUniformBlockiv(phongShader.program, phongShader.uLightsBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &lightsBufferSize);
	lightsBuffer = new GLubyte[lightsBufferSize];


	GLint posAttrib = glGetAttribLocation(phongShader.program, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, sizeof(GLfloat) * 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);

	GLint texAttrib = glGetAttribLocation(phongShader.program, "texCoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, sizeof(GLfloat) * 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 3));

	GLint normAttrib = glGetAttribLocation(phongShader.program, "normal");
	glEnableVertexAttribArray(normAttrib);
	glVertexAttribPointer(normAttrib, sizeof(GLfloat) * 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 5));*/

}

void ResourceManager::DumpData()
{
	ReleaseMesh(sphere);
	glDeleteShader(phongFragShader);
	glDeleteShader(phongVertShader);
	//glDeleteProgram(phongShader.program);
	glDeleteProgram(phongShader.shaderPointer);

	delete[] cameraBuffer;
	delete[] lightsBuffer;
	delete[] perModelBuffer;
}

char* ResourceManager::ReadTextFile(const char* filepath)
{
	FILE* file;
	char* content = NULL;
	errno_t err;

	int count = 0;

	if (filepath != NULL)
	{
		err = fopen_s(&file, filepath, "rt");

		if (file != NULL)
		{
			fseek(file, 0, SEEK_END);
			count = ftell(file);
			rewind(file);

			if (count > 0)
			{
				content = new char[count + 1];
				count = fread(content, sizeof(char), count, file);
				content[count] = '\0';
			}
			err = fclose(file);
		}
	}
	return content;
}

GLuint ResourceManager::CompileShader(char* shader, GLenum type)
{
	//Create the shader
	GLuint retShader;

	//get a shader handler
	retShader = glCreateShader(type);
	//read the shader from the source file
	const char* shaderSource = ReadTextFile(shader);
	//pass source to GL
	glShaderSource(retShader, 1, &shaderSource, NULL);
	//delete the memory from the source text
	delete[] shaderSource;
	//Compile shader
	glCompileShader(retShader);

	//Check compilation errors
	GLint  compiled;
	glGetShaderiv(retShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		std::cerr << shader << " failed to compile:" << std::endl;
		GLint  logSize;
		glGetShaderiv(retShader, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetShaderInfoLog(retShader, logSize, NULL, logMsg);
		std::cerr << logMsg << std::endl;
		delete[] logMsg;
		system("pause");
		exit(EXIT_FAILURE);
	}
	return retShader;
}

GLuint ResourceManager::LinkShaderProgram(GLuint* shaders, int numShaders, GLuint fragDataBindColorNumber, char* fragDataBindName)
{
	GLuint program = glCreateProgram();

	for (int i = 0; i < numShaders; ++i)
	{
		glAttachShader(program, shaders[i]);
	}

	//Tell the fragment shader which buffer to write to
	glBindFragDataLocation(program, fragDataBindColorNumber, fragDataBindName);

	glLinkProgram(program);

	//Check linking errors
	GLint  linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		std::cerr << "Shader program failed to link" << std::endl;
		GLint  logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetProgramInfoLog(program, logSize, NULL, logMsg);
		std::cerr << logMsg << std::endl;
		delete[] logMsg;

		exit(EXIT_FAILURE);
	}

	return program;
}

float StringToFloat(const char* string)
{
	bool decimalPointHit = false;
	bool negative = false;
	float decimalFactor = 1.0f;
	float retFloat = 0.0f;
	int stringItr = 0;
	float digit = 1.0f;
	while (string[stringItr])
	{
		if (string[stringItr] == '-')
		{
			negative = true;
		}
		else if (string[stringItr] == '.')
		{
			decimalPointHit = true;
		}
		else
		{
			float value = (float)(string[stringItr] - 48);
			if (decimalPointHit)
			{
				decimalFactor *= 0.1f;
				retFloat += value * decimalFactor;
			}
			else
			{
				retFloat = retFloat * digit + value;
				digit *= 10.0f;
			}
		}
		++stringItr;
	}
	if (negative) retFloat *= -1.0f;

	return retFloat;
}

unsigned int StringToUnsiginedInt(const char* string)
{
	unsigned int retInt = 0;
	int stringItr = 0;
	int digit = 1;
	int value;
	while (string[stringItr])
	{
		value = string[stringItr] - 48;
		retInt = retInt * digit + value;
		digit *= 10;
		++stringItr;
	}
	return retInt;
}

void ResourceManager::ParseOBJ(char* obj, std::vector<GLfloat>* vertPos, std::vector<GLfloat>* vertNorm, std::vector<GLint>* elements)
{
	vertPos->reserve(300);
	vertNorm->reserve(300);
	elements->reserve(300);
	int maxNumTerms = 5;
	int term, termItr, lineItr, elementItr;
	char currentChar, line[255], elementString[32];
	std::vector<char*> terms = std::vector<char*>();
	terms.resize(maxNumTerms);
	for (int i = 0; i < maxNumTerms; ++i)
	{
		terms[i] = new char[32];
	}
	for (int i = 0; obj[i]; ++i)
	{
		currentChar = obj[i];
		if (currentChar != '\n')
		{
			// Get the line we're on and put it in its own string
			for (int j = 0;; ++j, ++i)
			{
				currentChar = obj[i];
				if (currentChar != '\n' && currentChar)
				{
					line[j] = currentChar;
				}
				else
				{
					line[j] = '\0';
					break;
				}
			}
			 // If the line starts with '#' then it is a comment and should be ignored
			if (line[0] == '#')
			{
				continue;
			}

			// Parse the line by ' ' and put each value into its own string
		
			term = 0;
			termItr = 0;
			lineItr = 0;
			while (line[lineItr])
			{
				if (line[lineItr] != ' ')
				{
					terms[term][termItr++] = line[lineItr];
				}
				else
				{
					terms[term][termItr] = '\0';
					term++;
					termItr = 0;
				}
				lineItr++;
			}
			terms[term][termItr] = '\0';

			// The first term will determine what type of data follows
			if (terms[0][0] == 'v'&& !terms[0][1])
			{
				// Vertex store in the vertPos vector
				vertPos->push_back(StringToFloat(terms[1]));
				vertPos->push_back(StringToFloat(terms[2]));
				vertPos->push_back(StringToFloat(terms[3]));
			}

			if (terms[0][0] == 'v' && terms[0][1] == 'n')
			{
				// Normal, store in vertNorm
				vertNorm->push_back(StringToFloat(terms[1]));
				vertNorm->push_back(StringToFloat(terms[2]));
				vertNorm->push_back(StringToFloat(terms[3]));
			}

			if (terms[0][0] == 'f')
			{
				// Face, store in elements
				// Parse each of the terms in the terms array by '\' and store the resulting value in elements
				elementItr = 0;
				for (term = 1; term < 5; ++term)
				{
					for (termItr = 0; terms[term][termItr]; ++termItr)
					{
						if (terms[term][termItr] == '/' )
						{
							elementString[elementItr] = '\0';
							elements->push_back(StringToUnsiginedInt(elementString));
							elementItr = 0;
						}
						else
						{
							elementString[elementItr] = terms[term][termItr];
							++elementItr;
						}
					}
					// Add last element
					elementString[elementItr] = '\0';
					elements->push_back(StringToUnsiginedInt(elementString));
					elementItr = 0;
				}
			}
		}
	}
	for (int i = 0; i < maxNumTerms; ++i)
	{
		delete[] terms[i];
	}
}

void ResourceManager::GenMesh(std::vector<GLfloat>* verts, std::vector<GLint>* elements, Mesh& mesh)
{
	mesh = Mesh();
	
	glGenVertexArrays(1, &mesh.vao);
	glBindVertexArray(mesh.vao);

	glGenBuffers(1, &mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*verts), verts, GL_STATIC_DRAW);

	glGenBuffers(1, &mesh.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*elements), elements, GL_STATIC_DRAW);

	mesh.count = elements->size();
}

void ResourceManager::GenVertices(std::vector<GLfloat>* verts, std::vector<GLint>* vertElements, std::vector<GLfloat>* vertPos, std::vector<GLfloat>* vertNorms, std::vector<GLint>* elements)
{
	unsigned int numElements = elements->size() / 3;
	vertElements->resize(numElements);
	unsigned int uniqueElementCount = 0;
	bool commonVert, commonUV, commonNorm;
	for (unsigned int i = 0; i < numElements; ++i)
	{
		if ((*vertElements)[i] == 0)
		{
			(*vertElements)[i] = uniqueElementCount++;
		}
		// Compare each element to all other elements to find common verts
		for (unsigned int j = i + 1; j < numElements; ++j)
		{
			commonVert = (*elements)[i * 3] == (*elements)[j * 3];
			commonUV = (*elements)[i * 3 + 1] == (*elements)[j * 3 + 1];
			commonNorm = (*elements)[i * 3 + 2] == (*elements)[j * 3 + 2];
			if (commonVert && commonUV && commonNorm)
			{
				(*vertElements)[j] = (*vertElements)[i];
			}
		}
	}

	verts->resize(uniqueElementCount * 8);
	unsigned int vertElement, u, posValueIndex, normValueIndex;
	u = 0;
	for (unsigned int i = 0; i < numElements; ++i)
	{
		if ((*vertElements)[i] == u)
		{
			vertElement = (*vertElements)[i];
			posValueIndex = ((*elements)[vertElement * 3] - 1) * 3;
			normValueIndex = ((*elements)[vertElement * 3 + 2] - 1) * 3;
			
			(*verts)[u * 8] = (*vertPos)[posValueIndex];
			(*verts)[u * 8 + 1] = (*vertPos)[posValueIndex + 1];
			(*verts)[u * 8 + 2] = (*vertPos)[posValueIndex + 2];

			(*verts)[u * 8 + 3] = 0.0f;
			(*verts)[u * 8 + 4] = 0.0f;

			(*verts)[u * 8 + 5] = (*vertNorms)[normValueIndex];
			(*verts)[u * 8 + 6] = (*vertNorms)[normValueIndex + 1];
			(*verts)[u * 8 + 7] = (*vertNorms)[normValueIndex + 2];
			++u;
		}
		else
			continue;
	}
}

void ResourceManager::ReleaseMesh(Mesh& mesh)
{
	glDeleteVertexArrays(1, &mesh.vao);
	glDeleteBuffers(1, &mesh.vbo);
	glDeleteBuffers(1, &mesh.ebo);
}