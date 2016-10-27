#version 330 core

in vec4 outColor;
in vec2 outTexCoords;

out vec4 color;

uniform sampler2D spriteTexture;

void main()
{
    color = texture(spriteTexture, outTexCoords) + outColor;
}
