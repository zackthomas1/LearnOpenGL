#version 330 core

in vec3 normal;
in vec3 fragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    // ambient color
    float ambientIntensity = 0.1f;
    vec3 ambient = lightColor * ambientIntensity;

    // diffuse color
    vec3 norm = normalize(normal); 
    vec3 lightDir = normalize(lightPos - fragPos);
    float cosineTerm = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * cosineTerm;

    // specular color
    float specularIntensity = 0.5f;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(reflectDir, viewDir), 0.0),128);
    vec3 specular = specularIntensity * spec * lightColor;

    // output
    FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
}