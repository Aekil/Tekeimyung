layout (location = 0) in vec2 fragTexCoords;

out vec4 outFragColor;

uniform sampler2D image;

uniform bool horizontal;

// Gaussian blur weights for a 21x21 kernel: http://dev.theomader.com/gaussian-kernel-calculator/
uniform float weight[11] = float[] (0.084264, 0.088139, 0.091276, 0.093585, 0.094998, 0.095474, 0.094998, 0.093585, 0.091276, 0.088139, 0.084264);

void main()
{
    vec2 pixelSize = 1.0 / textureSize(image, 0); // Size of a single texel
    vec4 result = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    if (horizontal)
    {
        for (int i = -5; i <= 5; ++i)
        {
            result += texture(image, fragTexCoords + vec2(pixelSize.x * i, 0.0)) * weight[i + 5];
        }
     }
    else
    {
        for (int i = -5; i <= 5; ++i)
        {
            result += texture(image, fragTexCoords + vec2(0.0, pixelSize.y * i)) * weight[i + 5];
        }
    }
    outFragColor = result;
}
