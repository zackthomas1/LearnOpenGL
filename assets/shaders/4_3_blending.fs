#version 330 core

uniform sampler2D tex;

in vec2 texCoords;

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(tex, texCoords);

    // with blending enable there is no need to discard fragments
    // if(texColor.a < 0.1)
    //     discard;

    FragColor = texColor;
}

