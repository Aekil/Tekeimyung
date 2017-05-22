layout (location = 0) in vec2 fragTexCoords;

out vec4 outFragColor;

uniform sampler2D image;

void main()
{
    outFragColor = texture(image, fragTexCoords);
}
