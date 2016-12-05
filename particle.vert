#version 330 core
layout (location = 0) in vec3 vertex;

out vec4 ParticleColor;

uniform mat4 projection;
uniform vec3 offset;
uniform vec4 color;

void main()
{
    float scale = 10.0f;
    ParticleColor = color;
    gl_Position = projection * vec4((vertex * scale) + offset, 1.0);
}