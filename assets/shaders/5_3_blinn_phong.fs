#version 330 core
out vec4 FragColor;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D floorTexture;
uniform sampler2D shadowMapTexture;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform bool isBlinn;
uniform bool isGamma;
uniform float near_plane;
uniform float far_plane;

// when using perspective projection matrix visualizing the depth buffer will often give an almost cmoletely white result.
// This is because perspective projecion depth is transformed to non-linear depth values with most of the noticeable range close to
// the near plane. To visualize result
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

float ShadowCalculation(vec4 FragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fs_in.FragPosLightSpace.xyz / fs_in.FragPosLightSpace.w;
    
    // Transforms NDC [-1,1] into depth map range [0,1]
    projCoords = projCoords * 0.5 + 0.5;

    // Can sample depth map as the resulting [0,1] coordinates directly 
    // correspond to transformed NDC coordinates from first render pass.
    // This provides the closest depth from light's point of view.
    float closestDepth = texture(shadowMapTexture, projCoords.xy).r;

    // projected vector's z coordinate equals depth of this fragment from the light's perspective.
    float currentDepth = projCoords.z;

    // apply a small amount of bias to get a depth smaller than the surface's depth 
    // Simply offset the depth of the surface (or shadow map) by a small amount 
    // such that fragements are not incorrectly considered about the surface.
    // float bias = 0.005;

    // Bias value highly dependent on angle bewteen light source and surface.
    // A surface with a steep angle to the light source, requires a stronger bias. 
    // A better apporace is to adjust the amount of bias based on surface angle towards the light
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(fs_in.Normal, lightDir)), 0.005);

    // Force the shadow value 0.0 when projected vector z coordinate larger than 1.0
    // to address coorinates outside the far plane of the light's orthographic frustum.
    // a light-space projected fragment coordinate is further than ligh's far plane when z-coorinate is lrager than 1.0.
    if (projCoords.z > 1.0) return 0.0;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMapTexture, 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            // sample more than once from the depth map, each time from slightly different texCoord
            // average all subresults together to get a soft shadow edge
            float pcfDepth = texture(shadowMapTexture, projCoords.xy + vec2(x,y) * texelSize).r;
            // if the current fragement depth from the light source is greater than 
            // the closest depth then it is in shadow
            shadow += (currentDepth) > pcfDepth ? 1.0 : 0.0;
        }
    }
    return shadow / 9.0;
    // return (currentDepth - bias) > closestDepth ? 1.0 : 0.0;
}

void main()
{
    vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal); 

    // ambient
    vec3 ambient = 0.15 * color; 

    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float cosineTerm = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = lightColor * cosineTerm;

    //specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(normalize(-lightDir), normal);
    float spec = 0.0;
    if(isBlinn){
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    }else{
        spec = pow(max(dot(normal, reflectDir), 0.0), 32.0);
    }
    vec3 specular = lightColor * spec;

    // shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);

    // final result
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    float gamma = 2.2;
    if(isGamma){
        lighting = pow(lighting, vec3(1.0 / gamma));
    }

    FragColor = vec4(lighting, 1.0f);
}