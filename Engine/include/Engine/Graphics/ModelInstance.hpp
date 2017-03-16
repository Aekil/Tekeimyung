/**
* @Author   Guillaume Labey
*/

#pragma once

# include <vector>
# include <Engine/Graphics/Material.hpp>
# include <Engine/Graphics/Model.hpp>
# include <Engine/Graphics/MeshInstance.hpp>
# include <Engine/Graphics/BufferPool.hpp>

class ModelInstance
{
public:
    ModelInstance(Model* model);
    ModelInstance(const ModelInstance& modelInstance);
    ~ModelInstance();

    ModelInstance& operator=(const ModelInstance& modelInstance);

    const Model*                getModel() const;
    Model*                      getModel();
    std::vector<std::unique_ptr<MeshInstance> >&  getMeshsInstances();

    BufferPool::SubBuffer*      getBuffer() const;
    BufferPool::SubBuffer*      getBuffer(BufferPool* bufferPool);
    void                        setBuffer(BufferPool::SubBuffer* buffer);
    void                        setMaterial(Material* material);

private:
    Model*                      _model;
    std::vector<std::unique_ptr<MeshInstance> >   _meshsInstances;
    BufferPool::SubBuffer*      _buffer{nullptr};
};
