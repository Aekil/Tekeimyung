#version 420 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;

out vec3 fragNormal;

uniform mat4 model;

layout (std140, binding = 1) uniform camera
{
    mat4 proj;
    mat4 view;
};

void main()
{
    gl_Position = proj * view * model * vec4(inPosition, 1.0);
    fragNormal = inNormal;
}
