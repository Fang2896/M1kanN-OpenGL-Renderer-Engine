
#version 410 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTex;

out vec2 TexCoords;

void main(){
    gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f); // z = 0
    TexCoords = aTex;
}
