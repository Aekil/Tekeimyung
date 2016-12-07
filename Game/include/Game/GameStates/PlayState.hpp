#pragma once

#include <array>
#include <memory>
#include <utility>

#include <ECS/Entity.hpp>
#include <ECS/EntityManager.hpp>

#include <Engine/Window/IInputEvent.hpp>
#include <Engine/Core/GameState.hpp>
#include <Engine/Graphics/Animation.hpp>

#include <Game/EntityFactory.hpp>
#include <Game/Map.hpp>

START_GAMESTATE(PlayState)
 public:
    virtual ~PlayState();

    virtual void                        onEnter();
    virtual bool                        init();
    virtual bool                        update(float elapsedTime);

    Entity*                             getSelectedEntity();

private:
    Map*                                _map;
    std::pair <Keyboard::eKey, IInputEvent *>    _pair;
END_GAMESTATE(PlayState)
