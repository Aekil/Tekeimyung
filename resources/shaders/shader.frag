#version 420 core

in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 fragPos;

out vec4 outFragColor;

uniform sampler2D AmbientTexture;
uniform sampler2D DiffuseTexture;

layout (std140, binding = 0) uniform material
{
    vec4    ambient;
    vec4    diffuse;
    int     texturesTypes;
};

layout (std140, binding = 1) uniform camera
{
    mat4 proj;
    mat4 view;
    vec3 pos;
    int freezeRotations;
    vec3 dir;
};

vec4 getAmbient(vec4 lightAmbient)
{
    if ((texturesTypes & 2) != 0)
        return lightAmbient  * texture(AmbientTexture, fragTexCoords) * ambient;
    return lightAmbient * ambient;
}

vec4 getDiffuse(vec4 lightDiffuse, vec3 normal, vec3 lightDir)
{
    normal = normalize(normal);
    float diff = max(dot(normal, lightDir), 0.0);

    if ((texturesTypes & 1) != 0)
        return lightDiffuse * (diff * texture(DiffuseTexture, fragTexCoords));
    return lightDiffuse * (diff * diffuse);
}

vec4 CalcDirLight(vec3 lightDir, vec3 normal)
{
    vec4 lightAmbient = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    vec4 lightDiffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Ambient
    vec4 ambient = getAmbient(lightAmbient);

    // Diffuse shading
    vec4 diffuse = getDiffuse(lightDiffuse, normal, lightDir);

    return (ambient + diffuse);
}


void main()
{
    vec3 lightDir = normalize(vec3(350.0f, 150.0f, 100.0f) - fragPos);
    vec4 color = CalcDirLight(lightDir, fragNormal);

    outFragColor = color;
}
