layout (location = 0) in vec3 fragNormal;
layout (location = 1) in vec2 fragTexCoords;
layout (location = 2) in vec3 fragPos;
layout (location = 3) flat in uint instanceID;

uniform vec4 color;

out vec4 outFragColor;

void main()
{
    outFragColor = color;
}
