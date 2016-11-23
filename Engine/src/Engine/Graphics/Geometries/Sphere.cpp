#include <Engine/Graphics/Geometries/Sphere.hpp>

Sphere::Sphere(Sphere::sInfo& info): Geometry(Geometry::eType::SPHERE)
{
    float radius = info.radius;
    uint32_t rings = static_cast<uint32_t>(radius * 3.0f);
    uint32_t sectors = static_cast<uint32_t>(radius * 3.0f);

    // Sphere mesh
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();



    float const R = 1.0f / (float)(rings - 1.0f);
    float const S = 1.0f / (float)(sectors - 1.0f);
    int r, s;
    int i = 0;

    mesh->vertexs.resize(rings * sectors);
    for(r = 0; r < (int)rings; r++) for(s = 0; s < (int)sectors; s++) {
            float const y = sin(-(glm::pi<float>() / 2.0f) + glm::pi<float>() * r * R);
            float const x = cos(2.0f * glm::pi<float>() * s * S) * sin(glm::pi<float>() * r * R);
            float const z = sin(2.0f * glm::pi<float>() * s * S) * sin(glm::pi<float>() * r * R);

            auto &&vertex = mesh->vertexs[i];

            vertex.uv = glm::vec2(s * S, r * R);
            vertex.pos = glm::vec3(x * radius, y * radius, z * radius);
            vertex.normal = glm::vec3(x, y, z);
            vertex.color = glm::vec3(0.0f, 0.0f, 0.0f);

            i++;
    }

    mesh->indices.resize(rings * sectors * 4);
    auto j = mesh->indices.begin();
    for(r = 0; r < (int)rings; r++) for(s = 0; s < (int)sectors; s++) {
            *j++ = static_cast<uint32_t>(r * sectors + s);
            *j++ = static_cast<uint32_t>(r * sectors + (s + 1));
            *j++ = static_cast<uint32_t>((r + 1) * sectors + (s + 1));
            *j++ = static_cast<uint32_t>((r + 1) * sectors + s);
    }




    // Sphere material
    Material material;
    material._constants.ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    material._constants.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    mesh->material = material;

    // Add plane to meshs list
    _meshs.push_back(mesh);

    initVertexData();
    initIndexData();
    _buffer.updateData(_vertexData, getVertexsSize(), _indexData, getIndicesSize());
    calculateSize();
}

Sphere::~Sphere() {}
