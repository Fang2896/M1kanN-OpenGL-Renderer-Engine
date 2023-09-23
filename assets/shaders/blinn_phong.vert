#version 410 core

layout(location = 0) in vec3 inPosition; // Vertex position
layout(location = 1) in vec3 inNormal;   // Vertex normal
layout(location = 2) in vec2 inTexCoord; // Texture coordinate

out vec3 worldPos;
out vec3 norm;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    worldPos = vec3(model * vec4(inPosition, 1.0));
    norm = mat3(transpose(inverse(model))) * inNormal; // Correct way to transform normals
    texCoord = inTexCoord;

    gl_Position = projection * view * vec4(worldPos, 1.0);
}