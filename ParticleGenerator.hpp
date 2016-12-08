//
//  Particle.hpp
//  cse167Final
//
//  Created by Cyrus on 12/5/16.
//  Copyright © 2016 Cyrus. All rights reserved.
//

#ifndef Particle_hpp
#define Particle_hpp

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
#include "Geode.h"
#include <vector>


// Represents a single particle and its state
struct Particle {
    glm::vec3 Position, Velocity;
    glm::vec4 Color;
    GLfloat Life;
    
    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(1.0f) { }
};


// ParticleGenerator acts as a container for rendering a large number of
// particles by repeatedly spawning and updating particles and killing
// them after a given amount of time.
class ParticleGenerator
{
public:
    std::vector<glm::vec3> particlesPos;
    glm::vec3 p[40];
    // Constructor
    ParticleGenerator(GLuint amount);
    // Update all particles
    void Update(GLfloat dt, GLuint newParticles, glm::vec3 offset = glm::vec3(0.0f));
    // Render all particles
    void Draw(GLuint shaderProgram);
private:
    // State
    
    std::vector<Particle> particles;
    GLuint amount;
    // Render state
    GLuint VAO, VBO, PBO, CBO;
    // Initializes buffer and vertex attributes
    void init();
    // Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    GLuint firstUnusedParticle();
    // Respawns particle
    void respawnParticle(Particle &particle, glm::vec3 offset = glm::vec3(0.0f));
};

#endif