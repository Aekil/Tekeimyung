/**
* @Author   Guillaume Labey
*/

#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/ResourceManager.hpp>

#include <Engine/Graphics/Geometries/Cylinder.hpp>

Cylinder::Cylinder(Cylinder::sInfo& info): Geometry(Geometry::eType::CYLINDER)
{
    float radius = info.radius;
    float height = info.height;
    uint32_t numSegments = 50;

    // Cylinder mesh
    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(this);

    const float stepTheta = (2.0f * glm::pi<float>()) / numSegments;
    const float stepU = 1.0f / numSegments;
    const uint32_t verticesPerCircle = numSegments + 1;
    const uint32_t trisPerCap = numSegments - 2;
    const uint32_t firstSidePos = verticesPerCircle + verticesPerCircle;

    float curTheta = 0;
    std::vector<float> halfCosThetas(verticesPerCircle);
    std::vector<float> halfSinThetas(verticesPerCircle);
    for (uint32_t i = 0; i < verticesPerCircle; ++i)
    {
        halfCosThetas[i] = cos(curTheta) * radius;
        halfSinThetas[i] = sin(curTheta) * radius;
        curTheta += stepTheta;
    }

    // Top cap
    for (uint32_t i = 0; i < verticesPerCircle; ++i)
    {
        Vertex vertex;
        vertex.uv = glm::vec2(halfCosThetas[i] + 0.5, halfSinThetas[i] + 0.5);
        vertex.pos = glm::vec3(halfCosThetas[i], height / 2.0f, halfSinThetas[i]);
        vertex.color = glm::vec3(0.0f, 0.0f, 0.0f);
        vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        mesh->vertexs.push_back(vertex);
    }
    for (uint32_t i = 0; i < trisPerCap; ++i)
    {
        mesh->indices.push_back(i + 2);
        mesh->indices.push_back(i + 1);
        mesh->indices.push_back(0);
    }

    // Bottom cap
    for (uint32_t i = 0; i < verticesPerCircle; ++i)
    {
        Vertex vertex;
        vertex.uv = glm::vec2(halfCosThetas[i] + 0.5, halfSinThetas[i] + 0.5);
        vertex.pos = glm::vec3(halfCosThetas[i], -height / 2.0f, halfSinThetas[i]);
        vertex.color = glm::vec3(0.0f, 0.0f, 0.0f);
        vertex.normal = glm::vec3(0.0f, -1.0f, 0.0f);
        mesh->vertexs.push_back(vertex);
    }
    for (uint32_t i = 0; i < trisPerCap; ++i)
    {
        mesh->indices.push_back(verticesPerCircle);
        mesh->indices.push_back(verticesPerCircle + i + 1);
        mesh->indices.push_back(verticesPerCircle + i + 2);
    }

    // Top cap (for the sides)
    float curU = 1.0f;
    for (uint32_t i = 0; i < verticesPerCircle; ++i)
    {
        Vertex vertex;
        vertex.uv = glm::vec2(curU, 0.0f);
        vertex.pos = glm::vec3(halfCosThetas[i], height / 2.0f, halfSinThetas[i]);
        vertex.color = glm::vec3(0.0f, 0.0f, 0.0f);
        vertex.normal = -vertex.pos;
        mesh->vertexs.push_back(vertex);

        curU -= stepU;
    }

    // Bottom cap (for the sides)
    curU = 1.0f;
    for (uint32_t i = 0; i < verticesPerCircle; ++i)
    {
        Vertex vertex;
        vertex.uv = glm::vec2(curU, 1.0f);
        vertex.pos = glm::vec3(halfCosThetas[i], -height / 2.0f, halfSinThetas[i]);
        vertex.color = glm::vec3(0.0f, 0.0f, 0.0f);
        vertex.normal = -vertex.pos;
        mesh->vertexs.push_back(vertex);

        curU -= stepU;
    }
    // Sides (excep the last quad)
    for (uint32_t i = 0; i < numSegments; ++i)
    {
        mesh->indices.push_back(firstSidePos + i);
        mesh->indices.push_back(firstSidePos + i + 1);
        mesh->indices.push_back(firstSidePos + verticesPerCircle + i + 1);

        mesh->indices.push_back(firstSidePos + i);
        mesh->indices.push_back(firstSidePos + verticesPerCircle + i + 1);
        mesh->indices.push_back(firstSidePos + verticesPerCircle + i);
    }

    // Cylinder material
    Material* material = ResourceManager::getInstance()->getResource<Material>("geometry_default.mat");
    ASSERT(material != nullptr, "geometry_default.mat should exists");
    mesh->setMaterial(material);

    // Add plane to meshs list
    _meshs.push_back(std::move(mesh));

    initVertexData();
    initIndexData();
    _buffer.updateData(_vertexData, getVertexsSize(), _indexData, getIndicesSize());
    calculateSize();

    //_primitiveType = GL_LINE_STRIP;
}

Cylinder::~Cylinder() {}
