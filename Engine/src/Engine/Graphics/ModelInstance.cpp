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

    _meshsInstances.resize(meshs.size());
    for (auto& mesh: meshs)
    {
        _meshsInstances[i++] = std::make_unique<MeshInstance>(mesh.get());
    }
}

ModelInstance::ModelInstance(const ModelInstance& modelInstance)
{
    _model = modelInstance._model;

    _meshsInstances.resize(modelInstance._meshsInstances.size());
    uint32_t i = 0;
    for (const auto& meshInstance: modelInstance._meshsInstances)
    {
        _meshsInstances[i++] = std::make_unique<MeshInstance>(*meshInstance);
    }
}

ModelInstance::~ModelInstance() {}

ModelInstance& ModelInstance::operator=(const ModelInstance& modelInstance)
{
    _model = modelInstance._model;

    _meshsInstances.resize(modelInstance._meshsInstances.size());
    uint32_t i = 0;
    for (const auto& meshInstance: modelInstance._meshsInstances)
    {
        _meshsInstances[i++] = std::make_unique<MeshInstance>(*meshInstance);
    }

    return (*this);
}

void    ModelInstance::draw(const ShaderProgram& shaderProgram, const glm::vec4& color, const glm::mat4 transform)
{
    // Model matrix
    static GLint uniModel = shaderProgram.getUniformLocation("model");
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(transform));

    // Model color
    static GLint colorModel = shaderProgram.getUniformLocation("modelColor");
    glUniform4f(colorModel, color.x, color.y, color.z, color.w);

    // Bind buffer
    _model->getBuffer().bind();

    for (auto& meshInstance: _meshsInstances)
    {
        meshInstance->getMaterial()->bind(shaderProgram);
        Mesh* mesh = meshInstance->getMesh();

        // Draw to screen
        glDrawElements(_model->getPrimitiveType(), (GLuint)mesh->indices.size(), GL_UNSIGNED_INT, BUFFER_OFFSET((GLuint)mesh->idxOffset * sizeof(GLuint)));
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

std::vector<std::unique_ptr<MeshInstance> >& ModelInstance::getMeshsInstances()
{
    return (_meshsInstances);
}
