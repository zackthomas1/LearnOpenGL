#version 330 core

// declare the type of primitive input 
// receiving from the vertex shader.
layout (triangles) in; 
layout (line_strip, max_vertices = 6) out;

in VS_OUT{
    vec3 normal;
}gs_in[];

const float MAGNTIUDE = 0.2;

uniform mat4 projection;

void GenerateLine(int index){
    gl_Position = projection * gl_in[index].gl_Position; 
    EmitVertex(); 
    
    gl_Position = projection * (gl_in[index].gl_Position + 
                    vec4(gs_in[index].normal, 0.0) * MAGNTIUDE);
    EmitVertex(); 
    
    EndPrimitive();
}

void main()
{
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}