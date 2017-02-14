/**
* @Author   Guillaume Labey
*/

#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/RessourceManager.hpp>

#include <Engine/Graphics/Geometries/Plane.hpp>


Plane::Plane(Plane::sInfo& info): Geometry(Geometry::eType::PLANE)
{
    float width = info.width;
    float height = info.height;

    // Plane mesh
    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
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
        0, 2, 1,
        1, 2, 3
    };

    // Plane material
    Material material;
    material._constants.ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    material._constants.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    mesh->material = material;

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

    Material& material = getMaterial();

   material._constants.texturesTypes |= Texture::eType::AMBIENT;
   material._textures[Texture::eType::AMBIENT] = RessourceManager::getInstance()->getResource<Texture>(texture);
   material._needUpdate = true;

   // Set diffuse to 0.0 or the texture transparency won't work
   material._constants.diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
}
