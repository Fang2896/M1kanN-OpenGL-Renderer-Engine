
#version 410 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform samplerCube skybox;
uniform vec3 cameraPos;
uniform float ratio;

///*
//   * 折射率：
//   * 空气      1.00
//   * 水        1.33
//   * 冰        1.309
//   * 玻璃      1.52
//   * 钻石      2.42
//  */

void main(){
    vec3 I = normalize(FragPos - cameraPos);
    vec3 R = refract(I, normalize(Normal), 1.309);  // here to change ratio
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}