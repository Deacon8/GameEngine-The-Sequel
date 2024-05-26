#include "transform.h"
#include "glm/gtx/euler_angles.hpp"

glm::mat4 Transform::GetMatrix()
{
    //Per object Basis
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), position);  // reposition model
    rotation = glm::rotate(rotation, glm::radians(0.0f), glm::vec3(0, 1, 0));  // rotate model on y axis
    glm::mat4 scaled = glm::scale(glm::mat4(1.0f), scale);
    matrix = trans * rotation * scaled;
    return matrix;
}

void Transform::translate(glm::vec3 transformation)
{
    position += transformation;
}

//This definitely doesn't work
void Transform::eulerRotate(glm::vec3 eulerAngle)
{   
    glm::mat4 transformX = glm::eulerAngleX(eulerAngle.x);
    glm::mat4 transformY = glm::eulerAngleY(eulerAngle.y);
    glm::mat4 transformZ = glm::eulerAngleZ(eulerAngle.z);
    rotation += transformX*transformY*transformZ;
}

void Transform::localScale(glm::vec3 size)
{
    scale += size;
}