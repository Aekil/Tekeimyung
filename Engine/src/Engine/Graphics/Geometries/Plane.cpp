/**
* @Author   Guillaume Labey
*/

#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/ResourceManager.hpp>

#include <Engine/Graphics/Geometries/Plane.hpp>


Plane::Plane(Plane::sInfo& info): Geometry(Geometry::eType::PLANE)
{
    float width = info.width;
    float height = info.height;

    // Plane mesh
    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(this);
    mesh->vertexs = {
        //1. pos
        //2. color
        //3. normal
        //4. texture uv
        {glm::vec3(-width / 2.0f, height / 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},  // Top Left
        {glm::vec3(width / 2.0f, height / 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},  // Top Right
        {glm::vec3(-width / 2.0f, -height / 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},  // Bottom Left
        {glm::vec3(width / 2.0f, -height / 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)}  // Bottom Right
    };

    mesh->indices = {
        0, 2, 3,
        3, 1, 0
    };

    // Plane material
    Material* material = ResourceManager::getInstance()->getResource<Material>("geometry_default.mat");
    ASSERT(material != nullptr, "geometry_default.mat should exists");
    mesh->setMaterial(material);

    // Add plane to meshs list
    _meshs.push_back(std::move(mesh));

    initVertexData();
    initIndexData();
    _buffer.updateData(_vertexData, getVertexsSize(), _indexData, getIndicesSize());
    calculateSize();

    setTexture(info.texturePath);
}

Plane::~Plane() {}

void    Plane::setTexture(const std::string& texture)
{
    // Plane texture
    if (texture.length() == 0)
        return;

    Material* material = getMaterial();
    material->setTexture(Texture::eType::AMBIENT, ResourceManager::getInstance()->getOrLoadResource<Texture>(texture));
}
