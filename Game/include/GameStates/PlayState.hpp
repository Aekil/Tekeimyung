#pragma once

#include <array>
#include <memory>

#include "Core/GameState.hpp"
#include "Map.hpp"
#include "Entity.hpp"
#include "EntityFactory.hpp"

class PlayState: public GameState
{
 public:
    PlayState();
    virtual ~PlayState();

    virtual bool                        init();
    virtual bool                        update(float elapsedTime);

private:
    Entity*                             createEntity(const glm::vec3& pos, eArchetype type);
    void                                createTile(const glm::vec3& pos, eArchetype type);
    Entity*                             createParticlesEmittor(const glm::vec3& pos, eArchetype type);
    void                                goTo(Entity* emitter, Entity* character);

private:
    Map*                                _map;
    bool                                _windowImgui;

    // Entity enemy for demo
    Entity*                             _enemy;
};
