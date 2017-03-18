layout (location = 0) in vec2 fragTexCoords;

out vec4 outFragColor;

uniform sampler2D sceneTexture;

void main()
{
    vec4 color = texture(sceneTexture, fragTexCoords);

    outFragColor = color;
}
