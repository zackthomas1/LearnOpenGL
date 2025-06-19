#version 330 core 

uniform vec3 emission;
out vec4 FragColor;

void main()
{
    FragColor = vec4(emission, 1.0f); // set all 4 vector values to 1.0
}