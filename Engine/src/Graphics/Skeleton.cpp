#include <iostream>
#include "Graphics/Skeleton.hpp"

Skeleton::Skeleton() {}

Skeleton::~Skeleton() {}

Skeleton::sBone*  Skeleton::getBoneByName(const std::string& name)
{
    auto boneId = _bonesIds.find(name);

    if (boneId == _bonesIds.end())
        return (nullptr);

    return &_bones[boneId->second];
}

void    Skeleton::addBone(const std::string& name, const glm::mat4& offset)
{
    std::cout << "ADD Bone " << name.c_str() << std::endl;
    Skeleton::sBone bone;

    bone.name = name;
    bone.offset = offset;
    bone.id = static_cast<uint32_t>(_bones.size());
    _bones.push_back(bone);
    _bonesIds[name] = bone.id;
}
