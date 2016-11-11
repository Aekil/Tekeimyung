#include <iostream>
#include <cstring>

#include "Graphics/Skeleton.hpp"

Skeleton::Skeleton()
{
    std::memset(_bonesTransforms, 0, sizeof(_bonesTransforms));
    _ubo.setBindingPoint(2);
}

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
    bone.finalTransform = glm::mat4(1.0);
    _bones.push_back(bone);
    _bonesIds[name] = bone.id;
}

void    Skeleton::updateUniformBuffer()
{
    for (auto &&bone: _bones)
    {
        _bonesTransforms[bone.id] = bone.finalTransform;
    }
    _ubo.update(_bonesTransforms, sizeof(_bonesTransforms));
}

const UniformBuffer&    Skeleton::getUbo() const
{
    return _ubo;
}
