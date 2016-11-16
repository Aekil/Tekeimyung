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
};

void main()
{
    mat4 boneTransform = mat4(1.0);

    /*boneTransform += bones[inBonesIds[0].x] * inBonesWeights[0].x;
    boneTransform += bones[inBonesIds[0].y] * inBonesWeights[0].y;
    boneTransform += bones[inBonesIds[0].z] * inBonesWeights[0].z;
    boneTransform += bones[inBonesIds[0].w] * inBonesWeights[0].w;


    boneTransform += bones[inBonesIds[1].x] * inBonesWeights[1].x;
    boneTransform += bones[inBonesIds[1].y] * inBonesWeights[1].y;
    boneTransform += bones[inBonesIds[1].z] * inBonesWeights[1].z;
    boneTransform += bones[inBonesIds[1].w] * inBonesWeights[1].w;*/

    vec4 vertPos = boneTransform * vec4(inPosition, 1.0);
    gl_Position = proj * view * model * vertPos;
    fragNormal = mat3(transpose(inverse(model))) * inNormal;
    fragTexCoords = inTexCoords;
    fragPos = vec3(model * vertPos);
}
