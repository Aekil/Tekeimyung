#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTexCoords;

out vec4 outColor;
out vec2 outTexCoords;

uniform mat4 trans;
uniform mat4 proj;
uniform vec4 color;
uniform vec2 texCoordsShift;

void main()
{
    gl_Position = proj * trans * vec4(inPosition, 1.0);
    outColor = color;
    outTexCoords = inTexCoords + texCoordsShift;
}
