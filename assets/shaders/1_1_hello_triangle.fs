#version 330 core

out vec4 FragColor;

in vec4 vertexColor;
in vec4 vertexPosition;
in vec2 TexCoord;

uniform vec4 periodic_brightness;
uniform float alpha_tex;
uniform vec2 pos_tex; 
uniform float scale_tex;

// texture samplers
uniform sampler2D imgTex0;	// texture unit = 0
uniform sampler2D imgTex1; 	// texture unit = 1

void main()
{
	vec2 transTexCoord = (TexCoord * scale_tex) + pos_tex;
	vec4 tex0 = texture(imgTex0, TexCoord); 
	vec4 tex1 = texture(imgTex1, transTexCoord);

	vec3 FragRGB = (tex0.xyz * tex0.w) * (1.0 - tex1.w * alpha_tex) + (tex1.xyz * tex1.w * alpha_tex);
	FragColor = vec4(FragRGB, 1.0);

} 