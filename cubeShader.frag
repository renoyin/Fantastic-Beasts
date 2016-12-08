#version 330 core

out vec4 color;
in vec4 ShadowCoord;

uniform sampler2D shadowMap;

void main()
{
    float visibility = 1.0f;
    if (texture(shadowMap, ShadowCoord.xy).r < (ShadowCoord.z-0.05)) {
        visibility = 0.1;
    }
    //vec3 MaterialAmbientColor = vec3(0.25, 0.2, 0.075);
    //vec3 MaterialDiffuseColor = vec3(0.75, 0.60, 0.23);
    //vec3 MaterialSpecularColor = vec3(0.63, 0.55, 0.37);
    vec3 MaterialAmbientColor = vec3(0.5f);
    vec3 MaterialDiffuseColor = vec3(0.7f);
    vec3 MaterialSpecularColor = vec3(0.2f);
    float LightColor = 1.0f;
    vec3 shadow = MaterialAmbientColor +
            visibility * MaterialDiffuseColor * LightColor;
    color = vec4(shadow, 1.0f);
}
