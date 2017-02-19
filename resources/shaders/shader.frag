#version 420 core

in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 fragPos;

out vec4 outFragColor;

uniform sampler2D AmbientTexture;
uniform sampler2D DiffuseTexture;

uniform vec4 modelColor;

layout (std140, binding = 0) uniform materialUniformBlock
{
    vec4    ambient;
    vec4    diffuse;
    int     texturesTypes;
    int     faceCamera;
} material;


layout (std140, binding = 2) uniform lightUniformBlock
{
    vec3 ambient;
    vec3 diffuse;
    vec3 direction;
} light;

vec4 getAmbient()
{
    if ((material.texturesTypes & 2) != 0)
        return vec4(light.ambient, 1.0f)  * texture(AmbientTexture, fragTexCoords);
    return vec4(light.ambient, 1.0f) * material.ambient;
}

vec4 getDiffuse(vec3 normal)
{
    normal = normalize(normal);
    float diff = max(dot(normal, -light.direction), 0.0);

    if ((material.texturesTypes & 1) != 0)
        return vec4(light.diffuse, 1.0f) * (diff * texture(DiffuseTexture, fragTexCoords));
    return vec4(light.diffuse, 1.0f) * (diff * material.diffuse);
}

vec4 CalcFragColor(vec3 normal)
{
    // Ambient
    vec4 ambient = getAmbient() * modelColor;

    // Diffuse shading
    vec4 diffuse = getDiffuse(normal) * modelColor;

    return (ambient + diffuse);
}


void main()
{
    vec4 color = CalcFragColor(fragNormal);

    outFragColor = color;
}
