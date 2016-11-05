#pragma once

#include <array>
#include <memory>
#include <utility>
#include "Core/GameState.hpp"
#include "Core/Map.hpp"
#include "Entity.hpp"
#include "EntityFactory.hpp"
#include <Window/IInputEvent.hpp>

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
    std::pair <Keyboard::eKey, IInputEvent *>    _pair;

    // Entity enemies for demo
    Entity*                             _enemy;
    Entity*                             _enemy2;
};
