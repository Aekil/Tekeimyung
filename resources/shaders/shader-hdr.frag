layout (location = 0) in vec2 fragTexCoords;

out vec4 outFragColor;

uniform sampler2D image;

uniform float gamma = 2.2f;

void main()
{
    vec3 color = texture(image, fragTexCoords).rgb;
    vec3 mapped = color / (color + vec3(1.0));

    outFragColor = vec4(mapped, 1.0f);
}
