#ifndef Sphere_hpp
#define Sphere_hpp
#define _USE_MATH_DEFINES
#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Geode.h"
#include <time.h>
class Sphere: public Geode
{

    
public:
    Sphere(float radius, unsigned int rings, unsigned int sectors);
    ~Sphere();
    
    
    void draw(GLuint, glm::mat4);
    void update(glm::mat4 C);
    void spin(float);
    
    // These variables are needed for the shader program
    GLuint VBO, VBO2, VAO, EBO;
    GLuint uProjection, uModelview;
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<unsigned int> indices;
    int numsToDraw;
    bool isShadowMapping = false;
};
#endif
