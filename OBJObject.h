#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
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
#include "Window.h"


class OBJObject
{
private:
    std::vector<unsigned int> indices;
    //std::vector<glm::vec3> vertices;
    //std::vector<glm::vec3> normals;    
    glm::vec3 center;
    glm::mat4 toWorldBackup;

public:
	OBJObject(const char* filepath);
    float size;
    float angle;
    glm::mat4 toWorld;
    std::vector<GLfloat> vertices;
    std::vector<glm::vec3> normals;
    GLuint VBO, VAO, EBO, VBO2;

	void parse(const char* filepath);
	void draw(GLuint);
    void update();
    void spin(float deg);
    void updatePos(glm::vec3 vec);
    void updateScl(float);
    void orbitZ(float);
    void resetObj();
    glm::mat4 getToWorld();
    std::vector<GLfloat> getVertices();
    std::vector<glm::vec3> getNormals();
};

#endif
