/**
* @Author   Guillaume Labey
*/

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

ModelInstance::~ModelInstance()
{
    if (_buffer)
    {
        _buffer->free();
    }
}

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

BufferPool::SubBuffer*  ModelInstance::getBuffer() const
{
    return (_buffer);
}

BufferPool::SubBuffer*  ModelInstance::getBuffer(BufferPool* bufferPool)
{
    if (!_buffer)
    {
        _buffer = bufferPool->allocate();
    }
    return (_buffer);
}

void    ModelInstance::setBuffer(BufferPool::SubBuffer* buffer)
{
    _buffer = buffer;
}

void    ModelInstance::setMaterial(Material* material)
{
    for (auto& meshInstance: _meshsInstances)
    {
        meshInstance->setMaterial(material);
    }
}
