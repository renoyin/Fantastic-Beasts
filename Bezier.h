//
//  Bezier.h
//  project2
//
//  Created by Reno Yin on 11/11/2016.
//  Copyright © 2016 Reno Yin. All rights reserved.
//

#ifndef Bezier_h
#define Bezier_h

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include "Window.h"

class Bezier {
public:
    Bezier();
    ~Bezier();
    
    int t;
    float maxY = INT_MIN;
    glm::vec3 p[40];
    glm::mat4 toWorld;
    std::vector<glm::vec3> points;
    GLuint VAO, VBO, CPVAO, CPVBO;
    
    glm::vec3 getCurvePoint(glm::vec3, glm::vec3, glm::vec3, glm::vec3, float);
    void calcMidPoints(glm::vec3, glm::vec3, glm::vec3, glm::vec3, int);
    void draw(GLuint);
    void drawControlPoints(GLuint);
    void update();
};

#endif /* Bezier_h */
