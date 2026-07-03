#version 330 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

}; uniform Material material;

in vec2 texCoord;

out vec4 FragColor;

void main()
{
    vec3 tex_color = texture(material.texture_diffuse1, texCoord).rgb;
    FragColor = vec4(tex_color, 1.0);
}