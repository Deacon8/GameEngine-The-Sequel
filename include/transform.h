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

    glm::mat4 GetMatrix()
    {
        //Per object Basis
        glm::mat4 trans = glm::translate(glm::mat4(1.0f), position);  // reposition model
        rotation = glm::rotate(rotation, glm::radians(0.0f), glm::vec3(0, 1, 0));  // rotate model on y axis
        glm::mat4 scaled = glm::scale(glm::mat4(1.0f), scale);
        matrix = trans * rotation * scaled;
        return matrix;
    }
};