#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform vec2 iMouse;
uniform vec2 iResolution;
uniform float iTime;
uniform mat4 viewMatrix;
uniform float angle;
uniform vec3 camera;

out vec2 texCoord;

void main() {
    texCoord = aTexCoords;
    vec2 pivot = vec2((iResolution.x/2)+camera.x,(iResolution.y/2)+camera.y);
    vec2 relativePos = aPos - pivot;
    mat2 rotationMatrix = mat2(
        cos(angle), -sin(angle),
        sin(angle), cos(angle)
    );
    vec2 rotatedPos = rotationMatrix * relativePos;
    vec2 finalPos = rotatedPos + pivot;
    gl_Position = viewMatrix * vec4(finalPos, 0.0, 1.0);
}
