/**
* @Author   Guillaume Labey
*/

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/ModelInstance.hpp>


ModelInstance::ModelInstance(Model* model) : _model(model)
{
    uint32_t i = 0;
    auto& meshs = _model->getMeshs();

    _materials.resize(meshs.size());
    for (auto& mesh: meshs)
    {
        _materials[i++] = mesh->getMaterial();
    }
}

ModelInstance::ModelInstance(const ModelInstance& modelInstance)
{
    _model = modelInstance._model;
    _materials = modelInstance._materials;
}

ModelInstance::~ModelInstance() {}

ModelInstance& ModelInstance::operator=(const ModelInstance& modelInstance)
{
    _model = modelInstance._model;
    _materials = modelInstance._materials;
    return (*this);
}

void    ModelInstance::draw(const ShaderProgram& shaderProgram, const glm::vec4& color, const glm::mat4 transform) const
{
    // Model matrix
    static GLint uniModel = shaderProgram.getUniformLocation("model");
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(transform));

    // Model color
    static GLint colorModel = shaderProgram.getUniformLocation("modelColor");
    glUniform4f(colorModel, color.x, color.y, color.z, color.w);

    // Bind buffer
    _model->getBuffer().bind();

    auto& meshs = _model->getMeshs();
    uint32_t i = 0;
    for (auto &&material: _materials)
    {
        material->bind(shaderProgram);

        // Draw to screen
        glDrawElements(_model->getPrimitiveType(), (GLuint)meshs[i]->indices.size(), GL_UNSIGNED_INT, BUFFER_OFFSET((GLuint)meshs[i]->idxOffset * sizeof(GLuint)));
        ++i;
    }
}

const Model*    ModelInstance::getModel() const
{
    return (_model);
}

Model*  ModelInstance::getModel()
{
    return (_model);
}

std::vector<Material*>& ModelInstance::getMaterials()
{
    return (_materials);
}
