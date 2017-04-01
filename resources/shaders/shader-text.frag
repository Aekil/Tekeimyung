layout (location = 0) in vec3 fragNormal;
layout (location = 1) in vec2 fragTexCoords;
layout (location = 2) in vec3 fragPos;

out vec4 outFragColor;

uniform sampler2D textImage;

void main()
{
    outFragColor = vec4(1.0f, 1.0f, 1.0f, texture(textImage, fragTexCoords).r);
}
