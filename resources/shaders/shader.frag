#version 420 core

in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 fragPos;

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

layout (std140, binding = 1) uniform camera
{
    mat4 proj;
    mat4 view;
    vec3 pos;
    int freezeRotations;
    vec3 dir;
};

vec3 getAmbient(vec3 lightAmbient)
{
    return lightAmbient  * (vec3(texture(AmbientTexture, fragTexCoords)) + ambient);
}

vec3 getDiffuse(vec3 lightDiffuse, vec3 normal, vec3 lightDir)
{
    normal = normalize(normal);
    float diff = max(dot(normal, lightDir), 0.0);

    return lightDiffuse * (diff * (vec3(texture(DiffuseTexture, fragTexCoords)) + diffuse));
}

vec3 CalcDirLight(vec3 lightDir, vec3 normal)
{
    vec3 lightAmbient = vec3(0.5f, 0.5f, 0.5f);
    vec3 lightDiffuse = vec3(1.0f, 1.0f, 1.0f);

    // Ambient
    vec3 ambient = getAmbient(lightAmbient);

    // Diffuse shading
    vec3 diffuse = getDiffuse(lightDiffuse, normal, lightDir);

    return (ambient + diffuse);
}


void main()
{
    vec3 lightDir = normalize(vec3(350.0f, 150.0f, 100.0f) - fragPos);
    vec3 color = CalcDirLight(lightDir, fragNormal);

    outFragColor = vec4(color, 1.0);
}
