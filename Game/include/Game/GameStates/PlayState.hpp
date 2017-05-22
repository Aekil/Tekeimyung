/**
* @Author   Guillaume Labey
*/

#pragma once

#include <array>
#include <memory>
#include <utility>

#include <ECS/Entity.hpp>
#include <ECS/EntityManager.hpp>

#include <Engine/Window/IInputEvent.hpp>
#include <Engine/Core/GameState.hpp>

#include <Engine/EntityFactory.hpp>
#include <Engine/Utils/EventSound.hpp>

START_GAMESTATE(PlayState, "Game")
 public:
    ~PlayState();

    void                                onEnter();
    void                                setupSystems() override final;
    bool                                init();
    bool                                update(float elapsedTime);

private:
    std::pair <Keyboard::eKey, IInputEvent *>    _pair;

    tEventSound*                        _backgroundGameMusic = nullptr;
END_GAMESTATE(PlayState)
