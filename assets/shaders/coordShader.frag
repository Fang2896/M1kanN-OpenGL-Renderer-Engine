
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

in vec3 vPos;
out vec4 FragColor;

uniform vec3 viewPos;   // 这个没用
uniform DirectLight directLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;


void main()
{
     // x -> red
    if(abs(vPos.x) > abs(vPos.y) && abs(vPos.x) > abs(vPos.z)) {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
    // y -> yellow
    else if(abs(vPos.y) > abs(vPos.x) && abs(vPos.y) > abs(vPos.z)) {
        FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    }
    // z -> blue
    else {
        FragColor = vec4(0.0, 0.0, 1.0, 1.0);
    }
}
