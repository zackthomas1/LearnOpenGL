#version 330 core

in vec3 Position;
in vec3 Normal;
in vec2 texCoords;

uniform sampler2D tex;
uniform samplerCube skybox;
uniform vec3 camera_pos;

out vec4 FragColor;

void main()
{
    vec3 view_dir           = normalize(Position - camera_pos); 
    
    // reflection
    vec3 reflection_dir     = reflect(view_dir, Normal);
    vec4 reflection_color   = texture(skybox, reflection_dir);

    // refraction
    float refraction_index  = 1.52;
    vec3 refraction_dir     = refract(view_dir, Normal, 1.0 / refraction_index); 
    vec4 refraction_color   = texture(skybox, refraction_dir);
    
    // texture color
    vec4 texColor           = texture(tex, texCoords);

    // with blending enable there is no need to discard fragments
    // if(texColor.a < 0.1)
    //     discard;

    // FragColor = texColor;
    // FragColor = reflection_color;
    FragColor = refraction_color;
    // FragColor = (texColor * 0.75) + (0.25 * reflection_color);
}

