#ifndef _GEODE_H_
#define _GEODE_H_

#include "Node.h"

class Geode : public Node {
public:
    bool ifDraw = true;
    bool ifCollide = false;
    glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 toWorld = glm::mat4(1.0f);
    bool solid = true;
    void update(glm::mat4);
    void draw(GLuint, glm::mat4);
};

#endif
