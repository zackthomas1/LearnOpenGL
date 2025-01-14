#version 330 core 
struct Light{
    vec3 position;

    // Note: a light source has a different intensity for 
    // ambient, diffuse, and specular components. 
    vec3 ambient;   // Usually a low intensity
    vec3 diffuse;   // Set to exact color of light source
    vec3 specular;  // Usally kept at vec3(1.0) 
};

in vec3 normal;

uniform Light light;

out vec4 FragColor;

void main()
{
    FragColor = vec4(light.diffuse, 1.0f); // set all 4 vector values to 1.0
}