/**
* @Author   Guillaume Labey
*/

#include <Engine/Debug/Debug.hpp>
#include <Engine/Utils/ResourceManager.hpp>

#include <Engine/Graphics/Geometries/Cone.hpp>

glm::vec3 calculateSurfaceNormal(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
    glm::vec3 u = p2 - p1;
    glm::vec3 v = p3 - p1;

    return glm::cross(u, v);
}

Cone::Cone(Cone::sInfo& info): Geometry(Geometry::eType::CONE)
{
    float bottomRadius = info.bottomRadius;
    float height = info.height;
    uint32_t numSegments = 50;

    // Cone mesh
    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(this);



    // Base circle
    {
        Vertex centerVertex;
        centerVertex.uv = glm::vec2(0.0f, 0.0f);
        centerVertex.pos = glm::vec3(0.0f, 0.0f, 0.0f);
        centerVertex.color = glm::vec3(0.0f, 0.0f, 0.0f);
        centerVertex.normal = glm::vec3(0.0f, -1.0f, 0.0f);
        mesh->vertexs.push_back(centerVertex);

        for (uint32_t i = 0; i <= numSegments; ++i)
        {
            float rad = glm::radians((float)i / (float)numSegments * 360.0f);

            Vertex vertex;

            vertex.uv = glm::vec2(0.0f, 0.0f);
            vertex.pos = glm::vec3(cos(rad) * bottomRadius, 0.0f, sin(rad) * bottomRadius);
            vertex.normal = glm::vec3(0.0f, -1.0f, 0.0f);
            vertex.color = glm::vec3(0.0f, 0.0f, 0.0f);

            mesh->vertexs.push_back(vertex);

            mesh->indices.push_back(i + 2);
            mesh->indices.push_back(0);
            mesh->indices.push_back(i + 1);

        }
    }

    // Cone
    {
        uint32_t verticesOffset = (uint32_t)mesh->vertexs.size();
        Vertex topVertex;
        topVertex.uv = glm::vec2(0.0f, 0.0f);
        topVertex.pos = glm::vec3(0.0f, height, 0.0f);
        topVertex.color = glm::vec3(0.0f, 0.0f, 0.0f);
        topVertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);

        for (uint32_t i = 0; i <= numSegments; ++i)
        {
            float rad = glm::radians((float)i / (float)numSegments * 360.0f);

            Vertex vertex;

            vertex.uv = glm::vec2(0.0f, 0.0f);
            vertex.pos = glm::vec3(cos(rad) * bottomRadius, 0.0f, sin(rad) * bottomRadius);
            vertex.color = glm::vec3(0.0f, 0.0f, 0.0f);
            vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);

            mesh->vertexs.push_back(vertex);
            mesh->vertexs.push_back(topVertex);

            mesh->indices.push_back((uint32_t)mesh->vertexs.size() - 1);

            if (i == numSegments)
            {
                mesh->indices.push_back(verticesOffset);
            }
            else
            {
                mesh->indices.push_back(verticesOffset + (i * 2) + 2);
            }

            mesh->indices.push_back(verticesOffset + (i * 2));

        }

        // Calculate normals
        Vertex* vertices = mesh->vertexs.data() + verticesOffset;
        uint32_t verticesNb = (uint32_t)mesh->vertexs.size() - verticesOffset;
        for (uint32_t i = 0; i < verticesNb;)
        {
            Vertex& p1 = vertices[i % verticesNb];
            Vertex& p2 = vertices[(i + 1) % verticesNb];
            Vertex& p3 = vertices[(i + 2) % verticesNb];

            p1.normal = calculateSurfaceNormal(p1.pos, p2.pos, p3.pos);
            p2.normal = p1.normal;
            p3.normal = p1.normal;
            i += 3;
        }
    }


    // Cone material
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

Cone::~Cone() {}
