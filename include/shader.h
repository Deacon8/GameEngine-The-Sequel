#pragma once
#include <glm/glm.hpp>
#include "transform.h"

enum UniformType
{
    UVec3,
    UMat4
};

struct Uniform
{   
    UniformType type;
    const char* name;
    void* data;
    int location;
    Uniform(UniformType intype, const char* inname, float* indata, int ShaderProgram);
};

struct Shader
{	
	unsigned int VertexShader;
	unsigned int FragmentShader;
	
	unsigned int ShaderProgram;

    //Uniforms
    Uniform* uniforms;
    unsigned short ucount;
    //Switch to mvp on cpu
    int mvploc;

    //Possibly combine at some point
    unsigned int LoadVertexShader(const char* source);
    unsigned int LoadFragmentShader(const char* source);

    unsigned int LinkShaders(unsigned int vertexShader, unsigned int fragmentShader);

    void DeleteShader(unsigned int shader);

    void SetUniformSampler2D(const char* name, unsigned int unit);

    void UseShader(glm::mat4 mvp);
};

void LoadShaderSource(char* destination, char* path);
Shader LoadShader(char* VertexShaderPath, char* FragmentShaderPath, int UniformCount);