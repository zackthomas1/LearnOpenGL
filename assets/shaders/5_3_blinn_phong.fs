#version 330 core
out vec4 FragColor;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D floorTexture;
uniform sampler2D shadowMapTexture;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform bool isBlinn;
uniform bool isGamma;

float ShadowCalculation(vec4 FragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fs_in.FragPosLightSpace.xyz / fs_in.FragPosLightSpace.w;
    
    // Transforms NDC [-1,1] into depth map range [0,1]
    projCoords = projCoords * 0.5 + 0.5;

    // Can sample depth map as the resulting [0,1] coordinates directly 
    // correspond to transformed NDC coordinates from first render pass.
    // This provides the closest depth from light's point of view.
    float closestDepth = texture(shadowMapTexture, projCoords.xy).r;

    // projected vector's z coordinate equals depth of this fragment from the light's perspective.
    float currentDepth = projCoords.z;

    // if the current fragement depth from the light source is greater than 
    // the closest depth then it is in shadow
    return currentDepth > closestDepth ? 1.0 : 0.0;
}

void main()
{
    vec3 color = texture(floorTexture, fs_in.TexCoords).rgb; 

    // ambient
    vec3 ambient = 0.05 * color; 

    // diffuse
    vec3 normal = normalize(fs_in.Normal); 
    float cosineTerm = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = lightColor * cosineTerm;

    //specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos); 
    vec3 reflectDir = reflect(normalize(-lightDir), normal); 
    float spec = 0.0; 
    if(isBlinn){
        vec3 halfwayDir = normalize(lightDir + viewDir); 
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    }else{
        spec = pow(max(dot(normal, reflectDir), 0.0), 32.0);
    }
    vec3 specular = lightColor * spec;

    // shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);

    // final result
    vec3 lighting = (ambient + (diffuse + specular) * (1.0 - shadow)) * color;
    float gamma = 2.2;
    if(isGamma){
        lighting = pow(lighting, vec3(1.0 / gamma));
    }

    FragColor = vec4(lighting, 1.0f);
}