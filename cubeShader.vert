#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

uniform mat4 MVP;
uniform mat4 lightMVP;
uniform mat4 biasMatrix;

out vec4 ShadowCoord;

void main()
{
    mat4 depthBiasMVP = biasMatrix * lightMVP;
    gl_Position = MVP * vec4(position, 1.0f);
    ShadowCoord = depthBiasMVP * vec4(position, 1.0f);
}
