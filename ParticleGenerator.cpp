//
//  Particle.cpp
//  cse167Final
//
//  Created by Cyrus on 12/5/16.
//  Copyright © 2016 Cyrus. All rights reserved.
//

#include "ParticleGenerator.hpp"
#include "Window.h"

ParticleGenerator::ParticleGenerator(GLuint amount)
: amount(amount)
{
    this->init();
}

void ParticleGenerator::Update(GLfloat dt, GLuint newParticles, glm::vec3 offset)
{
    // Add new particles
    for (GLuint i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle], offset);
    }
    // Update all particles
    for (GLuint i = 0; i < this->amount; ++i)
    {
        Particle &p = this->particles[i];
        p.Life -= dt; // reduce life
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            p.Position -= vec3(p.Velocity) * dt;
            p.Color.a -= dt * 2.5;
        }
    }
}

// Render all particles
void ParticleGenerator::Draw(GLuint shaderProgram)
{
    
    glBindVertexArray(this->VAO);
    glPointSize((2.0f));
    glm::mat4 mvp = Window::P * Window::V;
    GLuint mvpUniform = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &mvp[0][0]);
    // Use additive blending to give it a 'glow' effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    for (int i=0; i<amount; i++)
    {
        if (particles[i].Life > 0.0f)
        {
            //            this->shader.SetVector3f("offset", particle.Position);
            //            this->shader.SetVector4f("color", particle.Color);
            
            glUniform3f(glGetUniformLocation(shaderProgram, "offset"), particles[i].Position.x, particles[i].Position.y, particles[i].Position.z);
            glUniform4f(glGetUniformLocation(shaderProgram, "color"), particles[i].Color.x, particles[i].Color.y, particles[i].Color.z, 1.0f);
            
            glDrawArrays(GL_POINTS, i, 1);
            
        }
        
    }
    glBindVertexArray(0);
    // Don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
    for (GLuint i = 0; i < this->amount; ++i)
        this->particlesPos.push_back(vec3(0.0f));
    // Set up mesh and attribute properties
    GLuint VBO;
    GLfloat particle_quad[] = {
        0.0f,0.0f,0.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    // Fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, amount*sizeof(vec3), &particlesPos[0], GL_STATIC_DRAW);
    // Set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
    
    // Create this->amount default particle instances
    for (GLuint i = 0; i < this->amount; ++i)
        this->particles.push_back(Particle());
    
}

// Stores the index of the last particle used (for quick access to next dead particle)
GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle()
{
    // First search from last used particle, this will usually return almost instantly
    for (GLuint i = lastUsedParticle; i < this->amount; ++i){
        if (this->particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // Otherwise, do a linear search
    for (GLuint i = 0; i < lastUsedParticle; ++i){
        if (this->particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle &particle, glm::vec3 offset)
{
    GLfloat randomx = ((rand() % 100) - 60) / 10.0f-2;
    GLfloat randomy = ((rand() % 100) - 60) / 10.0f-2;
    GLfloat randomz = ((rand() % 100) - 60) / 10.0f-2;
    //cout<<"random="<<random<<endl;
    GLfloat rColorx = 0.5 + ((rand() % 100) / 100.0f);
    GLfloat rColory = 0.5 + ((rand() % 100) / 100.0f);
    GLfloat rColorz = 0.5 + ((rand() % 100) / 100.0f);
    vec3 color = normalize(vec3(rColorx,rColory,rColorz));
    
    vec3 dirNorm = normalize(Window::direction);
    
    //cout<<"color="<<rColor<<endl;
    particle.Position = Window::spherePos+vec3(randomx, randomy, randomz) - dirNorm*offset;
    //cout<<"position="<<particle.Position.x<<","<<particle.Position.y<<","<<particle.Position.z<<endl;
    particle.Color = glm::vec4(color, 1.0f);
    particle.Life = 1.0f;
    particle.Velocity = normalize(Window::direction) * Window::speed;
    //particle.Velocity = vec3(0.1f);
}