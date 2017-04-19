/**
* @Author   Guillaume Labey
*/

#pragma once

#include <unordered_map>
#include <memory>

#include <ECS/System.hpp>

#include <Engine/Graphics/BufferPool.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/ModelInstance.hpp>
#include <Engine/Graphics/RenderQueue.hpp>

#include <Engine/Components.hh>
#include <Engine/Systems/ParticleSystem.hpp>

#define INSTANCING_MAX 400

START_SYSTEM(RenderingSystem)
    struct sBatch {
        ~sBatch()
        {
/*            if (buffer != nullptr)
            {
                _bufferPool->free(buffer);
            }*/
        }

        BufferPool::SubBuffer* buffer{nullptr};
        uint32_t instances{0};
        Material* material{nullptr};
        Mesh* mesh{nullptr};
        MeshInstance* meshInstance{nullptr};
        bool dynamic{false};
        bool hideDynamic{false};
    };

public:
    RenderingSystem(std::unordered_map<uint32_t, sEmitter*>* particleEmitters);
    ~RenderingSystem() override final;

    void update(EntityManager& em, float elapsedTime) override final;

    void                                    attachCamera(Camera* camera);

private:
    void                                    addParticlesToRenderQueue(EntityManager& em, float elapsedTime);
    void                                    addCollidersToRenderQueue(Entity* entity, sTransformComponent* transform);
    void                                    addLightConeToRenderQueue(sLightComponent* lightComp, sTransformComponent* transform);

    void                                    addCameraViewPerspectiveToRenderQueue(sCameraComponent* cameraComp, sTransformComponent* transform);
    void                                    addCameraViewOrthoGraphicToRenderQueue(sCameraComponent* cameraComp, sTransformComponent* transform);
    void                                    addCameraViewToRenderQueue(sCameraComponent* cameraComp, sTransformComponent* transform);

    void                                    addBatch(sTransformComponent* transform, sRenderComponent* render);
    BufferPool::SubBuffer*                  getModelBuffer(sTransformComponent* transform, sRenderComponent* render);
    void                                    updateModelBuffer(BufferPool::SubBuffer* buffer, const glm::mat4& transform, const glm::vec4& color);

private:
    std::unordered_map<uint32_t, sEmitter*>*    _particleEmitters;

    RenderQueue                                 _renderQueue;

    static bool _displayAllColliders;
    static std::unique_ptr<BufferPool>          _bufferPool;
    static std::unique_ptr<BufferPool>          _batchesBufferPool;

    std::vector<sBatch> _batches;

    // A camera can be attached to the RenderSystem
    // If no camera is attached, it will use the camera of an entity with sCameraComponent
    Camera*                                     _camera{nullptr};
END_SYSTEM(RenderingSystem)
