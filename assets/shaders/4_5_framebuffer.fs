#version 330 core

in vec2 TexCoords; 

uniform sampler2D screenTexture; 

out vec4 FragColor;

const float offset = 1.0 / 300.0;

void main()
{
    vec3 RGB = texture(screenTexture, TexCoords).rgb;
    // Inversion
    vec3 inversion = vec3(1.0) - RGB;

    // Grayscale
    float average = (0.2126 * RGB.r + 0.71552 * RGB.g + 0.0722 * RGB.b) / 3.0;
    vec3 grayscale = vec3(average);

    // kernels 
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset),  // top-left
        vec2(   0.0f, offset),  // top-left
        vec2( offset, offset),  // top-left
        vec2(-offset, 0.0f),    // center-left
        vec2(   0.0f, 0.0f),    // center-left
        vec2( offset, 0.0f),    // center-left
        vec2(-offset, -offset), // bottom-left
        vec2(   0.0f, -offset), // bottom-left
        vec2( offset, -offset)  // bottom-left
    );

    float sharpen_kernel[9] = float[](
        -1,-1,-1,
        -1, 9,-1,
        -1,-1,-1
    );

    float blur_kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16
    );

    float edge_detect_kernel[9] = float[](
        1, 1, 1,
        1,-8, 1,
        1, 1, 1
    );

    float sobel_kernel[9] = float[](
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    );

    vec3 sampleTex[9]; 
    for(int i = 0; i < 9; i++){
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]).rgb);
    }
    vec3 color = vec3(0.0); 
    for(int i = 0; i < 9; i++){
        color += sampleTex[i] * sobel_kernel[i];
    }


    FragColor = vec4(RGB, 1.0);
}

// kernel (or convolution matrix) is a small matrix-like array of values centered 
// on the current pixel that multiplies surrounding pixel values by its kernel values
// and adds them all together to form a single value

// Most kernels you'll find over the internet all sum up to 1 
// if you add all the weights together. If they don't add up to 1 it means that the 
// resulting texture color ends up brighter or darker than the original texture value.