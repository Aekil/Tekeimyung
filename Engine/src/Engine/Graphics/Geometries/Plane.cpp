#include <Engine/Graphics/Geometries/Plane.hpp>


Plane::Plane(): Plane(50, 50) {}

Plane::Plane(uint32_t width, uint32_t height)
{
    // Plane mesh
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
    mesh->vertexs = {
        //1. pos
        //2. color
        //3. normal
        //4. texture uv
        {glm::vec3(0.0f, height, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},  // Top Left
        {glm::vec3(width, height, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},  // Top Right
        {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},  // Bottom Left
        {glm::vec3(width, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)}  // Bottom Right
    };

    mesh->indices = {
        0, 1, 2,
        1, 2, 3
    };

    // Plane material
    Material material;
    material._constants.ambient = glm::vec3(0.611765f, 0.392157f, 0.172549f);
    material._constants.diffuse = glm::vec3(0.611765f, 0.392157f, 0.172549f);
    mesh->material = material;

    // Add plane to meshs list
    _meshs.push_back(mesh);

    initVertexData();
    initIndexData();
    _buffer.updateData(_vertexData, getVertexsSize(), _indexData, getIndicesSize());
}

Plane::~Plane() {}
