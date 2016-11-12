#pragma once

#include <array>
#include <memory>
#include <utility>

#include <ECS/Entity.hpp>
#include <ECS/EntityManager.hpp>

#include <Engine/Window/IInputEvent.hpp>
#include <Engine/Core/GameState.hpp>

#include <Game/EntityFactory.hpp>
#include <Game/Map.hpp>

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
