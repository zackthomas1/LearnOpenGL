#version 330 core
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 normal;

void main()
{
    normal = aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}