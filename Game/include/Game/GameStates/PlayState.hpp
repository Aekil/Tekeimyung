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

class WaveManager;

START_GAMESTATE(PlayState, "Game")
 public:
    ~PlayState();

    void                                onEnter();
    void                                setupSystems() override final;
    bool                                init();
    bool                                update(float elapsedTime);

public:
    bool                                _autoPlay = false;

private:
    std::pair <Keyboard::eKey, IInputEvent *>    _pair;

    tEventSound*                        _backgroundGameMusic = nullptr;
    tEventSound*                        _backgroundMenuMusic = nullptr;

    bool                                _mute;

    WaveManager*                        _waveManager = nullptr;

END_GAMESTATE(PlayState)
