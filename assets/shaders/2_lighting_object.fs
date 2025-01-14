#version 330 core
struct Material{
    vec3 ambient; 
    vec3 diffuse; 
    vec3 specular; 

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

in vec3 normal;
in vec3 fragPos;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    // ambient color
    // ------------------
    vec3 ambientColor = material.ambient * light.ambient;

    // diffuse color
    // -----------------
    vec3 norm = normalize(normal); 
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Note: The cosine term is the factor that describes how much light interacts with surface. 
    // A fragments brightness increases the closer it aligns with the incoming light rays from the source.
    float cosineTerm = max(dot(norm, lightDir), 0.0);
    vec3 diffuseColor = (material.diffuse * cosineTerm) * light.diffuse;

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
    vec3 specularColor = (material.specular * specularIntensity) * light.specular;

    // output
    // ----------
    vec3 rgbColor = ambientColor + diffuseColor + specularColor;
    FragColor = vec4(rgbColor, 1.0);
}