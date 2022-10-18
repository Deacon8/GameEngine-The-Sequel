#pragma once
#include <glm/glm.hpp>
struct Shader
{	
	unsigned int VertexShader;
	unsigned int FragmentShader;
	
	unsigned int ShaderProgram;

    //Possibly combine at some point
    unsigned int LoadVertexShader(const char* source);
    unsigned int LoadFragmentShader(const char* source);

    unsigned int LinkShaders(unsigned int vertexShader, unsigned int fragmentShader);

    void DeleteShader(unsigned int shader);

    void SetUniformFloat(const char* name, float value);
    void SetUniformVec3(const char* name, glm::vec3 &value);
    void SetUniformMat4(const char* name, glm::mat4 &value);

    void SetUniformSampler2D(const char* name, unsigned int unit);
};
typedef struct Shader Shader;

void LoadShaderSource(char* destination, char* path);
Shader LazyLoadShader(char* VertexShaderPath, char* FragmentShaderPath);