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

    // Attenuation terms: Reduce intensity of light source over distance
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight{
    vec3 position;

    // Note: a light source has a different intensity for 
    // ambient, diffuse, and specular components. 
    vec3 ambient;   // Usually a low intensity
    vec3 diffuse;   // Set to exact color of light source
    vec3 specular;  // Usally kept at vec3(1.0) 

    // Attenuation terms: Reduce intensity of light source over distance
    float constant;
    float linear;
    float quadratic;

    vec3 spotDir; // the direction the ligh is aiming
    float innerAngle;  // cutoff cos(angle(radians)) specifying the spot light radius
    float outerAngle; 
};

struct DirectionalLight{ 
    vec3 direction; // directional vector replaces position vector

    vec3 ambient; 
    vec3 diffuse; 
    vec3 specular; 

};

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

uniform Material material;
uniform SpotLight light;
uniform DirectionalLight dirLight;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{

    // light caster
    // ------------------
    float distance = distance(light.position, fragPos);
    float attenuation = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * pow(distance, 2)));
    
    vec3 lightDir = normalize(light.position - fragPos);    // positional light caster
    // vec3 lightDir = normalize(-dirLight.direction);  // directional light caster

    // calculate spot light terms
    float theta = dot(lightDir, normalize(-light.spotDir)); // the angle between the light direction and spot direction in radians
    float epsilon = light.innerAngle - light.outerAngle;
    float intensity = clamp((theta - light.outerAngle) / epsilon, 0.0, 1.0);

    // ambient color
    // ------------------
    vec3 ambientColor = vec3(texture(material.diffuse, texCoords)) * light.ambient;

    // diffuse color
    // -----------------
    vec3 norm = normalize(normal); 
    
    // Note: The cosine term is the factor that describes how much light interacts with surface. 
    // A fragments brightness increases the closer it aligns with the incoming light rays from the source.
    float cosineTerm = max(dot(norm, lightDir), 0.0);
    vec3 diffuseColor = texture(material.diffuse, texCoords).rgb * cosineTerm * light.diffuse;

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

    diffuseColor    *= intensity;
    specularColor   *= intensity;

    ambientColor    *= attenuation;
    diffuseColor    *= attenuation;
    specularColor   *= attenuation;

    // output
    // ----------
    
    vec3 rgbColor = ambientColor + diffuseColor + (specularColor);
    // compare the cosine value of the light direction and the cutoff angle.
    // If angle of light direction and spot direction is greater than the cutoff angle
    // only ouput ambient color
    // Remember: cos(0) = 1 & cos(90) = 0

    FragColor = vec4(rgbColor, 1.0);
}