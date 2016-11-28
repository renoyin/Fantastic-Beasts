#ifndef _MATRIXTRANSFORM_H_
#define _MATRIXTRANSFORM_H_

#include "Group.h"
#include <list>
#include "Window.h"

class MatrixTransform : public Group {
public:
    MatrixTransform();
    MatrixTransform(glm::mat4);
    bool checkVis();
    void update(glm::mat4);
    void draw(GLuint, glm::mat4);
};

#endif
