#version 330 core
out vec4 FragColor;

in VS_OUT
{
    vec3 FragPos; 
    vec3 Normal; 
    vec2 TexCoords; 
} fs_in;

uniform sampler2D floorTexture; 
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform bool isBlinn;
uniform bool isGamma;

void main()
{
    vec3 color = texture(floorTexture, fs_in.TexCoords).rgb; 

    // ambient
    vec3 ambient = 0.05 * color; 

    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos); 
    vec3 normal = normalize(fs_in.Normal); 
    float cosineTerm = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = color * cosineTerm;

    //specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos); 
    vec3 reflectDir = reflect(-lightDir, normal); 
    float spec = 0.0; 
    if(isBlinn){
        vec3 halfwayDir = normalize(lightDir + viewDir); 
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    }else{
        spec = pow(max(dot(normal, reflectDir), 0.0), 32.0);
    }
    vec3 specular = vec3(0.3) * spec;

    // attenuation
    float dist = length(lightPos - fs_in.FragPos);
    float attenuation = 1.0 / (isGamma ? dist * dist : dist);
    diffuse *= attenuation;
    specular *= attenuation;

    // final result
    vec3 result = diffuse + specular;
    float gamma = 2.2;
    if(isGamma){
        result = pow(result, vec3(1.0 / gamma));
    }

    FragColor = vec4(result, 1.0f);
}