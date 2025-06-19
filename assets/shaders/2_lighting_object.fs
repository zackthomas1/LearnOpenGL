#version 330 core
struct Material{

    // Note: sampler2D is a so called opaque type which means 
    // it be can't instantiated, but only defined as uniforms. 
    // If the struct would be instantiated other than as a uniform 
    // (like a function parameter) GLSL could throw strange errors; 
    // the same thus applies to any struct holding such opaque types.
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;

    // Note: The higher the shininess value of an object, the more it properly reflects the light 
    // instead of scattering it all around and thus the smaller the highlight becomes.
    float shininess;
};

struct Light{
    vec3 position;

    // Note: a light source has a different intensity for 
    // ambient, diffuse, and specular components. 
    vec3 ambient;   // Usually a low intensity
    vec3 diffuse;   // Set to exact color of light source
    vec3 specular;  // Usally kept at vec3(1.0) 
};

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    // ambient color
    // ------------------
    vec3 ambientColor = vec3(texture(material.diffuse, texCoords)) * light.ambient;

    // diffuse color
    // -----------------
    vec3 norm = normalize(normal); 
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Note: The cosine term is the factor that describes how much light interacts with surface. 
    // A fragments brightness increases the closer it aligns with the incoming light rays from the source.
    float cosineTerm = max(dot(norm, lightDir), 0.0);
    vec3 diffuseColor = (vec3(texture(material.diffuse, texCoords)) * cosineTerm) * light.diffuse;

    // specular color
    // -------------------
    // Note: To get the view direction calculate the difference between the view (camera) position
    // and the fragment position. Make sure that both are in world space coordinates.
    // It is maybe preferrable to perform this calculation in view-space.
    vec3 viewDir = normalize(viewPos - fragPos);

    // Note: The lightDir vector is negated. The reflect function expects the first vector 
    // to point from the light source towards the fragment's position. The lightDir vector 
    // currently points the other way around. To make sure we get the correct reflect vector
    // we reverse the lightDir vector. 
    vec3 reflectDir = reflect(-lightDir, norm); 

    // Note: Calculate the angular distance between this reflection vector and the view direction.
    // The closer the angle between them, the greater the impact of the specular light.
    float specularIntensity = pow(max(dot(reflectDir, viewDir), 0.0),material.shininess);
    vec3 specularColor = (texture(material.specular, texCoords).rgb * specularIntensity) * light.specular;

    vec3 emissionColor = texture(material.emission, texCoords).rgb;

    // output
    // ----------
    vec3 rgbColor = ambientColor + diffuseColor + specularColor + emissionColor;

    FragColor = vec4(rgbColor, 1.0);
}