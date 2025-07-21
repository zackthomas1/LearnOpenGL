#version 330 core

in vec3 texCoords;

uniform samplerCube skybox;

out vec4 FragColor;

void main()
{
    vec4 skyboxColor    = texture(skybox, texCoords);
    FragColor           = skyboxColor;
}