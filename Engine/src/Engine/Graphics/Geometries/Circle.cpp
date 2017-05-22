/**
* @Author   Guillaume Labey
*/

#include <Engine/Debug/Debug.hpp>
#include <Engine/Utils/ResourceManager.hpp>

#include <Engine/Graphics/Geometries/Circle.hpp>

Circle::Circle(Circle::sInfo& info): Geometry(Geometry::eType::CIRCLE)
{
    float outerRadius = info.outerRadius;
    float innerRadius = info.innerRadius / 2.0f;
    uint32_t numSegments = 360;

    // Circle mesh
    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(this);


    for (uint32_t i = 0; i <= numSegments; ++i)
    {
        float const theta = 2.0f * glm::pi<float>() * (float)i / (float)numSegments;

        Vertex innerVertex;
        Vertex outerVertex;

        innerVertex.uv = glm::vec2(0.0f, 0.0f);
        innerVertex.pos = glm::vec3(innerRadius * (float)cos(theta), 0.0f, innerRadius * (float)sin(theta));
        innerVertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        innerVertex.color = glm::vec3(0.0f, 0.0f, 0.0f);

        outerVertex.uv = glm::vec2(0.0f, 0.0f);
        outerVertex.pos = glm::vec3(outerRadius * (float)cos(theta), 0.0f, outerRadius * (float)sin(theta));
        outerVertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        outerVertex.color = glm::vec3(0.0f, 0.0f, 0.0f);

        mesh->vertexs.push_back(outerVertex);
        mesh->vertexs.push_back(innerVertex);
        mesh->indices.push_back(i * 2);
        mesh->indices.push_back(i * 2 + 1);
    }


    // Circle material
    Material* material = ResourceManager::getInstance()->getResource<Material>("geometry_default.mat");
    ASSERT(material != nullptr, "geometry_default.mat should exists");
    mesh->setMaterial(material);

    // Add plane to meshs list
    _meshs.push_back(std::move(mesh));

    initVertexData();
    initIndexData();
    _buffer.updateData(_vertexData, getVertexsSize(), _indexData, getIndicesSize());
    calculateSize();

    _primitiveType = GL_TRIANGLE_STRIP;
}

Circle::~Circle() {}
