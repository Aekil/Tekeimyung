#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <glm/mat4x4.hpp>

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
    };

public:
    Skeleton();
    ~Skeleton();
    sBone*                                      getBoneByName(const std::string& name);
    void                                        addBone(const std::string& name, const glm::mat4& offset);

private:
    // Map to get bones ids with name
    std::unordered_map<std::string, uint32_t>   _bonesIds;

    // Bones list
    // Bone index match the bone id
    std::vector<Skeleton::sBone>                _bones;
};
