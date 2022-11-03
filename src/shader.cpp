#include "shader.h"
#include "transform.h"
#include "memory.h"
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include "glm/gtx/string_cast.hpp"
#include <stdio.h>
#include <stdlib.h>
//#include <iostream>

void LoadShaderSource(char* destination, char* path)
{	
	FILE* file = fopen(path, "r");
	if (file != NULL) 
	{
		size_t newLen = fread(destination, sizeof(char), GetFileSize(path), file);
		if ( ferror( file ) != 0 ) 
		{
			fputs("Error reading file", stderr);
		} 
		else 
		{
			destination[newLen++] = '\0';
		}

		fclose(file);
	}
}

GLint CheckCompile(unsigned int shader)
{
	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	return isCompiled;
}

GLint CheckLink(unsigned int program)
{
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	return isLinked;
}

//Possibly combine at some point
unsigned int Shader::LoadVertexShader(const char* source)
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &source, NULL);
    glCompileShader(vertexShader);
	if(CheckCompile(vertexShader) == GL_FALSE)
	{
		printf("Vertex Compile Error");
	}
	return vertexShader;
}

unsigned int Shader::LoadFragmentShader(const char* source)
{
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &source, NULL);
    glCompileShader(fragmentShader);
	if(CheckCompile(fragmentShader) == GL_FALSE)
	{
		printf("Fragment Compile Error");
	}
	return fragmentShader;
}

unsigned int Shader::LinkShaders(unsigned int vertexShader, unsigned int fragmentShader)
{
	unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
	if(CheckLink(shaderProgram) == GL_FALSE)
	{
		printf("Link Error");
	}
	return shaderProgram;
}

void Shader::DeleteShader(unsigned int shader)
{
	glDeleteShader(shader);
}

void Shader::SetAllUniforms(glm::mat4 mvp)
{	
	//TODO: Find out where sampler is being set -> Somewhere in model loading -> seems ok for now
	glUseProgram(ShaderProgram);
	//Calculate on CPU first
	glUniformMatrix4fv(mvploc, 1, GL_FALSE, &(mvp)[0][0]);
	for(int i = 0; i < ucount; i++)
	{	
		switch (uniforms[i].type)
		{
		case UMat4:
			glm::mat4 umat4 = glm::make_mat4((float*)uniforms[i].data);
			glUniformMatrix4fv(uniforms[i].location, 1, GL_FALSE, &umat4[0][0]);
			break;
		case UVec3:
			glm::vec3 uvec3 = glm::make_vec3((float*)uniforms[i].data);
			glUniform3fv(uniforms[i].location, 1, &uvec3[0]);
			break;
		
		default:
			printf("This shouldn't happen\n");
			break;
		}
	}
}

Uniform::Uniform(UniformType intype, const char* inname, float* indata, int ShaderProgram)
{   
	name = inname;
	type = intype;
	location = glGetUniformLocation(ShaderProgram, inname);
	name = (char*)malloc(sizeof(char) * strlen(inname));
	switch(type)
	{
		case UVec3:
		data = (float*)malloc(sizeof(glm::vec3));
		break;
		case UMat4:
		data = (float*)malloc(sizeof(glm::mat4));
		break;
	}
	data = indata;
}

Shader LoadShader(char* VertexShaderPath, char* FragmentShaderPath, int UniformCount)
{	
	Shader shader;
	int vsize = GetFileSize(VertexShaderPath)+1;
	int fsize = GetFileSize(FragmentShaderPath)+1;
	char* BasicVertex = (char*)malloc(vsize);
	char* BasicFragment = (char*)malloc(fsize);
	LoadShaderSource(BasicVertex, VertexShaderPath);
	LoadShaderSource(BasicFragment, FragmentShaderPath);
	
	shader.VertexShader = shader.LoadVertexShader((const char*)BasicVertex);
	shader.FragmentShader = shader.LoadFragmentShader((const char*)BasicFragment);
	shader.ShaderProgram = shader.LinkShaders(shader.VertexShader, shader.FragmentShader);

	shader.DeleteShader(shader.VertexShader);
	shader.DeleteShader(shader.FragmentShader);

	shader.mvploc = glGetUniformLocation(shader.ShaderProgram, "mvp");

	shader.ucount = UniformCount;
	
	return shader;
}