layout (location = 0) in vec2 fragTexCoords;

out vec4 outFragColor;

uniform sampler2D image;

uniform bool horizontal;

// Gaussian blur weights for a 21x21 kernel: http://dev.theomader.com/gaussian-kernel-calculator/
uniform float weight[21] = float[] (0.041262, 0.043041, 0.044698, 0.046212, 0.047566, 0.048743, 0.049727, 0.050506, 0.05107, 0.051412, 0.051526, 0.051412, 0.05107, 0.050506, 0.049727, 0.048743, 0.047566, 0.046212, 0.044698, 0.043041, 0.041262);

void main()
{
    vec2 pixelSize = 1.0 / textureSize(image, 0); // Size of a single texel
    vec4 result = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    if (horizontal)
    {
        for (int i = -10; i <= 10; ++i)
        {
            result += texture(image, fragTexCoords + vec2(pixelSize.x * i, 0.0)) * weight[i + 10];
        }
     }
    else
    {
        for (int i = -10; i <= 10; ++i)
        {
            result += texture(image, fragTexCoords + vec2(0.0, pixelSize.y * i)) * weight[i + 10];
        }
    }
    outFragColor = result;
}
