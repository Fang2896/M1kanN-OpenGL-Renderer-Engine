
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

// 暂且仅用diffuse和specular
struct Material {
    float shininess;

    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;

    float ambientOcclusion;
};

uniform vec3 viewPos;

uniform bool useDiffuseTexture;
uniform bool useSpecularTexture;
uniform bool useLight;

uniform Material material;
uniform DirectLight directLight;   // 先用一个光源吧
uniform PointLight pointLight;
uniform SpotLight spotLight;

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;


void main()
{
    // 环境光
    // 漫反射
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(directLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 ambient;
    vec3 diffuse;
    if(useDiffuseTexture) {
        ambient = directLight.ambientColor * vec3(texture(material.texture_diffuse1, TexCoord));
        diffuse = directLight.diffuseColor * diff * vec3(texture(material.texture_diffuse1, TexCoord));
    } else {
        ambient = directLight.ambientColor * material.ambientColor;
        diffuse = directLight.diffuseColor * diff * material.diffuseColor;
    }

    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);

    vec3 specular;
    if(useSpecularTexture) {
        specular = directLight.specularColor * spec *
            vec3(texture(material.texture_specular1, TexCoord));
    } else {
        specular = directLight.specularColor * spec * material.specularColor;
    }

    vec3 result;
    if(useLight)
        result = (ambient + diffuse + specular) * directLight.intensity;
    else
        result = ambient * directLight.intensity;
    
    // for debug
//    if(useDiffuseTexture && useSpecularTexture) {
//        FragColor = vec4(1,0,0, 1.0);
//    } else if (useDiffuseTexture) {
//        FragColor = vec4(0,1,0, 1.0);
//    } else if (useSpecularTexture) {
//        FragColor = vec4(0,0,1, 1.0);
//    } else {
//        FragColor = vec4(1,1,0, 1.0);
//    }

    FragColor = vec4(result, 1.0);

}