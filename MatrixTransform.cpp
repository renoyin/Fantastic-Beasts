#include "MatrixTransform.h"

MatrixTransform::MatrixTransform() {
    M = glm::mat4(1.0f);
}

MatrixTransform::MatrixTransform(glm::mat4 m) {
    M = m;
}

void MatrixTransform::update(glm::mat4 C) {
    glm::mat4 CM = C * M;
    Group::update(CM);
}

void MatrixTransform::draw(GLuint shaderProgram, glm::mat4 C) {
    glm::mat4 CM = C * M;
    Group::draw(shaderProgram, CM);
}
