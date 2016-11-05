#version 420 core

in vec3 fragNormal;
in vec2 fragTexCoords;

out vec4 outFragColor;

uniform sampler2D AmbientTexture;
uniform sampler2D DiffuseTexture;

layout (std140, binding = 0) uniform material
{
    vec3    ambient;
    float   padding;
    vec3    diffuse;
    float   padding2;
    int     texturesTypes;
};

vec3 getAmbient(vec3 lightAmbient)
{
    if ((texturesTypes & 2) != 0)
        return lightAmbient  * vec3(texture(AmbientTexture, fragTexCoords));
    return lightAmbient  * ambient;
}

vec3 getDiffuse(vec3 lightDiffuse, vec3 normal, vec3 lightDir)
{
    float diff = max(dot(normal, lightDir), 0.0);

    if ((texturesTypes & 1) != 0)
        return lightDiffuse * (diff * vec3(texture(DiffuseTexture, fragTexCoords)));
    return lightDiffuse * (diff * diffuse);
}

void main()
{
   vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 lightDir = normalize(vec3(350.0f, 150.0f, 100.0f));
    vec3 color = getAmbient(lightColor) + getDiffuse(lightColor, fragNormal, lightDir);

    outFragColor = vec4(color, 1.0);
}
