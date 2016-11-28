#define _CRT_SECURE_NO_WARNINGS
#include "OBJObject.h"
#include "Window.h"
#include <string>
#include <iostream>
using namespace std;
OBJObject::OBJObject(const char *filepath)
{
	//toWorld = glm::mat4(1.0f);
    this->toWorld = glm::mat4(1.0f);
    this->size = 1.0f;
    this->angle = 0.0f;
    this->center = glm::vec3(0.0f, 0.0f, 0.0f);
    parse(filepath);
    
    
    // Create array object and buffers. Remember to delete your buffers when the object is destroyed!
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO2);
    
    // Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
    // Consider the VAO as a container for all your buffers.
    glBindVertexArray(VAO);
    
    // Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
    // you want to draw, such as vertices, normals, colors, etc.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
    // the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
                          3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
                          GL_FLOAT, // What type these components are
                          GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                          3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                          (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    // Enable the usage of layout location 0 (check the vertex shader to see what this is)
    glEnableVertexAttribArray(0);
    
    // We've sent the vertex data over to OpenGL, but there's still something missing.
    // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*normals.size(), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(GLfloat),
                          (GLvoid*)0);
    glEnableVertexAttribArray(1);
    
    // Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // Unbind the VAO now so we don't accidentally tamper with it.
    // NOTE: You must NEVER unbind the element array buffer associated with a VAO!
    glBindVertexArray(0);
}

void OBJObject::parse(const char *filepath) 
{
	//TODO parse the OBJ file
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
    
    FILE* fp;   // file pointer
    float x, y, z;  // vertex coordinates
    float r, g, b;  // vertex color
    char c1, c2; // characters read from file
    unsigned int i1, i2, i3;
    
    fp = fopen(filepath, "rb");
    // just in case the file can't be found or is corrupt
    if (fp == NULL)
    {
        std::cerr << "error loading file" << std::endl;
        std::cerr << errno << std::endl;
        exit(-1);
    }
    
    glm::vec3 vertex;
    glm::vec3 vertexNorm;

    do
    {
        c1 = fgetc(fp);
        if (c1 == EOF) break;
        
        if (c1 == 'v')
        {
            c2 = fgetc(fp);
            if (c2 == ' ')
            {
                fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
                vertices.push_back((GLfloat)x);
                vertices.push_back((GLfloat)y);
                vertices.push_back((GLfloat)z);
            }
            else if (c2 == 'n')
            {
                fscanf(fp, "%f %f %f", &x, &y, &z);
                vertexNorm = glm::vec3((GLfloat)x, (GLfloat)y, (GLfloat)z);
                vertexNorm = glm::normalize(vertexNorm);
                vertexNorm = (vertexNorm + glm::vec3(1.0f, 1.0f, 1.0f)) / 2.0f;
                //std::cout << glm::to_string(vertexNorm) << std::endl;
                normals.push_back(vertexNorm);
            }
        }
        else if (c1 == 'f')
        {
            c2 = fgetc(fp);
            if (c2 == ' ') {
                fscanf(fp, "%u//%*u %u//%*u %u//%*u", &i1, &i2, &i3);
                indices.push_back(i1-1);
                indices.push_back(i2-1);
                indices.push_back(i3-1);
            }
        }
    } while (true);
    fclose(fp);
    
//    // Move obj to center
//    GLfloat min, max;
//    float scale = 0.0f;
//    for (int offset = 0; offset < 3; offset++)
//    {
//        min = vertices[offset];
//        max = min;
//        
//        for (int i = offset; i < vertices.size(); i+=3)
//        {
//            if (vertices[i] < min) min = vertices[i];
//            if (vertices[i] > max) max = vertices[i];
//        }
//        center[offset] = (max + min) / 2;
//        if (max - min > scale) scale = max - min;
//    }
//    this->toWorld = glm::translate(glm::mat4(1.0f), -center) * this->toWorld;
//    
//    // Scale obj to fit in a 2x2x2 cube
//    scale = 2.0f / scale;
//    this->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale)) * this->toWorld;
//    this->toWorldBackup = this->toWorld;
}

void OBJObject::draw(GLuint shaderProgram)
{
    glm::mat4 mvpMatrix = Window::P * Window::V * this->toWorld;
    
    GLint mvp = glGetUniformLocation(shaderProgram, "MVP");
    GLint model = glGetUniformLocation(shaderProgram, "model");
    
    // Now send these values to the shader program
    glUniformMatrix4fv(mvp, 1, GL_FALSE, &mvpMatrix[0][0]);
    glUniformMatrix4fv(model, 1, GL_FALSE, &this->toWorld[0][0]);
    
    // Now draw the cube. We simply need to bind the VAO associated with it.
    glBindVertexArray(VAO);
    
    // Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
}

void OBJObject::update()
{
    //spin(1.0f);
}

void OBJObject::spin(float deg)
{
    this->angle += deg;
    if (this->angle > 360.0f || this->angle < -360.0f) this->angle = 0.0f;
    this->toWorld = (this->toWorld)*(glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)));
}

void OBJObject::updatePos(glm::vec3 vec)
{
    //this->totalMove += vec;
    glm::mat4 move = glm::translate(glm::mat4(1.0f), vec);
    this->toWorld = move * this->toWorld;
}

void OBJObject::updateScl(float factor)
{
    //this->deltaScl *= factor;
    toWorld = glm::scale(toWorld, glm::vec3(factor,factor,factor));
}

void OBJObject::orbitZ(float deg)
{
    glm::mat4 orbit = glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
    this->toWorld = orbit * this->toWorld;
}

void OBJObject::resetObj()
{
    this->angle = 0.0f;
    this->toWorld = this->toWorldBackup;
    this->size = 1.0f;
}

glm::mat4 OBJObject::getToWorld()
{
    return this->toWorld;
}

std::vector<GLfloat> OBJObject::getVertices()
{
    return this->vertices;
}

std::vector<glm::vec3> OBJObject::getNormals()
{
    return this->normals;
}


