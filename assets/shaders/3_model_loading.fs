#version 330 core

#define AMBIENT_INFLUENCE 0.2
#define DIFFUSE_INFLUENCE 1
#define SPECULAR_INFLUENCE 0.5

struct Material{

    // Note: sampler2D is a so called opaque type which means 
    // it be can't instantiated, but only defined as uniforms. 
    // If the struct would be instantiated other than as a uniform 
    // (like a function parameter) GLSL could throw strange errors; 
    // the same thus applies to any struct holding such opaque types.
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

    float shininess;
};
uniform Material material;

struct DirectionalLight{ 
    vec3 direction; // directional vector replaces position vector
    vec3 value;
};
uniform DirectionalLight dirLight;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);

uniform vec3 viewPos;

in vec3 fragPos; 
in vec3 normal; 
in vec2 texCoords;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(normal); 
    vec3 viewDir = normalize(fragPos - viewPos);

    // output
    // ----------
    vec3 outputColor = CalcDirLight(dirLight, norm, viewDir);
    // vec3 outputColor = texture(material.texture_diffuse1, texCoords).rgb;
    // vec3 outputColor = vec3(texCoords.x, texCoords.y, 0.0);
    FragColor = vec4(outputColor, 1.0);
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    // directional light caster
    vec3 lightDir = normalize(-light.direction);
    
    // ambient color
    // ------------------
    vec3 ambientColor = texture(material.texture_diffuse1, texCoords).rgb * (light.value * AMBIENT_INFLUENCE);
    
    // diffuse color
    // ------------------
    float cosineTerm = max(dot(normal, lightDir), 0.0);
    vec3 diffuseColor = texture(material.texture_diffuse1, texCoords).rgb * cosineTerm * (light.value * DIFFUSE_INFLUENCE);
    
    // specular color
    // ------------------
    vec3 reflectDir = reflect(-lightDir, normal); 

    // Note: Calculate the angular distance between this reflection vector and the view direction.
    // The closer the angle between them, the greater the impact of the specular light.
    float specularIntensity = pow(max(dot(reflectDir, viewDir), 0.0), 32.0);
    vec3 specularColor = texture(material.texture_specular1, texCoords).rgb * specularIntensity * (light.value * SPECULAR_INFLUENCE);

    // output
    // ------------------
    // return ambientColor + diffuseColor + specularColor;
    return vec4(texture(material.texture_diffuse1, texCoords).rgb, 1.0);
}