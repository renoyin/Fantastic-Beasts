#include "Sphere.hpp"
#include "Window.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace std;

Sphere::Sphere(float radius, unsigned int rings, unsigned int sectors)
{

    float const R = 1./(float)(rings-1);
    float const S = 1./(float)(sectors-1);
    int r, s;
    
    vertices.resize(rings * sectors * 3);
    normals.resize(rings * sectors * 3);
    texcoords.resize(rings * sectors * 2);
    std::vector<GLfloat>::iterator v = vertices.begin();
    std::vector<GLfloat>::iterator n = normals.begin();
    std::vector<GLfloat>::iterator t = texcoords.begin();
    for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
        float const y = sin( -M_PI_2 + M_PI * r * R );
        float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
        float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );
        
        *t++ = s*S;
        *t++ = r*R;
        
        *v++ = x * radius;
        *v++ = y * radius;
        *v++ = z * radius;
        
        *n++ = x;
        *n++ = y;
        *n++ = z;
    }
    
    indices.resize(rings * sectors * 6);
    std::vector<unsigned int>::iterator i = indices.begin();
    for(r = 0; r < rings-1; r++) for(s = 0; s < sectors-1; s++) {
        *i++ = r * sectors + s;
        *i++ = (r + 1) * sectors + (s + 1);
        *i++ = r * sectors + (s + 1);
        
        *i++ = r * sectors + s;
        *i++ = (r + 1) * sectors + s;
        *i++ = (r + 1) * sectors + (s + 1);
//        *i++ = r * sectors + s;
//        *i++ = r * sectors + (s+1);
//        *i++ = (r+1) * sectors + (s+1);
//        *i++ = (r+1) * sectors + s;
    }
    
    //cout<< indices[34]<<endl;
    //cout<< vertices.size() << endl;
    
    // Create array object and buffers. Remember to delete your buffers when the object is destroyed!
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO);

    // Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
    // Consider the VAO as a container for all your buffers.
    glBindVertexArray(VAO);
    // Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
    // you want to draw, such as vertices, normals, colors, etc.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
    // the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
                          3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
                          GL_FLOAT, // What type these components are
                          GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                          3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                          (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    // glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
    // the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*normals.size(), &normals[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, // This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
                          3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
                          GL_FLOAT, // What type these components are
                          GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                          3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                          (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    
    // We've sent the vertex data over to OpenGL, but there's still something missing.
    // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);
    
 
    // Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind the VAO now so we don't accidentally tamper with it.
    // NOTE: You must NEVER unbind the element array buffer associated with a VAO!
    glBindVertexArray(0);
    
}

Sphere::~Sphere()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}


void Sphere::draw(GLuint shaderProgram, glm::mat4 C)
{
    if(ifDraw) {
        //toWorld = C*toWorld;
        glm::mat4 mvp = Window::P * Window::V * C * toWorld;
        glm::mat4 modelview = Window::V *C * toWorld;
        // We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
        // Consequently, we need to forward the projection, view, and model matrices to the shader programs
        // Get the location of the uniform variables "projection" and "modelview"
        //GLuint mvpUniform = glGetUniformLocation(shaderProgram, "MVP");
        //GLuint modelUniform = glGetUniformLocation(shaderProgram, "model");
        uProjection = glGetUniformLocation(shaderProgram, "projection");
        uModelview = glGetUniformLocation(shaderProgram, "modelview");
        glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
        glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
        // Now send these values to the shader program
        //glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &mvp[0][0]);
        //glUniformMatrix4fv(modelUniform, 1, GL_FALSE, &toWorld[0][0]);
        
        glUniform3f(glGetUniformLocation(shaderProgram, "colorin"), color.x, color.y, color.z);
        

        // draw sphere
        glBindVertexArray(VAO);
        if(solid) {
            glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, 0);
        }
        else{
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, 0);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            
        }
        glBindVertexArray(0);
    }
    
  
}

void Sphere::update(glm::mat4 C) {
    Geode::update(C);
}
