/**
* @Author   Guillaume Labey
*/

#pragma once

#include <vector>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <Engine/Graphics/Light.hpp>
#include <Engine/Graphics/MeshInstance.hpp>
#include <Engine/Graphics/ModelInstance.hpp>
#include <Engine/Graphics/UI/Text.hpp>
#include <Engine/Graphics/UniformBuffer.hpp>
#include <Engine/Debug/Logger.hpp>

#define MAX_RENDERABLE_MESHS 4000
#define MAX_LIGHTS 50
#define CHECK_QUEUE_NOT_FULL(queueSize)                                                                 \
        if (queueSize >= MAX_RENDERABLE_MESHS)                                                          \
        {                                                                                               \
            LOG_WARN("RenderQueue::addModel/addUIModel:: Can't add mesh to queue, the max number of meshs in the queue has been reached"); \
            return;                                                                                     \
        }

struct sRenderableMesh {
    MeshInstance* meshInstance;
    UniformBuffer* ubo;
    uint32_t uboOffset;
    uint32_t uboSize;
    uint32_t instancesNb;

    // Layer to sort UI
    int layer;

    bool dynamic;

    // If true and dynamic is false,
    // all dynamic objects behind it will not be seen with transparency
    bool hideDynamic;
};

struct sRenderableText {
    Text text;

    // Layer to sort UI
    int layer;

    glm::vec2 pos;
};

class RenderQueue {
public:
    RenderQueue() = default;
    ~RenderQueue() = default;

    void                            addModel(ModelInstance* modelInstance, UniformBuffer* ubo,
                                                uint32_t uboOffset = 0,
                                                uint32_t uboSize = 0,
                                                uint32_t instancesNb = 0,
                                                bool dynamic = false,
                                                bool hideDynamic = false);
    void                            addMesh(MeshInstance* meshInstance, UniformBuffer* ubo,
                                                uint32_t uboOffset = 0,
                                                uint32_t uboSize = 0,
                                                uint32_t instancesNb = 0,
                                                bool dynamic = false,
                                                bool hideDynamic = false);
    void                            addUIModel(ModelInstance* modelInstance,
                                                UniformBuffer* ubo,
                                                int layer,
                                                uint32_t uboOffset = 0,
                                                uint32_t uboSize = 0,
                                                uint32_t instancesNb = 0);
    void                            addText(const Text& text,
                                                int layer,
                                                const glm::vec2& pos);
    void                            addLight(Light* light);
    void                            clear();

    std::vector<sRenderableMesh>& getOpaqueMeshs();
    uint32_t getOpaqueMeshsNb() const;

    std::vector<sRenderableMesh>& getTransparentMeshs();
    uint32_t getTransparentMeshsNb() const;

    std::vector<sRenderableMesh>& getUIOpaqueMeshs();
    uint32_t getUIOpaqueMeshsNb() const;

    std::vector<sRenderableMesh>& getUITransparentMeshs();
    uint32_t getUITransparentMeshsNb() const;

    std::vector<sRenderableText>& getTexts();
    uint32_t getTextsNb() const;

    std::vector<Light*>& getLights();
    uint32_t getLightsNb() const;

private:
    std::vector<sRenderableMesh>    _opaqueMeshs{MAX_RENDERABLE_MESHS};// TODO: Change static size
    uint32_t                        _opaqueMeshsNb{0};

    std::vector<sRenderableMesh>    _transparentMeshs{MAX_RENDERABLE_MESHS};// TODO: Change static size
    uint32_t                        _transparentMeshsNb{0};

    std::vector<sRenderableMesh>    _uiOpaqueMeshs{MAX_RENDERABLE_MESHS};// TODO: Change static size
    uint32_t                        _uiOpaqueMeshsNb{0};

    std::vector<sRenderableMesh>    _uiTransparentMeshs{MAX_RENDERABLE_MESHS};// TODO: Change static size
    uint32_t                        _uiTransparentMeshsNb{0};

    std::vector<sRenderableText>    _texts{MAX_RENDERABLE_MESHS};// TODO: Change static size
    uint32_t                        _textsNb{0};

    std::vector<Light*>             _lights{MAX_LIGHTS};// TODO: Change static size
    uint32_t                        _lightsNb{0};
};
