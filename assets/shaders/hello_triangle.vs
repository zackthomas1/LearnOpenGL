#version 330 core

layout (location = 0) in vec3 aPos;		// 
layout (location = 1) in vec3 aColor;	// the color variable has attribute position 1 

uniform float xOffset;

out vec4 vertexColor;
out vec4 vertexPosition;

void main()
{
	mat4 transformationMat = mat4(
								1.0,  0.0,  0.0,  0.0, 
								0.0, -1.0,  0.0,  0.0, 
								0.0,  0.0,  1.0,  0.0, 
								0.0,  0.0,  0.0,  1.0
								); 

	vertexColor = vec4(aColor.x, aColor.y, aColor.z,1.0);
	vertexPosition = transformationMat * vec4((aPos.x + xOffset), aPos.y, aPos.z, 1.0);; 

	gl_Position = transformationMat * vec4((aPos.x + xOffset), aPos.y, aPos.z, 1.0);
}