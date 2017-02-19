/**
* @Author   Guillaume Labey
*/

#pragma once

#include <vector>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <Engine/Graphics/MeshInstance.hpp>
#include <Engine/Graphics/ModelInstance.hpp>
#include <Engine/Utils/Logger.hpp>

#define MAX_RENDERABLE_MESHS 4000
#define CHECK_QUEUE_NOT_FULL(queueSize)                                                                 \
        if (queueSize >= MAX_RENDERABLE_MESHS)                                                          \
        {                                                                                               \
            LOG_WARN("RenderQueue::addModelInstance:: Can't add mesh to queue, the max number of meshs in the queue has been reached"); \
            return;                                                                                     \
        }

struct sRenderableMesh {
    MeshInstance* meshInstance;
    glm::vec4 color;
    glm::mat4 transform;
};

class RenderQueue {
public:
    RenderQueue() = default;
    ~RenderQueue() = default;

    void                            addModel(ModelInstance* modelInstance, const glm::vec4& color, const glm::mat4& transform);
    void                            addUIModel(ModelInstance* modelInstance, const glm::vec4& color, const glm::mat4& transform);
    void                            clear();

    std::vector<sRenderableMesh>& getOpaqueMeshs();
    uint32_t getOpaqueMeshsNb() const;

    std::vector<sRenderableMesh>& getTransparentMeshs();
    uint32_t getTransparentMeshsNb() const;

    std::vector<sRenderableMesh>& getUIOpaqueMeshs();
    uint32_t getUIOpaqueMeshsNb() const;

    std::vector<sRenderableMesh>& getUITransparentMeshs();
    uint32_t getUITransparentMeshsNb() const;

private:
    std::vector<sRenderableMesh>    _opaqueMeshs{MAX_RENDERABLE_MESHS};// TODO: Change static size
    uint32_t                        _opaqueMeshsNb{0};

    std::vector<sRenderableMesh>    _transparentMeshs{MAX_RENDERABLE_MESHS};// TODO: Change static size
    uint32_t                        _transparentMeshsNb{0};

    std::vector<sRenderableMesh>    _uiOpaqueMeshs{MAX_RENDERABLE_MESHS};// TODO: Change static size
    uint32_t                        _uiOpaqueMeshsNb{0};

    std::vector<sRenderableMesh>    _uiTransparentMeshs{MAX_RENDERABLE_MESHS};// TODO: Change static size
    uint32_t                        _uiTransparentMeshsNb{0};
};