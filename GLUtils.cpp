#include "GLUtils.h"

glm::mat4 frustum(float left, float right, float top, float bottom, 
                  float near, float far, glm::mat4& current)
{
    glm::mat4 trans = glm::mat4(1.0);
    trans[0] = glm::vec4((2*near)/(right-left), 0, 
                        (right+left)/(right-left), 0);
    trans[1] = glm::vec4(0, (2*near)/(top-bottom), 
                         (top+bottom)/(top-bottom), 0);
    trans[2] = glm::vec4(0, 0, -(far+near)/(far-near), 
                        -(2*far*near)/(far-near));
    trans[3] = glm::vec4(0, 0, -1, 0);
    return current*trans;
}
