layout (location = 0) in vec2 fragTexCoords;

out vec4 outFragColor;

uniform sampler2D image;

uniform bool horizontal;

// Gaussian blur weights for a 21x21 kernel: http://dev.theomader.com/gaussian-kernel-calculator/
uniform float weight[5] = float[] (0.1784, 0.210431, 0.222338, 0.210431, 0.1784);

void main()
{
    vec2 pixelSize = 1.0 / textureSize(image, 0); // Size of a single texel
    vec4 result = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    //vec2 pixelSize = 1.0f / vec2(1920.0, 1017.0);

    if (horizontal)
    {
        for (int i = -2; i <= 2; ++i)
        {
            result += texture(image, fragTexCoords + vec2(pixelSize.x * i, 0.0)) * weight[i + 2];
        }
        outFragColor = result;
     }
    else
    {
        for (int i = -2; i <= 2; ++i)
        {
            result += texture(image, fragTexCoords + vec2(0.0, pixelSize.y * i)) * weight[i + 2];
        }
        outFragColor = result;
    }
    //outFragColor = texture(image, fragTexCoords);
}
