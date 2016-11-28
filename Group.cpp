#include "Group.h"
#include "Node.h"
#include "Window.h"


void Group::draw(GLuint shaderProgram, glm::mat4 C) {
    for (std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it) {
        if((*it)->isVisible)
            (*it)->draw(shaderProgram, C);
    }
}


void Group::update(glm::mat4 C) {
    for (std::list<Node*>::iterator it = children.begin(); it != children.end(); it++)
    {
        (*it)->update(C);
    }
}


void Group::addChild(Node* child) {
    children.push_back(child);
}


void Group::removeChild(Node* child)
{
    children.remove(child);
};


void Group::walk() {
    if(rotateAngle >= 30 || rotateAngle <= -30) {
        direction = -direction;
    }
    rotateAngle += direction;
    glm::mat4 translateFront = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f));
    glm::mat4 rotateFront = glm::rotate(glm::mat4(1.0f), direction / 180.0f * glm::pi<float>(), glm::vec3(1,0,0));
    glm::mat4 rotateBack = glm::rotate(glm::mat4(1.0f), -direction / 180.0f * glm::pi<float>(), glm::vec3(1,0,0));
    glm::mat4 translateFrontInverse = glm::inverse(translateFront);
    
    if(front != NULL) {
        front->update(translateFrontInverse * rotateFront * translateFront);
    }
    if(back != NULL) {
        back->update(translateFrontInverse * rotateBack * translateFront);
    }
}
