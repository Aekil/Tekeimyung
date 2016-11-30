#pragma once

#include <unordered_map>
#include <memory>

#include <ECS/System.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/Model.hpp>
#include <Engine/Graphics/Camera.hpp>

#include <Game/Components.hh>
#include <Game/Map.hpp>
#include <Game/Systems/ParticleSystem.hpp>

#define RENDERING_SYSTEM_NAME "Rendering system"

START_SYSTEM(RenderingSystem)
public:
    RenderingSystem(Map* map, std::unordered_map<uint32_t, sEmitter*>* particleEmitters);
    virtual ~RenderingSystem();

    virtual void update(EntityManager& em, float elapsedTime);
    virtual bool                            init();
    void                                    renderEntity(Entity* entity);
    void                                    renderCollider(Entity* entity);
    void                                    renderParticles(EntityManager& em);
    const ShaderProgram&                    getShaderProgram() const;

private:
    bool                                    isTransparentEntity(Entity* entity) const;
    std::shared_ptr<Model>                  getModel(Entity* entity);

private:
    // Shader program
    ShaderProgram                               _shaderProgram;

    Map*                                        _map;
    std::unordered_map<uint32_t, sEmitter*>*    _particleEmitters;

    ADD_MONITORING_VAR

    Camera                                      _camera;

    std::unordered_map<uint32_t, Entity*>       _transparentEntities;
END_SYSTEM(RenderingSystem)
