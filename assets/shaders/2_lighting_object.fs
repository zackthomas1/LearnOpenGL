#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform vec3 objectColor;
uniform vec3 lightColor;

// texture sampler
uniform sampler2D imgTex0;  // texture unit = 0
uniform sampler2D imgTex1;  // texture unit = 1

void main()
{

    vec4 tex0 = texture(imgTex0, texCoord); 
    vec4 tex1 = texture(imgTex1, texCoord);
    
    float alpha_tex = 0.5f;
	vec3 texColor = (tex0.xyz * tex0.w) * (1.0 - tex1.w * alpha_tex) + (tex1.xyz * tex1.w * alpha_tex);

    FragColor = vec4(lightColor * objectColor, 1.0);
}