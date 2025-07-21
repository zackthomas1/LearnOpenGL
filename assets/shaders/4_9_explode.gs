#version 330 core

// declare the type of primitive input 
// receiving from the vertex shader.
layout (triangles) in; 
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT{
    vec3 fragPos; 
    vec3 normal; 
    vec2 texCoords; 
} gs_in[];

uniform float time;

out vec3 fragPos; 
out vec3 normal; 
out vec2 texCoords;

vec3 GetNormal(){
    vec3 a = vec3(gl_in[0].gl_Position - gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position - gl_in[1].gl_Position);
    return normalize(cross(a, b)); 
}

vec4 explode(vec4 position, vec3 normal){
    float magnitude = 2.0; 
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
    return position + vec4(direction, 0.0);
}

void main()
{
    gl_Position = explode(gl_in[0].gl_Position, gs_in[0].normal);
    fragPos     = gs_in[0].fragPos;
    normal      = gs_in[0].normal;
    texCoords   = gs_in[0].texCoords;
    // Each call to EmitVertex, the vector currently set 
    // to gl_Positionis added to the output primitive.
    EmitVertex(); 

    gl_Position = explode(gl_in[1].gl_Position, gs_in[1].normal);
    fragPos     = gs_in[1].fragPos;
    normal      = gs_in[1].normal;
    texCoords   = gs_in[1].texCoords;
    EmitVertex(); 

    gl_Position = explode(gl_in[2].gl_Position, gs_in[2].normal);
    fragPos     = gs_in[2].fragPos;
    normal      = gs_in[2].normal;
    texCoords   = gs_in[2].texCoords;
    EmitVertex(); 
    
    // By repeatedly calling EndPrimitive, after one or more EmitVertex calls,
    // multiple primitives can be generated. 
    EndPrimitive();
}