#version 330 core

// declare the type of primitive input 
// receiving from the vertex shader.
layout (points) in; 
layout(line_strip, max_vertices = 2) out;

void main()
{
    gl_Position = gl_in[0].gl_Position;
    // Each call to EmitVertex, the vector currently set 
    // to gl_Positionis added to the output primitive.
    EmitVertex(); 

    // By repeatedly calling EndPrimitive, after one or more EmitVertex calls,
    // multiple primitives can be generated. 
    EndPrimitive();
}