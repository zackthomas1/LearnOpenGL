#version 330 core

layout (location = 0) in vec3 aPos;		// 
layout (location = 1) in vec3 aColor;	// the color variable has attribute position 1 
layout (location = 2) in vec2 aTexCoord;// 

out vec4 vertexColor;
out vec4 vertexPosition;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view; 
uniform mat4 projection;

void main()
{
	// pass uniforms to fragment shader
	vertexColor = vec4(aColor.x, aColor.y, aColor.z,1.0);
	vertexPosition = vec4(aPos.x, aPos.y, aPos.z, 1.0);; 
	TexCoord = aTexCoord;

	gl_Position = projection * view * model * vec4(aPos.x , aPos.y, aPos.z, 1.0);
}