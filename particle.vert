#version 330 core
layout (location = 0) in vec3 position;

out vec4 ParticleColor;

uniform mat4 projection;
uniform vec3 offset;
uniform vec4 color;

void main()
{
    float scale = 1.0f;
    ParticleColor = color;
    gl_Position = projection * vec4(offset, 1.0);
    //gl_Position = projection * vec4(position.x, position.y, position.z, 1.0f);
}