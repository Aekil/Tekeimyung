#version 330 core

in vec3 fragColor;
in vec3 fragNormal;

out vec4 outFragColor;

vec3 getAmbient(vec3 lightAmbient)
{
    return lightAmbient  * fragColor;
}

vec3 getDiffuse(vec3 lightDiffuse, vec3 normal, vec3 lightDir)
{
    float diff = max(dot(normal, lightDir), 0.0);

    return lightDiffuse * (diff * fragColor);
}

void main()
{
   vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 lightDir = normalize(vec3(350.0f, 150.0f, 100.0f));
    vec3 color = getAmbient(lightColor) + getDiffuse(lightColor, fragNormal, lightDir);

    outFragColor = vec4(color, 1.0);
}
