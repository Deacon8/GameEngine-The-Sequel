#pragma once
#include "gltf/tiny_gltf.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <glad/glad.h>

struct Model
{ 
  tinygltf::Model gltf;
  std::pair<GLuint, std::map<int, GLuint>> vaoAndEbos;
};

//Model Loading
//std::pair<GLuint, std::map<int, GLuint>> bindModel(tinygltf::Model &model);

Model loadModel(const char *filename);

void bindMesh(std::map<int, GLuint>& vbos, tinygltf::Model &model, tinygltf::Mesh &mesh);

void bindModelNodes(std::map<int, GLuint>& vbos, tinygltf::Model &model, tinygltf::Node &node);

std::pair<GLuint, std::map<int, GLuint>> bindModel(tinygltf::Model &model);

void drawMesh(const std::map<int, GLuint>& vbos, tinygltf::Model &model, tinygltf::Mesh &mesh);

// recursively draw node and children nodes of model
void drawModelNodes(const std::pair<GLuint, std::map<int, GLuint>>& vaoAndEbos, tinygltf::Model &model, tinygltf::Node &node);

void debugModel(tinygltf::Model &model);

//Rendering

void DrawObject(Model model);


