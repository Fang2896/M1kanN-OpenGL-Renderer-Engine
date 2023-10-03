
#version 410 core

//enum class PostProcessingType {
//    NORMAL = 0,
//    INVERSION = 1,
//    GRAY = 2,
//    SHARPEN = 3,
//    BLUR = 4,
//    EDGE = 5,
//    SCAN = 6
//};

out vec4 FragColor;
in vec2 TexCoords;

const float sharpenKernel[9] = float[9](
    -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1
);

const float blurKernel[9] = float[9](
    1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f,
    2.0f/16.0f, 4.0f/16.0f, 2.0f/16.0f,
    1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f
);

const float edgeKernel[9] = float[9](
    1,  1, 1,
    1, -8, 1,
    1,  1, 1
);

const float offset = 1.0f / 300.0f;

uniform sampler2D screenTexture;
uniform int postProcessingType;

vec4 processKernel(vec2 uv, float[9] kernel) {
    const vec2 offsets[9] = vec2[9](
        vec2(-offset, offset),
        vec2(0.0f, offset),
        vec2(offset, offset),
        vec2(-offset, 0.0f),
        vec2(0.0f, 0.0f),
        vec2(offset, 0.0f),
        vec2(-offset, -offset),
        vec2(0.0f, -offset),
        vec2(offset, -offset)
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++) {
        sampleTex[i] = texture(screenTexture, uv.st + offsets[i]).rgb;
    }

    vec3 tempColor;
    for(int i = 0; i < 9; ++i) {
        tempColor += sampleTex[i] * kernel[i];
    }

    return vec4(tempColor, 1.0f);
}

void main(){
    vec3 color;
    switch(postProcessingType) {
        case 0: // Normal
            color = texture(screenTexture, TexCoords).rgb;
            FragColor = vec4(color, 1.0f);
            break;
        case 1: // Inversion
            color = texture(screenTexture, TexCoords).rgb;
            FragColor = vec4(1.0 - color, 1.0f);
            break;
        case 2: // Gray
            color = texture(screenTexture, TexCoords).rgb;
            float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
            FragColor = vec4(average, average, average, 1.0f);
            break;
        case 3: // sharpen
            FragColor = processKernel(TexCoords, sharpenKernel);
            break;
        case 4: // blur
            FragColor = processKernel(TexCoords, blurKernel);
            break;
        case 5: // edge
            FragColor = processKernel(TexCoords, edgeKernel);
            break;
        case 6: // scan
            FragColor = vec4(1,0,0, 1.0f);  // TODO: add scan post processing
    }

}