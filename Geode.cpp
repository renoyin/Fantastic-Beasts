#include "Geode.h"

void Geode::update(glm::mat4 C) {
    this->toWorld = C * this->toWorld;
}


void Geode::draw(GLuint shaderProgram, glm::mat4 C) {
    draw(shaderProgram, C);
}
