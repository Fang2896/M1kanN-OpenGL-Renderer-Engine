
#version 410 core

struct PointLight {
    vec3 position;
    vec3 ambientColor;     // Light color
    vec3 diffuseColor;     // Light color
    vec3 specularColor;     // Light color
    float intensity;
    float falloff;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambientColor;     // Light color
    vec3 diffuseColor;     // Light color
    vec3 specularColor;     // Light color
    float intensity;
    float falloff;
};

struct DirectLight {
    vec3 direction; // Light direction
    vec3 ambientColor;     // Light color
    vec3 diffuseColor;     // Light color
    vec3 specularColor;     // Light color
    float intensity; // Light intensity
};

struct ShapeMaterial {
    float shininess;
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;

    float ambientOcclusion;

    // TODO: 可以考虑增加透明度？

};

uniform vec3 viewPos;
uniform ShapeMaterial material;
uniform DirectLight directLight;   // 先用一个光源吧

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;


void main()
{
    // 环境光
    vec3 ambient = directLight.ambientColor * material.ambientColor;

    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(directLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = directLight.diffuseColor * (diff * material.diffuseColor);

    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = directLight.specularColor * (spec * material.specularColor);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}