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

void Shader::SetUniformFloat(const char* name, float value, int location)
{
	glUseProgram(ShaderProgram);
	glUniform1f(location, value);
}

void Shader::SetUniformVec3(const char* name, glm::vec3 &value, int location)
{
	glUseProgram(ShaderProgram);
	glUniform3fv(location, 1, &value[0]);
}

void Shader::SetUniformMat4(const char* name, glm::mat4 &value, int location)
{
	glUseProgram(ShaderProgram);
	glUniformMatrix4fv(location, 1, GL_FALSE, (&value[0][0]));
}

void Shader::SetAllUniforms(Transform model, glm::mat4 &view, glm::mat4 &projection)
{	
	this->SetUniformMat4("projection", projection, mvploc[2]);
	this->SetUniformMat4("view", view, mvploc[1]);
	this->SetUniformMat4("model", model.GetMatrix(), mvploc[0]);
	for(int i = 0; i < ucount; i++)
	{
		switch (uniforms[i].type)
		{
		case UMat4:
			glm::mat4 umat4 = glm::make_mat4(uniforms[i].data);
			this->SetUniformMat4(uniforms[i].name, umat4, uniforms[i].location);
			break;
		case UVec3:
			glm::vec3 uvec3 = glm::make_vec3(uniforms[i].data);
			this->SetUniformVec3(uniforms[i].name, uvec3, uniforms[i].location);
			//std::cout << "Name: " << uniforms[i].name << std::endl;
			//std::cout << "Data: " << glm::to_string(uvec3) << std::endl;
			break;
		
		default:
			printf("This shouldn't happen\n");
			break;
		}
		//printf("3");
	}
}

/*void Shader::SetUniformSampler2D(Shader shader, const char* name, unsigned int unit)
{
	GLint loc = glGetUniformLocation(shader.ShaderProgram, name);
	glUseProgram(shader.ShaderProgram);
	glUniform1i(loc, unit);
}*/

Shader LazyLoadShader(char* VertexShaderPath, char* FragmentShaderPath)
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
	
	return shader;
}