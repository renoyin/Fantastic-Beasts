#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

uniform mat4 shadowMappingMVPMatrix;

out vec2 TexCoords;

void main()
{
    gl_Position = shadowMappingMVPMatrix * vec4(position, 1.0f);
    TexCoords = texCoord;
}
