/**
* @Author   Guillaume Labey
*/

#pragma once

#include <unordered_map>
#include <memory>

#include <ECS/System.hpp>

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

    void                                    addParticlesToRenderQueue(EntityManager& em, float elapsedTime);
    void                                    addCollidersToRenderQueue(Entity* entity, sTransformComponent* transform);
    void                                    addLightConeToRenderQueue(sLightComponent* lightComp, sTransformComponent* transform);

private:
    std::unordered_map<uint32_t, sEmitter*>*    _particleEmitters;

    Camera*                                     _camera;
    Camera                                      _defaultCamera;

    RenderQueue                                 _renderQueue;

    static bool _displayAllColliders;
END_SYSTEM(RenderingSystem)
