/**
* @Author   Guillaume Labey
*/

#pragma once

#include <ECS/System.hpp>
#include <ECS/World.hpp>

#include <Engine/Core/GameState.hpp>
#include <Engine/Utils/EventSound.hpp>


START_GAMESTATE(PauseState, "Pause")
 public:
    ~PauseState();

    void                                onEnter() override final;
    void                                setupSystems() override final;
    bool                                init() override final;
    bool                                update(float elapsedTime) override final;

 private:
    tEventSound*                        _backgroundGameMusic = nullptr;
END_GAMESTATE(PauseState)
