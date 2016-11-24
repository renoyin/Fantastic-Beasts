#ifndef _Node_H_
#define _Node_H_

#include <stdio.h>
#include <iostream>
#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "skybox.h"


class Node {
public:
    glm::mat4 M;
    bool isVisible = true;
    
    void walk();
    virtual void draw(GLuint shaderProgram, glm::mat4) = 0;
    virtual void update(glm::mat4 C) = 0;
};

#endif
