
#version 410 core

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float falloff;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float intensity;
};

struct Material {
    sampler2D texture_diffuse1;
    // ... 其他的属性，如specular1, ambient1等
};

uniform DirectionalLight dirLight;
uniform Material material;

out vec4 FragColor;
in vec2 TexCoords;

void main()
{
    // FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    FragColor = texture(material.texture_diffuse1, TexCoords);
}
