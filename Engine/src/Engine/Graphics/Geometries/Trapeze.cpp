/**
* @Author   Guillaume Labey
*/

#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/ResourceManager.hpp>

#include <Engine/Graphics/Geometries/Trapeze.hpp>

Trapeze::Trapeze(Trapeze::sInfo& info): Geometry(Geometry::eType::TRAPEZE)
{
    float height = info.height;
    Trapeze::sInfo::sTop top = info.top;
    Trapeze::sInfo::sBottom bottom = info.bottom;

    // Trapeze mesh
    // TODO: Put correct normal (Not plane normal)
    // TODO: Put correct uv
    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(this);
    mesh->vertexs = {
        //1. pos
        //2. color
        //3. normal
        //4. texture uv

        // Bottom => 0 offset
        {glm::vec3(-bottom.width / 2.0f, -height / 2.0f, -bottom.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-bottom.width / 2.0f, -height / 2.0f, bottom.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(bottom.width / 2.0f, -height / 2.0f, -bottom.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(bottom.width / 2.0f, -height / 2.0f, bottom.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},

        // Top => 4 offset
        {glm::vec3(-top.width / 2.0f, height / 2.0f, -top.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-top.width / 2.0f, height / 2.0f, top.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(top.width / 2.0f, height / 2.0f, -top.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(top.width / 2.0f, height / 2.0f, top.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},

        // Front => 8 offset
        {glm::vec3(-top.width / 2.0f, height / 2.0f, top.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(top.width / 2.0f, height / 2.0f, top.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-bottom.width / 2.0f, -height / 2.0f, bottom.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(bottom.width / 2.0f, -height / 2.0f, bottom.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-top.width / 2.0f, -height / 2.0f, bottom.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(top.width / 2.0f, -height / 2.0f, bottom.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},

        // Left => 14 offset
        {glm::vec3(-top.width / 2.0f, height / 2.0f, -top.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-top.width / 2.0f, height / 2.0f, top.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-bottom.width / 2.0f, -height / 2.0f, -bottom.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-bottom.width / 2.0f, -height / 2.0f, bottom.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-bottom.width / 2.0f, -height / 2.0f, -top.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-bottom.width / 2.0f, -height / 2.0f, top.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},

        // Back => 20 offset
        {glm::vec3(top.width / 2.0f, height / 2.0f, -top.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-top.width / 2.0f, height / 2.0f, -top.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(bottom.width / 2.0f, -height / 2.0f, -bottom.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-bottom.width / 2.0f, -height / 2.0f, -bottom.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(top.width / 2.0f, -height / 2.0f, -bottom.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-top.width / 2.0f, -height / 2.0f, -bottom.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},

        // Right => 26 offset
        {glm::vec3(top.width / 2.0f, height / 2.0f, top.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(top.width / 2.0f, height / 2.0f, -top.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(bottom.width / 2.0f, -height / 2.0f, bottom.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(bottom.width / 2.0f, -height / 2.0f, -bottom.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(bottom.width / 2.0f, -height / 2.0f, top.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(bottom.width / 2.0f, -height / 2.0f, -top.length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)}
    };

    mesh->indices = {
        // Bottom
        3, 1, 0,
        0, 2, 3,

        // Top
        7, 6, 4,
        4, 5, 7,

        // Front
        11, 9, 13,
        13, 9, 8,
        8, 12, 13,
        12, 8, 10,

        // Left
        17, 15, 19,
        19, 15, 14,
        14, 18, 19,
        18, 14, 16,

        // Bottom
        23, 21, 25,
        25, 21, 20,
        20, 24, 25,
        24, 20, 22,

        // Right
        29, 27, 31,
        31, 27, 26,
        26, 30, 31,
        30, 26, 28
    };

    // Trapeze material
    Material* material = ResourceManager::getInstance()->getResource<Material>("geometry_default.mat");
    ASSERT(material != nullptr, "geometry_default.mat should exists");
    mesh->setMaterial(material);

    // Add plane to meshs list
    _meshs.push_back(std::move(mesh));

    initVertexData();
    initIndexData();
    _buffer.updateData(_vertexData, getVertexsSize(), _indexData, getIndicesSize());
    calculateSize();
}

Trapeze::~Trapeze() {}
