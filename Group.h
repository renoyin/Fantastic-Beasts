#ifndef _GROUP_H_
#define _GROUP_H_

#include "Node.h"
#include "Geode.h"
#include <list>

class Group : public Node {
public:
    Group* front = NULL;
    Group* back = NULL;
    bool canWalk = false;
    float boundRadius;
    glm::vec3 movement = glm::vec3(0.0f);
    float moveDir = 1.0f;
    float position = 0.0f;
    float rotateAngle = 0.0f;
    float direction = 1.0f;
    std::list<Node*> children;
    
    void addChild(Node* child);
    void removeChild(Node* child);
    void walk();
    void update(glm::mat4 C);
    void draw(GLuint shaderProgram, glm::mat4 C);
};

#endif
