#version 420 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec2 inTexCoords;

out vec3 fragNormal;
out vec2 fragTexCoords;
out vec3 fragPos;

uniform mat4 model;

layout (std140, binding = 0) uniform material
{
    vec4    ambient;
    vec4    diffuse;
    int     texturesTypes;
    int     faceCamera;
};

layout (std140, binding = 1) uniform camera
{
    mat4 proj;
    mat4 view;
    vec3 pos;
    vec3 dir;
};

void main()
{
    mat4 modelView = view * model;

    if (faceCamera == 1)
    {
        float d = sqrt(pow(modelView[0][0], 2) + pow(modelView[1][1], 2) + pow(modelView[2][2], 2));
        // Column 0:
        modelView[0][0] = d;
        modelView[0][1] = 0;
        modelView[0][2] = 0;

        // Column 1:
        modelView[1][0] = 0;
        modelView[1][1] = d;
        modelView[1][2] = 0;

        // Column 2:
        modelView[2][0] = 0;
        modelView[2][1] = 0;
        modelView[2][2] = d;

        fragNormal = -dir;
    }
    else
    {
        fragNormal = mat3(transpose(inverse(model))) * inNormal;
    }

    gl_Position = proj * modelView * vec4(inPosition, 1.0);
    fragTexCoords = inTexCoords;
    fragPos = vec3(model * vec4(inPosition, 1.0));
}
