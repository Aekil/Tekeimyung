layout (location = 0) in vec2 fragTexCoords;

out vec4 outFragColor;

uniform sampler2D image;

uniform bool horizontal;
uniform vec2 targetSize;

// Gaussian blur weights for a 21x21 kernel: http://dev.theomader.com/gaussian-kernel-calculator/
uniform float weight[11] = float[] (0.035822, 0.05879, 0.086425, 0.113806, 0.13424, 0.141836, 0.13424, 0.113806, 0.086425, 0.05879, 0.035822);

void main()
{
    vec4 result = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec2 pixelSize = 1.0f / targetSize;

    if (horizontal)
    {
        for (int i = -5; i <= 5; ++i)
        {
            result += texture(image, fragTexCoords + vec2(pixelSize.x * i, 0.0)) * weight[i + 5];
        }
        outFragColor = result;
    }
    else
    {
        for (int i = -5; i <= 5; ++i)
        {
            result += texture(image, fragTexCoords + vec2(0.0, pixelSize.y * i)) * weight[i + 5];
        }
        outFragColor = result;
    }
}
