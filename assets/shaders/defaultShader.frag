
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
uniform bool enableDepthMode;
uniform bool enableOutline;
uniform bool isMultiMeshModel;

uniform bool isReflection;
uniform bool isRefraction;
uniform bool isFresnel;
uniform samplerCube skybox;

uniform Material material;
uniform DirectLight directLight;   // 先用一个光源吧
uniform PointLight pointLight;
uniform SpotLight spotLight;

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;


// Compute Fresnel using Schlick's approximation
float fresnelSchlick(float cosTheta, float IOR) {
    float R0 = (1.0 - IOR) / (1.0 + IOR);
    R0 = R0 * R0;
    return R0 + (1.0 - R0) * pow(1.0 - cosTheta, 5.0);
}

vec3 getFresnel() {
    // Constants
    const float IOR = 1.5;  // Index of Refraction for glass
    vec3 glassColor = material.diffuseColor;  // Assuming white clear glass
    vec3 viewDir = normalize(viewPos - FragPos);

    float cosTheta = dot(normalize(viewDir), normalize(Normal));
    float fresnel = fresnelSchlick(cosTheta, IOR);

    // Compute reflection and refraction
    vec3 reflected = reflect(viewDir, Normal);
    vec3 refracted = refract(viewDir, Normal, 1.0 / IOR);  // Assuming air to glass transition

    // Sample environment
    vec3 envReflection = texture(skybox, reflected).rgb;
    vec3 envRefraction = texture(skybox, refracted).rgb;

    // Combine reflection and refraction with Fresnel factor
    vec3 combined = mix(envRefraction * glassColor, envReflection, fresnel);
    return combined;
}


void main()
{
    // 环境光
    // 漫反射
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(directLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 ambient;
    vec3 diffuse;
    vec4 diffuseTexSampler;

    float resultAlpha = 1.0f;

    if(useDiffuseTexture) {
        diffuseTexSampler = texture(material.texture_diffuse1, TexCoord);

        if(!isMultiMeshModel) {
            resultAlpha = diffuseTexSampler.a;
        }

        ambient = directLight.ambientColor * vec3(diffuseTexSampler);
        diffuse = directLight.diffuseColor * diff * vec3(diffuseTexSampler);
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
    if(useLight) {
        result = (ambient + diffuse + specular) * directLight.intensity;
    } else {
        result = ambient * directLight.intensity;
    }

    if(enableDepthMode) {
        result = vec3(gl_FragCoord);
        resultAlpha = 1.0f;
    } else if(enableOutline) {
        result = vec3(1.0,1.0,0.0); // red as outline color
        resultAlpha = 1.0f;
    }

    // reflection and refraction
    if(isReflection) {
        vec3 I = normalize(FragPos - viewPos);
        vec3 R = reflect(I, normalize(Normal));

        resultAlpha = 1.0f;
        result = texture(skybox, R).rgb;
    } else if(isRefraction) {
        ///*
        //   * 折射率：
        //   * 空气      1.00
        //   * 水        1.33
        //   * 冰        1.309
        //   * 玻璃      1.52
        //   * 钻石      2.42
        //  */
        vec3 I = normalize(FragPos - viewPos);
        vec3 R = refract(I, normalize(Normal), 1.0 / 1.33);  // here to change ratio
        resultAlpha = 1.0f;
        result = texture(skybox, R).rgb;
    } else if (isFresnel) {
        resultAlpha = 1.0f;
        result = getFresnel();
    }

    // outline and depth mode
    if(enableDepthMode) {
        result = vec3(gl_FragCoord);
        resultAlpha = 1.0f;
    } else if(enableOutline) {
        result = vec3(1.0,1.0,0.0); // red as outline color
        resultAlpha = 1.0f;
    }

    FragColor = vec4(result, resultAlpha);
}
