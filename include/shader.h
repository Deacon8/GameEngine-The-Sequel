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
    char* name;
    float* data;
    int location;
};

struct Shader
{	
	unsigned int VertexShader;
	unsigned int FragmentShader;
	
	unsigned int ShaderProgram;

    //Uniforms
    Uniform* uniforms;
    unsigned short ucount;
    int mvploc[3];
    //shader.SetUniformVec3("sun_color", sun_color);
    //type name variable

    //Possibly combine at some point
    unsigned int LoadVertexShader(const char* source);
    unsigned int LoadFragmentShader(const char* source);

    unsigned int LinkShaders(unsigned int vertexShader, unsigned int fragmentShader);

    void DeleteShader(unsigned int shader);

    void SetUniformFloat(const char* name, float value, int location);
    void SetUniformVec3(const char* name, glm::vec3 &value, int location);
    void SetUniformMat4(const char* name, glm::mat4 &value, int location);

    void SetUniformSampler2D(const char* name, unsigned int unit);

    void SetAllUniforms(Transform model, glm::mat4 &view, glm::mat4 &projection);
};

void LoadShaderSource(char* destination, char* path);
Shader LazyLoadShader(char* VertexShaderPath, char* FragmentShaderPath);