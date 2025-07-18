#version 330 core
struct Material{

    // Note: sampler2D is a so called opaque type which means 
    // it be can't instantiated, but only defined as uniforms. 
    // If the struct would be instantiated other than as a uniform 
    // (like a function parameter) GLSL could throw strange errors; 
    // the same thus applies to any struct holding such opaque types.
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

    float shininess;
}; uniform Material material;

out vec4 FragColor;

float near  = 0.1; 
float far   = 100.0; 

float LinearizeZValue(float depth){
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    FragColor = vec4(0.04,0.28,0.26, 1.0);
}

