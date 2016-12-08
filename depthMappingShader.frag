#version 330 core

out vec4 color;
in vec2 TexCoords;

uniform sampler2D depthMap;

void main()
{
    float depthValue = texture(depthMap, TexCoords).r;
    if (depthValue != 1) depthValue *= 2;
    //if(depthValue < 0.0f) depthValue = 0.0f;
    color = vec4(vec3(depthValue), 1.0);
}
