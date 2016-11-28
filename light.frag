#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 Normal;
in vec3 FragPos;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirectionLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 direction;
    vec3 position;
    float cutoff;
    float exponent;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

uniform DirectionLight dirLight;
uniform PointLight pointLight;
//uniform SpotLight spotLight;
uniform Material material;
uniform vec3 viewPos;
uniform int mode = 2;
uniform vec3 colorin;


// function prototypes
vec3 getColor_directional(DirectionLight light, vec3 normal, vec3 viewDir);
vec3 getColor_point(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 getColor_spot(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{
    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(1.0f);
    
    if (mode == 1) {
        result = getColor_directional(dirLight, norm, viewDir);
    }
    else if (mode == 2) {
        result = getColor_point(pointLight, norm, FragPos, viewDir);
    }
    //else if (mode == 3) {
    //    result = getColor_spot(spotLight, norm, FragPos, viewDir);
    //}
    else if (mode == 4) {
        result = vec3(0.0f);
    }
    else {
        result = colorin;
    }
    color = vec4(result, 1.0);
}


vec3 getColor_directional(DirectionLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    
    // ambient
    vec3 ambient = light.ambient * material.ambient;
    
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
    
    return (ambient + diffuse + specular);
}


vec3 getColor_point(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    
    // ambient
    vec3 ambient = light.ambient * material.ambient;
    
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
    
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    
    // apply attenuation on lighting
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

vec3 getColor_spot(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    
    // ambient
    vec3 ambient = light.ambient * material.ambient;
    
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
    
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.quadratic * distance * distance);
    
    float threshold = dot(lightDir, normalize(-light.direction));
    float cosine = cos(light.cutoff);
    float intensity = 0;
    if(threshold > cosine) {
        intensity = pow(threshold, light.exponent);
    }
    //    float epsilon = light.cutOff - light.outerCutOff;
    //    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    // Combine results
//    ambient = light.ambient * material.ambient;
//    diffuse = light.diffuse * diff * material.diffuse;
//    specular = light.specular * spec * material.specular;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return (ambient + diffuse + specular);
    
    
}

