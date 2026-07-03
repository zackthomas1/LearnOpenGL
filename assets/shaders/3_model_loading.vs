#version 330 core 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
    vec3 fragPos; 
    vec3 normal; 
    vec2 texCoords; 
} vs_out;

void main()
{

    vs_out.fragPos = (model * vec4(aPos, 1.0)).xyz; 

    mat3 normalMat = mat3(transpose(inverse(model))); 
    vs_out.normal = normalMat * aNormal; 
    
    // texture coordinates
    vs_out.texCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}