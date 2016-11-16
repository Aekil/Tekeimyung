#pragma once

#define MAX_BONES 100

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <glm/mat4x4.hpp>

#include <Engine/Graphics/UniformBuffer.hpp>

class Skeleton
{
public:
    struct sBone
    {
        // Bone id
        uint32_t id;

        // Bone name
        std::string name;

        // Bone offset matrix to convert vertices from local space to bone space
        glm::mat4   offset;

        // Bone final transformation
        glm::mat4   finalTransform;
    };

public:
    Skeleton();
    ~Skeleton();
    sBone*                                      getBoneByName(const std::string& name);
    sBone*                                      getBoneById(uint32_t id);
    const std::vector<Skeleton::sBone>&         getBones() const;
    void                                        addBone(const std::string& name, const glm::mat4& offset);

    void                                        updateUniformBuffer();
    const UniformBuffer&                        getUbo() const;

private:
    // Map to get bones ids with name
    std::unordered_map<std::string, uint32_t>   _bonesIds;

    // Bones list
    // Bone index match the bone id
    std::vector<Skeleton::sBone>                _bones;

    // Bones transformations list
    // Used to pass to vertex shader
    glm::mat4                                   _bonesTransforms[MAX_BONES];
    UniformBuffer                               _ubo;
};
