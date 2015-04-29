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
	-1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	+1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	+1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-1.0f, +1.0f, +1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	+1.0f, +1.0f, +1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	+1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
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
	//char* sphereObj = ReadTextFile("../Resources/meshes/Sphere.obj");
	//GLfloat *vertPos = nullptr;
	//unsigned int numVertPos;
	//GLfloat *vertNorms = nullptr;
	//unsigned int numVertNorms;
	//GLint **elements = nullptr;
	//unsigned int numElements;
	//ParseOBJ(sphereObj, &vertPos, numVertPos, &vertNorms, numVertNorms, (GLint***)&elements, numElements);

	//GLfloat* verts = nullptr;
	//unsigned int numVerts;
	//GLuint* vertElements = nullptr;
	//GenVertices(&verts, numVerts, &vertElements, vertPos, vertNorms, numVertNorms, (GLint**)elements, numElements);

	//GenMesh(verts, numVerts, vertElements, numElements, sphere);

	//GenMesh(vertices, 8, elements, 36, sphere);

	sphere = Mesh();

	glGenVertexArrays(1, &sphere.vao);
	glBindVertexArray(sphere.vao);

	glGenBuffers(1, &sphere.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sphere.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8 * 8, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &sphere.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * 36, elements, GL_STATIC_DRAW);

	sphere.count = 36;
	/*
	delete[] verts;
	delete[] vertElements;

	delete[] sphereObj;
	delete[] vertPos;
	delete[] vertNorms;
	for (unsigned int i = 0; i < numElements; ++i)
	{
		delete[] elements[i];
	}
	delete[] elements;*/

	GLuint shaders[2];
	
	phongFragShader = CompileShader("fshader.glsl", GL_FRAGMENT_SHADER);
	phongVertShader = CompileShader("vshader.glsl", GL_VERTEX_SHADER);

	shaders[0] = phongFragShader;
	shaders[1] = phongVertShader;

	phongShader = Shader();
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


	GLuint posAttrib = glGetAttribLocation(phongShader.program, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, sizeof(GLfloat) * 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);

	GLuint texAttrib = glGetAttribLocation(phongShader.program, "texCoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, sizeof(GLfloat) * 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 3));

	GLuint normAttrib = glGetAttribLocation(phongShader.program, "normal");
	glEnableVertexAttribArray(normAttrib);
	glVertexAttribPointer(normAttrib, sizeof(GLfloat) * 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 5));
}

void ResourceManager::DumpData()
{
	ReleaseMesh(sphere);
	glDeleteShader(phongFragShader);
	glDeleteShader(phongVertShader);
	glDeleteProgram(phongShader.program);

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

void ResourceManager::ParseOBJ(char* obj, GLfloat** vertPos, unsigned int& numVertPos, GLfloat** vertNorm, unsigned int& numVertNorms, GLint*** elements, unsigned int& numElements)
{
	numVertPos= 0;
	numVertNorms = 0;
	numElements = 0;

	int vertPosStorage = 300;
	int vertNormStorage = 300;
	int elementStorage = 100;

	*vertPos = new GLfloat[vertPosStorage];
	*vertNorm = new GLfloat[vertNormStorage];
	*elements = (GLint**)(new GLint[elementStorage][3]);
	for (int i = 0; i < elementStorage; ++i)
	{
		(*elements)[i] = new GLint[3];
	}

	int vert, norm, vertEle, elementIndex, numTerms, term, termItr, lineItr, elementItr;
	char currentChar, line[255], (*terms)[32], elementString[32];
	vert = 0;
	norm = 0;
	vertEle = 0;
	for (int i = 0; obj[i]; ++i)
	{
		currentChar = obj[i];
		if (currentChar != '\n')
		{
			// Get the line we're on and put it in its own string
			numTerms = 1;
			for (int j = 0;; ++j, ++i)
			{
				currentChar = obj[i];
				if (currentChar != '\n' && currentChar)
				{
					line[j] = currentChar;
					numTerms += currentChar == ' ';
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
			terms = new char[numTerms][32];
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
				// Position, store in vertPos
				if (vert + 3 >= vertPosStorage)
				{
					GLfloat *temp = *vertPos;
					*vertPos = new GLfloat[vertPosStorage * 2];
					memcpy(*vertPos, temp, sizeof(GLfloat) * vertPosStorage);
					vertPosStorage *= 2;
					delete[] temp;
				}
				(*vertPos)[vert] = StringToFloat(terms[1]);
				(*vertPos)[vert + 1] = StringToFloat(terms[2]);
				(*vertPos)[vert + 2] = StringToFloat(terms[3]);
				++numVertPos;
				vert += 3;
				delete[] terms;
			}

			if (terms[0][0] == 'v' && terms[0][1] == 'n')
			{
				// Normal, store in vertNorm
				if (norm + 3 >= vertNormStorage)
				{
					GLfloat *temp = *vertNorm;
					*vertNorm = new GLfloat[vertNormStorage * 2];
					memcpy(*vertNorm, temp, sizeof(GLfloat) * vertNormStorage);
					vertNormStorage *= 2;
					delete[] temp;
				}
				(*vertNorm)[norm] = StringToFloat(terms[1]);
				(*vertNorm)[norm + 1] = StringToFloat(terms[2]);
				(*vertNorm)[norm + 2] = StringToFloat(terms[3]);
				++numVertNorms;
				norm += 3;
				delete[] terms;
			}

			if (terms[0][0] == 'f')
			{
				// Face, store in elements
				if (vertEle + 4 >= elementStorage)
				{
					GLint** temp = *elements;
					*elements = (GLint**)new GLint[elementStorage * 2][3];
					for (int i = 0; i < elementStorage * 2; ++i)
					{
						(*elements)[i] = new GLint[3];
					}
					
					for (int i = 0; i < elementStorage; ++i)
					{
						memcpy((*elements)[i], temp[i], sizeof(GLuint) * 3);
					}
					for (int i = 0; i < elementStorage; ++i)
					{
						delete[] temp[i];
					}
					elementStorage *= 2;
					delete[] temp;
				}
				
				// Parse each of the terms in the terms array by '\' and store the resulting value in elements
				elementItr = 0;
				for (term = 1; term < numTerms; ++term)
				{
					elementIndex = 0;
					for (termItr = 0; terms[term][termItr]; ++termItr)
					{
						if (terms[term][termItr] == '/' )
						{
							elementString[elementItr] = '\0';
							(*elements)[vertEle][elementIndex] = StringToUnsiginedInt(elementString);
							elementItr = 0;
							++elementIndex;
						}
						else
						{
							elementString[elementItr] = terms[term][termItr];
							++elementItr;
						}
					}
					// Add last element
					elementString[elementItr] = '\0';
					(*elements)[vertEle][elementIndex] = StringToUnsiginedInt(elementString);
					elementItr = 0;
					++numElements;
					++vertEle;
				}
			}
		}
	}
}

void ResourceManager::GenMesh(GLfloat* verts, GLuint numVerts, GLuint* elements, GLuint numElements, Mesh& mesh)
{
	mesh = Mesh();
	
	glGenVertexArrays(1, &mesh.vao);
	glBindVertexArray(mesh.vao);

	glGenBuffers(1, &mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVerts * 8, verts, GL_STATIC_DRAW);

	glGenBuffers(1, &mesh.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * numElements, elements, GL_STATIC_DRAW);

	mesh.count = numElements;
}

void ResourceManager::GenVertices(GLfloat** verts, unsigned int& numVerts, GLuint** vertElements, GLfloat* vertPos, GLfloat* vertNorms, unsigned int numVertNorms, GLint** elements, unsigned int numElements)
{
	int vertStorage = numVerts >= numVertNorms ? numVerts : numVertNorms;
	*vertElements = new GLuint[numElements];
	unsigned int uniqueElementCount = 0;
	bool commonVert, commonUV, commonNorm;
	for (unsigned int i = 0; i < numElements; ++i)
	{
		if ((*vertElements)[i] > i)
		{
			(*vertElements)[i] = uniqueElementCount++;
		}
		// Compare each element to all other elements to find common verts
		for (unsigned int j = i + 1; j < numElements; ++j)
		{
			commonVert = elements[i][0] == elements[j][0];
			commonUV = elements[i][1] == elements[j][1];
			commonNorm = elements[i][2] == elements[j][2];
			if (commonVert && commonUV && commonNorm)
			{
				(*vertElements)[j] = (*vertElements)[i];
			}
		}
	}

	*verts = new GLfloat[uniqueElementCount];
	numVerts = uniqueElementCount;
	unsigned int vertElement, u, posValueIndex, normValueIndex;
	u = 0;
	for (unsigned int i = 0; i < numElements; ++i)
	{
		if ((*vertElements)[i] == u)
		{
			vertElement = (*vertElements)[i];
			posValueIndex = (elements[vertElement][0] - 1) * 3;
			normValueIndex = (elements[vertElement][2] - 1) * 3;
			
			(*verts)[u * 8] = vertPos[posValueIndex];
			(*verts)[u * 8 + 1] = vertPos[posValueIndex + 1];
			(*verts)[u * 8 + 2] = vertPos[posValueIndex + 2];

			(*verts)[u * 8 + 3] = 0.0f;
			(*verts)[u * 8 + 4] = 0.0f;

			(*verts)[u * 8 + 5] = vertNorms[normValueIndex];
			(*verts)[u * 8 + 6] = vertNorms[normValueIndex + 1];
			(*verts)[u * 8 + 7] = vertNorms[normValueIndex + 2];
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