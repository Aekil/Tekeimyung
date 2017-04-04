layout (location = 0) in vec2 fragTexCoords;

out vec4 outFragColor;

uniform sampler2D sceneTexture;
uniform sampler2D sceneBloomTexture;

void main()
{
    vec4 color = texture(sceneTexture, fragTexCoords);
    vec4 bloomColor = texture(sceneBloomTexture, fragTexCoords);

    outFragColor = color + bloomColor;
}
