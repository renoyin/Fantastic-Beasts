#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 lightMVPMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightMVPMatrix * model * vec4(position, 1.0f);
}
