
#version 410 core

layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;

out vec3 TexCoords;

void main(){
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0f);
    gl_Position = pos.xyww; // note: set depth to 1.0f
}
