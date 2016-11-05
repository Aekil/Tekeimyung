#include "Graphics/Material.hpp"

Material::Material(): _needUpdate(true)
{
    _constants.diffuse = {0.0f, 0.0f, 0.0f};
    _constants.ambient = {0.0f, 0.0f, 0.0f};
}

bool    Material::loadFromAssimp(aiMaterial* material)
{
    aiColor3D color;

    if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
        _constants.diffuse = glm::vec3(color.r, color.g, color.b);
    }
    if (material->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS) {
        _constants.ambient = glm::vec3(color.r, color.g, color.b);
    }


    return (true);
}

void    Material::bind(const ShaderProgram& shaderProgram)
{
    if (_needUpdate)
    {
        _ubo.update(&_constants, sizeof(_constants));
        _needUpdate = false;
    }

    // Bind ubo
    _ubo.bind(shaderProgram, "material");
}
