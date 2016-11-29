#version 330 core
// This is a sample fragment shader.
//
// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
uniform vec3 colorin;
// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
    color = vec4(colorin, sampleExtraOutput);
}

//struct Material {
//    vec3 diffuse;
//    vec3 specular;
//    vec3 ambient;
//    float shininess;
//};
//
//struct DirLight {
//    //bool on;
//    vec3 direction;
//    
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//};
//
//struct PointLight {
//    //bool on;
//    vec3 position;
//    
//    float constant;
//    float linear;
//    float quadratic;
//    
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//};
//
//struct SpotLight {
//    //bool on;
//    vec3 position;
//    vec3 direction;
//    float cutOff;
//    float exponent;
//    
//    float constant;
//    float linear;
//    float quadratic;
//    
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//};
//
//
//in vec3 FragPos;
//in vec3 Normal;
//
//
//out vec4 color;
//
//uniform vec3 viewPos;
//uniform DirLight dirLight;
//uniform PointLight pointLight;
//uniform SpotLight spotLight;
//uniform Material material;
//uniform int lightType = 0;
//
//// Function prototypes
//vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
//vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
//void main()
//{
//    // Properties
//    vec3 norm = normalize(Normal);
//    vec3 viewDir = normalize(viewPos - FragPos);
//    vec3 result = vec3(1.0f);
//    // == ======================================
//    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
//    // For each phase, a calculate function is defined that calculates the corresponding color
//    // per lamp. In the main() function we take all the calculated colors and sum them up for
//    // this fragment's final color.
//    // == ======================================
//    // Phase 1: Directional lighting
//    if(lightType==1) {
//        result = CalcDirLight(dirLight, norm, viewDir);
//        //result = -dirLight.direction;
//        //result = material.ambient;
//    }
//    else if(lightType==2) {
//        result = CalcPointLight(pointLight, norm, FragPos, viewDir);
//        //result = FragPos;
//        //result = material.specular;
//    }
//    else if(lightType==3) {
//        result = CalcSpotLight(spotLight, norm, FragPos, viewDir);
//        //result = pointLight.diffuse;
//        //result = material.diffuse;
//    }
//    else {
//        result = norm;
//    }
//    color = vec4(result, 1.0);
//}
//
//// Calculates the color when using a directional light.
//vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
//{
//    vec3 lightDir = normalize(-light.direction);
//    // Diffuse shading
//    float diff = max(dot(normal, lightDir), 0.0);
//    // Specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    // Combine results
//    vec3 ambient = light.ambient * material.ambient;
//    vec3 diffuse = light.diffuse * diff * material.diffuse;
//    vec3 specular = light.specular * spec * material.specular;
//    return (ambient + diffuse + specular);
//    
//    
//}
//
//// Calculates the color when using a point light.
//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
//{
//    vec3 lightDir = normalize(light.position - fragPos);
//    // Diffuse shading
//    float diff = max(dot(normal, lightDir), 0.0);
//    // Specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    // Attenuation
//    float distance = length(light.position - fragPos);
//    float attenuation = 1.0f / (light.quadratic * (distance * distance));
//    // Combine results
//    vec3 ambient = light.ambient * material.ambient;
//    vec3 diffuse = light.diffuse * diff * material.diffuse;
//    vec3 specular = light.specular * spec * material.specular;
//    ambient *= attenuation;
//    diffuse *= attenuation;
//    specular *= attenuation;
//    return (ambient + diffuse + specular);
//    //return lightDir;
//}
//
//// Calculates the color when using a spot light.
//vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
//{
//    vec3 lightDir = normalize(light.position - fragPos);
//    // Diffuse shading
//    float diff = max(dot(normal, lightDir), 0.0);
//    // Specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    // Attenuation
//    float distance = length(light.position - fragPos);
//    float attenuation = 1.0f / (light.quadratic * (distance * distance));
//    // Spotlight intensity
//    
//    float threshold = dot(lightDir, normalize(-light.direction));
//    float cosine = cos(light.cutOff);
//    float intensity = 0;
//    if(threshold > cosine) {
//        intensity = pow(threshold, light.exponent);
//    }
////    float epsilon = light.cutOff - light.outerCutOff;
////    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
//    // Combine results
//    vec3 ambient = light.ambient * material.ambient;
//    vec3 diffuse = light.diffuse * diff * material.diffuse;
//    vec3 specular = light.specular * spec * material.specular;
//    ambient *= attenuation * intensity;
//    diffuse *= attenuation * intensity;
//    specular *= attenuation * intensity;
//    return (ambient + diffuse + specular);
//}