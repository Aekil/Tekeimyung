#pragma once

#include <unordered_map>
#include <memory>

#include <ECS/System.hpp>

#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/Model.hpp>
#include <Engine/Graphics/Camera.hpp>

#include <Game/Components.hh>
#include <Game/Map.hpp>
#include <Game/Systems/ParticleSystem.hpp>

class RenderingSystem: public System
{
public:
    RenderingSystem(Map* map, std::unordered_map<uint32_t, sEmitter*>* particleEmitters);
    virtual ~RenderingSystem();

    virtual void update(EntityManager& em, float elapsedTime);
    virtual bool                            init();
    void                                    renderEntity(Entity* entity);
/*    void                                    renderEntities(EntityManager& em, std::list<uint32_t>::const_iterator& it, uint16_t layer, uint32_t x, uint32_t y);*/
    void                                    renderParticles(EntityManager& em);
    const ShaderProgram&                    getShaderProgram() const;

private:
    std::shared_ptr<Model>                  getModel(Entity* entity);

private:
    // Shader program
    ShaderProgram                               _shaderProgram;

    Map*                                        _map;
    std::unordered_map<uint32_t, sEmitter*>*    _particleEmitters;

    Camera                                      _camera;
};
