#include "Graphics/Material.hpp"

Material::Material(): diffuse(1.0f, 1.0f, 1.0f) {}

bool    Material::loadFromAssimp(aiMaterial* material)
{
    aiColor3D color;

    if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
        diffuse = glm::vec3(color.r, color.g, color.b);
    }

    return (true);
}

void    Material::bind(ShaderProgram& shaderProgram) const
{
    // Bind diffuse
    GLint uniColor = shaderProgram.getUniformLocation("color");
    glUniform3f(uniColor, diffuse.x, diffuse.y, diffuse.z);
}
