#version 330 core
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 instanceMatrix;

uniform mat4 view;
uniform mat4 perspective;

out vec2 texCoord;

void main()
{
    texCoord = aTexCoord;
    gl_Position = perspective * view * instanceMatrix * vec4(aPos, 1.0);
}