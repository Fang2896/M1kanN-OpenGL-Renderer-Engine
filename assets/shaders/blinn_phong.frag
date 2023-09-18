#version 430 core

in vec3 worldPos;
in vec3 norm;
in vec2 texCoord;

out vec4 FragColor;

struct Light {
    vec3 direction; // Light direction
    vec3 color;     // Light color
    float intensity; // Light intensity
};

struct Material {
    float shininess;
    float metallic;
    float roughness;
    vec3 baseColor;
    float ambientOcclusion;
};

uniform vec3 ambientColor;
uniform Light light;
uniform Material material;
uniform sampler2D textureSampler;

void main()
{
    // Ambient
    vec3 aColor = ambientColor * material.ambientOcclusion;

    // Diffuse
    vec3 normNormalized = normalize(norm);
    vec3 lightDirNormalized = normalize(-light.direction);
    float diff = max(dot(normNormalized, lightDirNormalized), 0.0);
    vec3 dColor = diff * light.color * material.baseColor;

    // Specular
    vec3 viewDir = normalize(-fragPos);
    vec3 halfwayDir = normalize(lightDirNormalized + viewDir);
    float spec = pow(max(dot(normNormalized, halfwayDir), 0.0), 64.0);
    vec3 sColor = spec * light.intensity * light.color;

    // Texture color
    vec3 textureColor = texture(textureSampler, texCoord).rgb;

    // Metallic and Roughness effect (simple implementation)
    textureColor = mix(textureColor, material.baseColor, material.metallic) * (1.0 - material.roughness);

    vec3 result = (aColor + dColor + sColor) * textureColor;
    FragColor = vec4(result, 1.0);
}
