//
//  skybox.h
//  project2
//
//  Created by Reno Yin on 26/10/2016.
//  Copyright © 2016 Reno Yin. All rights reserved.
//

#ifndef skybox_h
#define skybox_h


#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cstdio>
#include <iostream>
#include "glm/ext.hpp"

class skybox {
public:
    skybox(std::vector<const GLchar*>);
    ~skybox();
    
    GLuint textureID;
    glm::mat4 toWorld;
    GLuint skyboxVBO, skyboxVAO;
    float angle;
    
    void draw(GLuint);
    GLuint loadTexture(std::vector<const GLchar*> );
    unsigned char* loadPPM(const char* filename, int& width, int& height);
};

#endif /* skybox_h */
