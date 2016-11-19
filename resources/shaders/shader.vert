#version 420 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec2 inTexCoords;
layout (location = 4) in ivec4 inBonesIds[2];
layout (location = 6) in vec4 inBonesWeights[2];

out vec3 fragNormal;
out vec2 fragTexCoords;
out vec3 fragPos;

const int MAX_BONES = 100;

uniform mat4 model;

layout (std140, binding = 2) uniform bonesUniform
{
    mat4 bones[MAX_BONES];
};

layout (std140, binding = 1) uniform camera
{
    mat4 proj;
    mat4 view;
    vec3 pos;
    int freezeRotations;
    vec3 dir;
};

void main()
{
    mat4 boneTransform = mat4(1.0);
    mat4 modelView = view * model;

    if (freezeRotations == 1)
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

    /*boneTransform += bones[inBonesIds[0].x] * inBonesWeights[0].x;
    boneTransform += bones[inBonesIds[0].y] * inBonesWeights[0].y;
    boneTransform += bones[inBonesIds[0].z] * inBonesWeights[0].z;
    boneTransform += bones[inBonesIds[0].w] * inBonesWeights[0].w;


    boneTransform += bones[inBonesIds[1].x] * inBonesWeights[1].x;
    boneTransform += bones[inBonesIds[1].y] * inBonesWeights[1].y;
    boneTransform += bones[inBonesIds[1].z] * inBonesWeights[1].z;
    boneTransform += bones[inBonesIds[1].w] * inBonesWeights[1].w;*/

    vec4 vertPos = boneTransform * vec4(inPosition, 1.0);
    gl_Position = proj * modelView * vertPos;
    fragTexCoords = inTexCoords;
    fragPos = vec3(model * vertPos);
}
