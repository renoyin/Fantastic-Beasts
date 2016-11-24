//
//  Bezier.cpp
//  project2
//
//  Created by Reno Yin on 11/11/2016.
//  Copyright © 2016 Reno Yin. All rights reserved.
//

#include "Bezier.h"

Bezier::Bezier() {
    p[0] = glm::vec3(-30,35,0); p[1] = glm::vec3(-35,30,0); p[2] = glm::vec3(-40,25,0); p[3] = glm::vec3(-50,15,0);
    p[4] = glm::vec3(-50,15,0); p[5] = glm::vec3(-60,5,0); p[6] = glm::vec3(-50,10,0); p[7] = glm::vec3(-45,5,0);
    p[8] = glm::vec3(-45,5,0); p[9] = glm::vec3(-40,0,0); p[10] = glm::vec3(-40,-5,0); p[11] = glm::vec3(-35,-10,0);
    p[12] = glm::vec3(-35,-10,0); p[13] = glm::vec3(-30,-15,0); p[14] = glm::vec3(-25,-20,0); p[15] = glm::vec3(-20,-25,0);
    p[16] = glm::vec3(-20,-25,0); p[17] = glm::vec3(-15,-30,0); p[18] = glm::vec3(-25,-35,0); p[19] = glm::vec3(-20,-40,0);
    p[20] = glm::vec3(-20,-40,0); p[21] = glm::vec3(-15,-45,0); p[22] = glm::vec3(0,-45,0); p[23] = glm::vec3(10,-35,0);
    p[24] = glm::vec3(10,-35,0); p[25] = glm::vec3(20,-25,0); p[26] = glm::vec3(40,-50,0); p[27] = glm::vec3(50,-40,0);
    p[28] = glm::vec3(50,-40,0); p[29] = glm::vec3(60,-30,0); p[30] = glm::vec3(45,5,0); p[31] = glm::vec3(40,15,0);
    p[32] = glm::vec3(40,15,0); p[33] = glm::vec3(35,25,0); p[34] = glm::vec3(25,25,0); p[35] = glm::vec3(15,20,0);
    p[36] = glm::vec3(15,20,0); p[37] = glm::vec3(5,15,0); p[38] = glm::vec3(-25,40,0); p[39] = glm::vec3(-30,35,0);
    
    
    this->toWorld = glm::mat4(1.0f);
    calcMidPoints(p[0], p[1], p[2], p[3], 0);
    calcMidPoints(p[4], p[5], p[6], p[7], 1);
    calcMidPoints(p[7], p[8], p[9], p[11], 2);
    calcMidPoints(p[12], p[13], p[14], p[15], 3);
    calcMidPoints(p[16], p[17], p[18], p[19], 4);
    calcMidPoints(p[20], p[21], p[22], p[23], 5);
    calcMidPoints(p[24], p[25], p[26], p[27], 6);
    calcMidPoints(p[28], p[29], p[30], p[31], 7);
    calcMidPoints(p[32], p[33], p[34], p[35], 8);
    calcMidPoints(p[36], p[37], p[38], p[39], 9);
    
    
    
    // VAO and VBO for Track
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(points[0]), &points[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    
    // VAO and VBO for control points
    glGenVertexArrays(1, &CPVAO);
    glGenBuffers(1, &CPVBO);
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(CPVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, CPVBO);
    glBufferData(GL_ARRAY_BUFFER, 40 * sizeof(p[0]), &p[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
}

Bezier::~Bezier() {
    
}

glm::vec3 Bezier::getCurvePoint(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float t) {
    glm::vec3 point;
    point.x = (pow((1 - t), 3.0) * p0.x) + (3 * pow((1 - t), 2) * t * p1.x) + (3 * (1 - t) * t * t * p2.x) + (pow(t, 3) * p3.x);
    point.y = (pow((1 - t), 3.0) * p0.y) + (3 * pow((1 - t), 2) * t * p1.y) + (3 * (1 - t) * t * t * p2.y) + (pow(t, 3) * p3.y);
    point.z = (pow((1 - t), 3.0) * p0.z) + (3 * pow((1 - t), 2) * t * p1.z) + (3 * (1 - t) * t * t * p2.z) + (pow(t, 3) * p3.z);
    return point;
}


void Bezier::calcMidPoints(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int segIndex) {
    int numSeg = 150;
    
    for (int i = 0; i < numSeg; i++) {
        float n = (float)i / (float)(numSeg - 1);
        glm::vec3 temp = getCurvePoint(p0, p1, p2, p3, n);
        this->points.push_back(temp);
        if(maxY < temp.y) {
            maxY = temp.y;
            t = 150 * segIndex + i;
        }
    }
}

void Bezier::draw(GLuint shaderProgram) {
    glm::mat4 MVP = Window::P * Window::V * this->toWorld;
    
    // transformation matrix
    GLuint MVPuniform = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(MVPuniform, 1, GL_FALSE, &MVP[0][0]);
    
    // color
    GLfloat color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLuint colorUniform = glGetUniformLocation(shaderProgram, "curveColor");
    glUniform4fv(colorUniform, 1, &color[0]);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_STRIP, 0, points.size());
    glBindVertexArray(0);
    
    drawControlPoints(shaderProgram);
}

void Bezier::drawControlPoints(GLuint shaderProgram) {
    
    GLfloat codeUniform = glGetUniformLocation(shaderProgram, "code");
    GLuint colorUniform = glGetUniformLocation(shaderProgram, "curveColor");
    glBindVertexArray(CPVAO);
    glPointSize(9);
    
    // Yellow lines
    GLfloat color[4] = {1.0f, 1.0f, 0.0f, 1.0f};
    glUniform4fv(colorUniform, 1, &color[0]);
    for (int i = 0; i < 40; i+=2) {
        glDrawArrays(GL_LINE_STRIP, i, 2);
    }
    
    // Red interpolating points
    color[1] = 0.0f;
    glUniform4fv(colorUniform, 1, &color[0]);
    for (int i = 0; i < 40; i+=4) {
        glUniform1ui(codeUniform, i+1);
        glDrawArrays(GL_POINTS, i, 1);
    }
    
    // Green control Points
    color[0] = 0.0f;
    color[1] = 1.0f;
    glUniform4fv(colorUniform, 1, &color[0]);
    for (int i = 1; i < 40; i+=4) {
        glUniform1ui(codeUniform, i+1);
        glDrawArrays(GL_POINTS, i, 1);
    }
    for (int i = 2; i < 40; i+=4) {
        glUniform1ui(codeUniform, i+1);
        glDrawArrays(GL_POINTS, i, 1);
    }
    
    glBindVertexArray(0);
}

void Bezier::update() {
    // VAO and VBO for control points
    glGenVertexArrays(1, &CPVAO);
    glGenBuffers(1, &CPVBO);
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(CPVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, CPVBO);
    glBufferData(GL_ARRAY_BUFFER, 40 * sizeof(p[0]), &p[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    
    points.clear();
    maxY = INT_MIN;
    calcMidPoints(p[0], p[1], p[2], p[3], 0);
    calcMidPoints(p[4], p[5], p[6], p[7], 1);
    calcMidPoints(p[7], p[8], p[9], p[11], 2);
    calcMidPoints(p[12], p[13], p[14], p[15], 3);
    calcMidPoints(p[16], p[17], p[18], p[19], 4);
    calcMidPoints(p[20], p[21], p[22], p[23], 5);
    calcMidPoints(p[24], p[25], p[26], p[27], 6);
    calcMidPoints(p[28], p[29], p[30], p[31], 7);
    calcMidPoints(p[32], p[33], p[34], p[35], 8);
    calcMidPoints(p[36], p[37], p[38], p[39], 9);
    
    
    // VAO and VBO for Track
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(points[0]), &points[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
