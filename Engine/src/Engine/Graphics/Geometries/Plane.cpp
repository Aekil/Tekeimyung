#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/RessourceManager.hpp>

#include <Engine/Graphics/Geometries/Plane.hpp>


Plane::Plane(Plane::sInfo& info): Geometry(Geometry::eType::PLANE)
{
    float width = info.width;
    float height = info.height;

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
        0, 2, 1,
        1, 2, 3
    };

    // Plane material
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

    // Plane texture
    if (info.texturePath.length() > 0)
    {
        ASSERT(_meshs.size() == 1, "A plane should have 1 mesh");

        Material& material = _meshs[0]->material;

       material._constants.texturesTypes |= Texture::eType::AMBIENT;
       material._textures[Texture::eType::AMBIENT] = &RessourceManager::getInstance()->getTexture(info.texturePath);
       material._needUpdate = true;

       // Set diffuse to 0.0 or the texture transparency won't work
       material._constants.diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
}

Plane::~Plane() {}
