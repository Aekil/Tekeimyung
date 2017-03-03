/**
* @Author   Guillaume Labey
*/

#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/ResourceManager.hpp>

#include <Engine/Graphics/Geometries/Box.hpp>

Box::Box(Box::sInfo& info): Geometry(Geometry::eType::BOX)
{
    float width = info.width;
    float height = info.height;
    float length = info.length;

    // Box mesh
    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(this);
    mesh->vertexs = {
        //1. pos
        //2. color
        //3. normal
        //4. texture uv

        // Front
        {glm::vec3(-width / 2.0f, height / 2.0f, -length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(width / 2.0f, height / 2.0f, -length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(-width / 2.0f, -height / 2.0f, -length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(width / 2.0f, -height / 2.0f, -length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},

        // Back
        {glm::vec3(-width / 2.0f, height / 2.0f, length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(width / 2.0f, height / 2.0f, length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-width / 2.0f, -height / 2.0f, length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(width / 2.0f, -height / 2.0f, length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},

        // Left
        {glm::vec3(-width / 2.0f, height / 2.0f, -length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(-width / 2.0f, height / 2.0f, length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-width / 2.0f, -height / 2.0f, -length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-width / 2.0f, -height / 2.0f, length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},

        // Right
        {glm::vec3(width / 2.0f, height / 2.0f, -length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(width / 2.0f, height / 2.0f, length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(width / 2.0f, -height / 2.0f, -length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(width / 2.0f, -height / 2.0f, length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},

        // Top
        {glm::vec3(-width / 2.0f, height / 2.0f, -length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-width / 2.0f, height / 2.0f, length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(width / 2.0f, height / 2.0f, -length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(width / 2.0f, height / 2.0f, length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},

        // Bottom
        {glm::vec3(-width / 2.0f, -height / 2.0f, -length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-width / 2.0f, -height / 2.0f, length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(width / 2.0f, -height / 2.0f, -length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(width / 2.0f, -height / 2.0f, length / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)}
    };

    mesh->indices = {
        // Front
        0, 1, 3,
        3, 2, 0,

        // Left
        8, 10, 11,
        11, 9, 8,

        // Back
        4, 6, 7,
        7, 5, 4,

        // Right
        13, 15, 14,
        14, 12, 13,

        // Bottom
        23, 21, 20,
        20, 22, 23,

        // Top
        19, 18, 16,
        16, 17, 19,
    };

    // Box material
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

Box::~Box() {}
