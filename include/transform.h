#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Transform
{
    glm::mat4 matrix = glm::mat4(1.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 scale = glm::vec3(1, 1, 1);

    glm::mat4 GetMatrix();

    void translate(glm::vec3 transformation);
    void eulerRotate(glm::vec3 eulerAngle);
    void localScale(glm::vec3 size);
};