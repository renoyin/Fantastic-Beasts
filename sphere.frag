#version 330 core
uniform vec3 colorin;
out vec4 color;

void main()
{
    color = vec4(colorin, 1.0f);
}
