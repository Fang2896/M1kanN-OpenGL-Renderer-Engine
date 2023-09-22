
#version 430 core

in vec3 vPos;
out vec4 FragColor;

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
