#include "transform.h"

glm::mat4 Transform::GetMatrix()
{
    //Per object Basis
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), position);  // reposition model
    rotation = glm::rotate(rotation, glm::radians(0.0f), glm::vec3(0, 1, 0));  // rotate model on y axis
    glm::mat4 scaled = glm::scale(glm::mat4(1.0f), scale);
    matrix = trans * rotation * scaled;
    return matrix;
}