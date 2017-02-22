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
#include <Engine/Graphics/Camera.hpp>
#include <Engine/Graphics/RenderQueue.hpp>

#include <Engine/Components.hh>
#include <Engine/Systems/ParticleSystem.hpp>

START_SYSTEM(RenderingSystem)
public:
    RenderingSystem(Camera* camera, std::unordered_map<uint32_t, sEmitter*>* particleEmitters);
    virtual ~RenderingSystem();

    virtual void update(EntityManager& em, float elapsedTime);
    virtual bool                            init();

private:
    void                                    addParticlesToRenderQueue(EntityManager& em, float elapsedTime);
    void                                    addCollidersToRenderQueue(Entity* entity, sTransformComponent* transform);
    void                                    addLightConeToRenderQueue(sLightComponent* lightComp, sTransformComponent* transform);

    BufferPool::SubBuffer*                  getModelBuffer(sTransformComponent* transform, sRenderComponent* render);
    void                                    updateModelBuffer(BufferPool::SubBuffer* buffer, const glm::mat4& transform, const glm::vec4& color);

private:
    std::unordered_map<uint32_t, sEmitter*>*    _particleEmitters;

    Camera*                                     _camera;
    Camera                                      _defaultCamera;

    RenderQueue                                 _renderQueue;

    static bool _displayAllColliders;
    static std::unique_ptr<BufferPool>          _bufferPool;
END_SYSTEM(RenderingSystem)
