/**
* @Author   Guillaume Labey
*/

#pragma once

#include <memory>

#include <ECS/System.hpp>
#include <ECS/World.hpp>

#include <Engine/Core/GameState.hpp>
#include <Engine/Utils/EventSound.hpp>

class PlayState;

START_GAMESTATE(ConsoleState, "Console")
 public:
    ~ConsoleState();

    void                                onEnter() override final;
    void                                setupSystems() override final;
    bool                                init() override final;
    bool                                update(float elapsedTime) override final;

    static void                         handleCheatCodeKillAll(PlayState* playState);
    static void                         handleCheatCodeGiveMeGold(PlayState* playState);
    static void                         handleCheatCodeBuildForMe(PlayState* playState);
    static void                         handleCheatCodePlayForMe(PlayState* playState);
    static void                         handleCheatCodeAegis(PlayState* playState);
    static void                         handleCheatCode(PlayState* playState, const std::string& cheatCode);

 private:
    tEventSound*                        _backgroundGameMusic = nullptr;

    Entity*                             _console = nullptr;

    static Entity::sHandle              _lastBuiltBaseTurretHandle;
END_GAMESTATE(ConsoleState)
